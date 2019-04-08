#include <memory>
#include <mutex>
#include <thread>
#include <iostream>

class Singleton {
    static std::unique_ptr<Singleton> instance_;
    
    Singleton() = default;
    
public:
    // Is this thread safe?
    // Yes, it is. Static initializatio is thread safe from C++11
    static Singleton& getInstance()
    {
        if (!instance_)
        {
            instance_.reset(new Singleton{});
        }
        return *instance_;
    }
};

std::unique_ptr<Singleton> Singleton::instance_ = nullptr;

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
