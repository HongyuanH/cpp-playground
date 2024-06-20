#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

// Interview question:
// Sort words based on Welsh:
// a,b,c,ch,d,dd,e,f,ff,g,ng,h,i,j,l,ll,m,n,o,p,ph,r,rh,s,t,th,u,w,y
class WelshComp {
public:
    WelshComp(vector<string> table) {
        for (size_t i=0; i<table.size(); i++) {
            lut[table[i]] = i;
        }
    }

    // operator() returns true if a comes first in the list after sorting
    bool operator()(const string& a, const string& b) {
        auto getOrder = [this](const string& x, size_t& i) {
            int n;
            if (i+1<x.size() && lut.count(x.substr(i, 2))) {
                n = lut.at(x.substr(i, 2));
                i += 2;
            } else if (lut.count(x.substr(i, 1))) {
                n = lut.at(x.substr(i, 1));
                i += 1;
            } else {
                throw runtime_error("Invalid word: not found in lut");
            }
            return n; 
        };

        size_t iA = 0;
        size_t iB = 0;
        while (iA<a.size() && iB<a.size()) {
            int nA = getOrder(a, iA);
            int nB = getOrder(b, iB);
            if (nA < nB) {
                return true;
            } else if (nA > nB) {
                return false;
            }
        }
        // at this point, a is a substr of b, or b is a substr of a
        return a.size() < b.size();
    }

private:
    unordered_map<string, int> lut;
};

int main() {
    vector<string> table {"a","b","c","ch","d","dd","e","f","ff","g","ng","h","i","j","l","ll","m","n","o","p","ph","r","rh","s","t","th","u","w","y"};
    vector<string> input {"ddr", "nah", "dea", "dd", "ngah"};
    //vector<string> output {"dea", "dd", "ddr", "ngah", "nah"};
    WelshComp comp(table);
    sort(input.begin(), input.end(), comp);
    for (const auto& word : input) {
        cout << word << endl;
    }
    return 0;
}