#ifndef STOCK_H
#define STOCK_H

#include <string>

class Stock {
private:
    std::string symbol;
    double currentPrice;

public:
    Stock() : symbol(""), currentPrice(0.0) {}
    Stock(std::string symbol, double currentPrice);

    std::string getSymbol() const;
    double getPrice() const;

    void updatePrice(double newPrice); // for price simulation or API update
};

#endif
