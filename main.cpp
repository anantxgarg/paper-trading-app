// g++ -Iinclude -Llib src/*.cpp main.cpp -lsqlite3 -o stock_simulator
//hello
#include <iostream>
#include <fstream>
#include <map> 
#include <cstdlib>
#include <vector>
#include "User.h"
#include "Market.h"
#include "Stock.h"
#include "sqlite3.h"

using string = std::string;
using map = std::map<string, double>;

double getLatestPrice(const string& symbol) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    double price = -1.0;

    int rc = sqlite3_open("stock_data.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    string query = 
        "SELECT close FROM stock_prices WHERE symbol = ? ORDER BY date DESC LIMIT 1;";
    
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return -1;
    }

    // Bind the stock symbol to the query
    sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        price = sqlite3_column_double(stmt, 0);
    } else {
        std::cerr << "No price found for " << symbol << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return price;
}

map loadPrices(const std::vector<string>& symbols) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    map prices;

    int rc = sqlite3_open("stock_data.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    string query = 
        "SELECT close FROM stock_prices WHERE symbol = ? ORDER BY date DESC LIMIT 1;";

    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return {};
    }

    for (const auto& symbol : symbols) {
        sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            double price = sqlite3_column_double(stmt, 0);
            prices[symbol] = price;
        } else {
            std::cerr << "No price found for " << symbol << std::endl;
        }

        sqlite3_reset(stmt);  // Reset the statement for the next symbol
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return prices;
}

int main() {
    //system("py fetch_prices.py"); //Aim to run after U.S. market close (≥ 4 PM ET) so you’re sure the day’s “close” price has been finalized.

    User user("test_user", 100000.0); // $100K starting cash
    Market market;
    map pairs;

    std::vector<std::string> symbols = {"AAPL", "TSLA", "GOOGL"};
    pairs = loadPrices(symbols);

    for (const auto& [symbol, price] : pairs) {
        market.addStock(Stock(symbol, price));
    }

    std::cout << "Welcome, " << user.getUsername() << "!" << std::endl;
    market.viewAllStocks();

    std::cout << "What stock would you like to buy? ";
    string stockSymbol;
    std::cin >> stockSymbol;

    std::cout << "How many shares would you like to buy? ";
    int quantity;
    std::cin >> quantity;

    // Get current stock price from market
    Stock* selectedStock = market.getStock(stockSymbol);
    if (selectedStock) {
        double price = selectedStock->getPrice();
        user.buyStock(stockSymbol, quantity, price);
        user.viewPortfolio();
    } 
    else {
        std::cout << "Stock " << stockSymbol << " not found." << std::endl;
    }

    std::cout << "What stock would you like to sell? ";
    string stockToSell;
    std::cin >> stockToSell;

    std::cout << "How many shares would you like to sell? ";
    int quantityToSell;
    std::cin >> quantityToSell;

    // Retrieve current price from the market
    Stock* stock = market.getStock(stockToSell);
    if (stock) {
        double sellPrice = stock->getPrice();
        user.sellStock(stockToSell, quantityToSell, sellPrice);
        user.viewPortfolio();
    } else {
        std::cout << "Stock " << stockToSell << " not found." << std::endl;
    }

    std::cout << "Balance: $" << user.getBalance() << std::endl;

    return 0;
}
