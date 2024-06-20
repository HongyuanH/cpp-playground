/*
This is a C++ question. Implement a class StockMarket in C++ that has the two following functions:

class StockMarket
{
public:
void addTrade(string stockName, int vol);
- This function keeps track the volumes transacted for a stock by adding the trade information
void printTop(int numberOfStock);
- This function prints the top numberOfStock stocks that has the highest volumes
}
And here's a sample calling sequence that uses the above class:
StockMarket stockMarket;
stockMarket.addTrade("GOOGLE", 50);
stockMarket.addTrade("APPLE", 150);
stockMarket.addTrade("GOOGLE", 100);
stockMarket.addTrade("MSFT", 250);
stockMarket.addTrade("GOOGLE", 200);
stockMarket.printTop(2);
Expected output:
GOOGLE 350
MSFT 250
*/

#include <iostream>
#include <unordered_map>
#include <map>

using namespace std;

class StockMarket {
public:
    void addTrade(string stockName, int vol) {
        auto item = data.find(stockName);
        if (item != data.end()) {
            auto range = sortedData.equal_range(item->second);
            for (auto iter=range.first; iter!=range.second; iter++) {
                if (iter->second == item->first) {
                    sortedData.erase(iter);
                    break;
                }
            }
            item->second += vol;
            sortedData.insert({item->second, stockName});
        } else {
            data.insert({stockName, vol});
            sortedData.insert({vol, stockName});
        }
    }

    void printTop(int numberOfStock) {
        for (const auto& item : sortedData) {
            cout << item.second << ":" << item.first << endl;
            numberOfStock--;
            if (numberOfStock==0) {
                return;
            }
        }
    }

    multimap<int, string, greater<int>> sortedData;
    unordered_map<string, int> data;
};

int main() {
    StockMarket stockMarket;
    stockMarket.addTrade("GOOGLE", 50);
    stockMarket.addTrade("APPLE", 150);
    stockMarket.addTrade("GOOGLE", 100);
    stockMarket.addTrade("MSFT", 250);
    stockMarket.addTrade("GOOGLE", 200);
    stockMarket.printTop(2);
    return 0;
}