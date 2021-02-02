#include <memory>
#include <mutex>
#include <thread>
#include <iostream>

class Singleton {
    static std::unique_ptr<Singleton> instance_;
    static std::once_flag flag_;
    
    Singleton() = default;
    
public:
    static Singleton& getInstance()
    {
        std::call_once(flag_, [&] {
            instance_.reset(new Singleton{});
        });
        return *instance_;
    }
};

std::unique_ptr<Singleton> Singleton::instance_ = nullptr;
std::once_flag Singleton::flag_;

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
