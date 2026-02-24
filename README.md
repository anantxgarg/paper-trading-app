# Paper Trading App

A C++ paper trading simulator that lets you practice buying and selling stocks using real historical price data.
---
_<small>*This project is actively under development and currently supports basic single-session trading.*</small>_

## Features

- Fetch and store daily stock price data via the Alpha Vantage API
- Simulate buying and selling stocks with a virtual cash balance
- Track portfolio holdings with average cost basis per position
- View all available market stocks and their latest closing prices
- Persistent local price history via SQLite

---

## Project Structure

```
paper-trading-app/
├── include/
│   ├── Market.h        # manages available stocks
│   ├── Stock.h         # symbol and price
│   └── User.h          # balance and portfolio
├── src/
│   ├── Market.cpp
│   ├── Stock.cpp
│   └── User.cpp
├── sqlite-dll-win-x64-3490100/
│   └── sqlite3.def     # SQLite Windows DLL exports
├── main.cpp            # Entry point — app logic and DB queries
├── fetch_prices.py     # Python script to fetch prices into SQLite
├── test.cpp            # Quick DB connection test
├── stock_data.db       # SQLite database (auto-created on first run)
└── .vscode/            # VS Code build configuration (MSYS2/MinGW)
```

---

## Prerequisites

### C++ Build Toolchain
- [MSYS2](https://www.msys2.org/) with MinGW-w64 (`g++` available at `C:\msys64\mingw64\bin\g++.exe`)
- SQLite3 library and header (`sqlite3.h`, `libsqlite3`)

### Python (for price fetching)
- Python 3.x
- `alpha_vantage` package:
  ```bash
  pip install alpha_vantage
  ```

### Alpha Vantage API Key
- Sign up for a free key at [alphavantage.co](https://www.alphavantage.co/)
- Replace the `API_KEY` value in `fetch_prices.py`

---

## Setup & Build

### 1. Fetch Stock Prices

Run the Python script to populate the local SQLite database with historical price data. This should be run after U.S. market close (≥ 4:00 PM ET) to ensure the day's closing price is finalized.

```bash
python fetch_prices.py
```

This creates `stock_data.db` and populates a `stock_prices` table with OHLCV data for the configured symbols (`AAPL`, `TSLA`, `GOOGL` by default). Subsequent runs only insert new data.

### 2. Build the App

From the project root, compile with:

```bash
g++ -Iinclude -Llib src/*.cpp main.cpp -lsqlite3 -o stock_simulator
```

Or use the pre-configured VS Code build task (`Ctrl+Shift+B`) which uses MSYS2's `g++`.

### 3. Run

```bash
./stock_simulator
```

---

## Usage

On launch, the app:
1. Loads the latest closing price for each stock from the local database
2. Creates a user with a $100,000 virtual starting balance
3. Displays all available stocks and their prices

You are then prompted to:
- **Buy** — enter a stock symbol and number of shares to purchase
- **Sell** — enter a stock symbol and number of shares to sell
- View your **updated portfolio** and **balance** after each transaction

### Example Session

```
Welcome, test_user!
Available Stocks:
AAPL - $189.30
GOOGL - $172.50
TSLA - $248.10

What stock would you like to buy? AAPL
How many shares would you like to buy? 10

Successfully bought 10 shares of AAPL at $189.30 each.
AAPL : 10 shares, Average Price: $189.30

What stock would you like to sell? AAPL
How many shares would you like to sell? 5

Successfully sold 5 stocks of AAPL
AAPL : 5 shares, Average Price: $189.30

Balance: $98056.50
```

---

## Adding More Stocks

To track additional symbols, add them to the `symbols` list in `fetch_prices.py`:

```python
symbols = ['AAPL', 'TSLA', 'GOOGL', 'MSFT', 'AMZN']
```

Then re-run the fetch script and recompile. The new symbols will appear automatically in the market view.

---

## Database Schema

The `stock_data.db` SQLite database contains a single table:

```sql
CREATE TABLE stock_prices (
    symbol  TEXT NOT NULL,
    date    TEXT NOT NULL,
    open    REAL,
    high    REAL,
    low     REAL,
    close   REAL,
    volume  INTEGER,
    PRIMARY KEY (symbol, date)
);
```

---

## Testing the Database Connection

A standalone test file is included to verify your SQLite setup:

```bash
g++ -Iinclude test.cpp -lsqlite3 -o test && ./test
```

Expected output:
```
Latest AAPL close (2025-01-15): $189.30
```

---

## Known Limitations

- **No persistence between sessions** — user balance and portfolio reset on each run (no save/load yet)
- **Single session only** — buy/sell is limited to one of each per run
- **Free API rate limits** — Alpha Vantage's free tier allows 25 requests/day and 5 requests/minute; the fetch script includes a 12-second delay between calls to stay within limits
- **No real-time prices** — prices are end-of-day closing values, not live quotes

---

## Roadmap

- [ ] Persist user portfolio and balance across sessions (SQLite)
- [ ] Support multiple buy/sell operations per session (menu loop)
- [ ] Calculate and display P&L per position
- [ ] Add transaction history
- [ ] Support portfolio value tracking over time
- [ ] Add price simulation mode (random walk) for offline use

---

## License

This project is for educational and personal use. Stock data is sourced from [Alpha Vantage](https://www.alphavantage.co/).
