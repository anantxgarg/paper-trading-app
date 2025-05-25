#include "Market.h"
#include <iostream>

void Market::addStock(const Stock& stock){
    stocks[stock.getSymbol()] = stock;
}

Stock* Market::getStock(const std::string& symbol){
    auto it = stocks.find(symbol);
    if (it != stocks.end()) {
        return &it->second;
    }
    return nullptr;  
}

void Market::updateStockPrice(const std::string& symbol, double newPrice){
    Stock* stock = getStock(symbol);
    if (stock != nullptr) {
        stock->updatePrice(newPrice);
    } else {
        std::cout << "Stock " << symbol << " not found in market.\n";
    }
}
void Market::viewAllStocks() const{
    std::cout << "Available Stocks:\n";
    for (const auto& [symbol, stock] : stocks) {
        std::cout << symbol << " - $" << stock.getPrice() << std::endl;
    }
}