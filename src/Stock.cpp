#include "Stock.h"
#include <iostream>

Stock::Stock(std::string symbol_, double currentPrice_){
    symbol = symbol_;
    currentPrice = currentPrice_;
}

std::string Stock::getSymbol() const{
    return symbol;
}

double Stock::getPrice() const{
    return currentPrice;
}
void Stock::updatePrice(double newPrice){
    currentPrice = newPrice;
}
