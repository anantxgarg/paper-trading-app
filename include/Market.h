#ifndef MARKET_H
#define MARKET_H

#include "Stock.h"
#include <map>

class Market {
private:
    std::map<std::string, Stock> stocks; // symbol -> Stock

public:
    void addStock(const Stock& stock);
    Stock* getStock(const std::string& symbol);
    void updateStockPrice(const std::string& symbol, double newPrice);

    void viewAllStocks() const;
};

#endif
