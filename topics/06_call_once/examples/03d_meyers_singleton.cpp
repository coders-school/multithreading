#include <memory>
#include <mutex>
#include <thread>
#include <iostream>

class Singleton {  
    Singleton() = default;
    
public:
    static Singleton& getInstance()
    {
        static Singleton instance_;
        return instance_;
    }
};

int main() {
    auto single = [] {
        auto& s1 = Singleton::getInstance();
        std::cout << &s1 << '\n';
    };
    std::thread t1(single);
    std::thread t2(single);
    t1.join();
    t2.join();
    single();

    return 0;
}
