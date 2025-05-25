#include <sqlite3.h>
#include <iostream>
#include <string>

int main() {
    sqlite3* db;
    sqlite3_stmt* stmt;

    // Open the SQLite database
    if (sqlite3_open("stock_data.db", &db)) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Example query: Get latest close price for AAPL
    std::string sql = R"(
        SELECT date, close FROM stock_prices 
        WHERE symbol='AAPL' 
        ORDER BY date DESC LIMIT 1;
    )";

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Execute and fetch results
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string date(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        double close = sqlite3_column_double(stmt, 1);
        std::cout << "Latest AAPL close (" << date << "): $" << close << std::endl;
    } else {
        std::cout << "No data found for AAPL" << std::endl;
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
