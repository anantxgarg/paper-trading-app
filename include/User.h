#ifndef USER_H
#define USER_H

#include <string>
#include <map>
#include "sqlite3.h"

struct StockInfo {
    int quantity;
    double avgPrice;
};

class User{
private:
    std::string username;
    double balance;
    std::map<std::string, StockInfo> portfolio; // stockSymbol -> quantity

public:
    User(std::string name, double initialBalance);

    void buyStock(const std::string& stockSymbol, int quantity, double price);
    void sellStock(const std::string& stockSymbol, int quantity, double price);

    void viewPortfolio() const;
    double getBalance() const;
    std::string getUsername() const;
    
    // Serialization methods (for saving/loading user data)
    // std::string serialize() const;
    // static User deserialize(const std::string& data);
};

#endif
