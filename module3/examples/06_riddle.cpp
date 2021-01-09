#include <future>
#include <iostream>
#include <string>

int main() {
    std::string x = "x";

    std::async(std::launch::async, [&x]() {
        x = "y";
    });
    std::async(std::launch::async, [&x]() {
        x = "z";
    });

    std::cout << x;
}
