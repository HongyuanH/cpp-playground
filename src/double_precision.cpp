#include <iostream>
#include <iomanip> 

using namespace std;

int main() {
    double x = 1.23456789;
    double y = 0.0123456789;
    cout << setprecision (20) << "x=" << x << ", y=" << y << ", x/y=" << x/y << endl;
    int64_t ix = static_cast<int64_t>(x * 10000000);
    int64_t iy = static_cast<int64_t>(y * 10000000);
    cout << "ix=" << ix << ", iy=" << iy << ", ix%iy=" << ix%iy << endl;
    return 0;
}