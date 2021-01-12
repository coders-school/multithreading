#include <iostream>
#include <thread>

void foo() {}
void bar() {}

int main() {
    std::thread t1;
    std::thread t2(foo);
    std::thread t3(std::move(t2));
    t1 = std::thread(bar);

    std::cout << std::boolalpha << "t1: " << t1.joinable() << std::endl;
    std::cout << std::boolalpha << "t2: " << t2.joinable() << std::endl;
    std::cout << std::boolalpha << "t3: " << t3.joinable() << std::endl;
    
    t1.join();
    t3.join();
    return 0;
}