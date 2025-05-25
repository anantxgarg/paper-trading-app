from alpha_vantage.timeseries import TimeSeries
import sqlite3
import time

API_KEY = 'L10S0WZHSNAA0OV7'  # Replace with your real key
symbols = ['AAPL', 'TSLA', 'GOOGL']
prices = {}

ts = TimeSeries(key=API_KEY, output_format='json')

# Connect to SQLite database (creates it if it doesn't exist)
conn = sqlite3.connect('stock_data.db')
cursor = conn.cursor()

# Create table if it doesn't exist
cursor.execute('''
CREATE TABLE IF NOT EXISTS stock_prices (
    symbol TEXT NOT NULL,
    date TEXT NOT NULL,
    open REAL,
    high REAL,
    low REAL,
    close REAL,
    volume INTEGER,
    PRIMARY KEY (symbol, date)
)
''')

for symbol in symbols:
    try:
        data, meta_data = ts.get_daily(symbol=symbol, outputsize='compact')
        latest_api_date = max(data.keys())  # Most recent date in the fetched data

        # Get the latest date for this symbol in your local database
        cursor.execute('SELECT MAX(date) FROM stock_prices WHERE symbol = ?', (symbol,))
        result = cursor.fetchone()
        latest_db_date = result[0]  # Could be None if no data yet

        if latest_db_date == latest_api_date:
            print(f"No new data for {symbol} (latest date {latest_db_date})")
            continue  # Skip to next symbol

        # If there's new data, process all entries (could be >1 new rows)
        for date, daily_data in data.items():
            if latest_db_date is not None and date <= latest_db_date:
                continue  # Skip already-existing dates

            cursor.execute('''
                INSERT OR IGNORE INTO stock_prices (symbol, date, open, high, low, close, volume)
                VALUES (?, ?, ?, ?, ?, ?, ?)
            ''', (
                symbol,
                date,
                float(daily_data['1. open']),
                float(daily_data['2. high']),
                float(daily_data['3. low']),
                float(daily_data['4. close']),
                int(daily_data['5. volume'])
            ))

        print(f"Updated data for {symbol} (new data up to {latest_api_date})")
        time.sleep(12)

    except Exception as e:
        print(f"Error fetching data for {symbol}: {e}")

conn.commit()
conn.close()