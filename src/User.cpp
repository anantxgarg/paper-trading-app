#include "User.h"
#include <iostream>

User::User(std::string name, double initialBalance)
    : username(name), balance(initialBalance){
}

void User::buyStock(const std::string& stockSymbol, int quantity, double price) {
    double totalCost = quantity * price;

    if (totalCost > balance) {
        std::cout << "Not enough balance to buy " << quantity << " shares of " << stockSymbol << std::endl;
        return;
    }

    balance -= totalCost;

    // Check if the stock exists in the portfolio
    if (portfolio.find(stockSymbol) != portfolio.end()) {
        // Stock exists, update quantity and average price
        StockInfo& stockInfo = portfolio[stockSymbol];

        // Update the quantity
        stockInfo.quantity += quantity;

        // Update the average price (weighted average)
        stockInfo.avgPrice = ((stockInfo.avgPrice * stockInfo.quantity) + (price * quantity)) / (stockInfo.quantity + quantity);
    } else {
        // New stock, add to the portfolio
        portfolio[stockSymbol] = StockInfo{quantity, price};
    }

    std::cout << "Successfully bought " << quantity << " shares of " << stockSymbol << " at $" << price << " each." << std::endl;
}

void User::sellStock(const std::string& stockSymbol, int quantity, double price){
    if (portfolio.find(stockSymbol) == portfolio.end() || portfolio[stockSymbol].quantity < quantity){
        std::cout << "Not enough shares to sell!" << std::endl;
        return;
    }

    double totalEarnings = quantity * price;
    balance += totalEarnings;
    portfolio[stockSymbol].quantity -= quantity;

    if (portfolio[stockSymbol].quantity == 0) {
        portfolio.erase(stockSymbol); // Clean up zero holdings
    }

    std::cout << "Successfully sold " << quantity << " stocks of " << stockSymbol << std::endl;
}

void User::viewPortfolio() const {
    if (portfolio.empty()) {
        std::cout << "Portfolio is empty.\n";
        return;
    }

    for (const auto& item : portfolio) {
        const std::string& symbol = item.first;  // Stock symbol
        const StockInfo& stockInfo = item.second;  // StockInfo object

        std::cout << symbol << " : " << stockInfo.quantity << " shares, "
                  << "Average Price: $" << stockInfo.avgPrice << std::endl;
    }
}

double User::getBalance() const{
    return balance;
}

std::string User::getUsername() const{
    return username;
}
