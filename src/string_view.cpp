#include <iostream>
#include <string>
#include <string_view>
#include <cstring>

using namespace std;

void sv2cstr() {
    string_view sv{ "balloon" };
    sv.remove_suffix(3);

    {
        string str{ sv };
        const char* cstr{ str.c_str() };
        cout << str << " has " << strlen(cstr) << " letter(s)\n";
    }

    {
        string str{ sv };
        const char* cstr{ sv.data() };
        cout << str << " has " << strlen(cstr) << " letter(s)\n";
    }
}

void return_sv() {
    auto func = []() {
        string name{"World"};
        string_view view{ name };
        return view;
    };

    string_view view{ func() };
    cout << "Hello " << view << endl; // Undefined behavior
}

int main() {
    sv2cstr();
    return_sv();
    return 0;
}