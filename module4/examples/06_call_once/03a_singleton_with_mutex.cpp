#include <memory>
#include <mutex>
#include <thread>
#include <iostream>

class Singleton {
    static std::unique_ptr<Singleton> instance_;
    
    Singleton() = default;
    
public:
    static Singleton& getInstance()
    {
        std::mutex mutex_;
        std::unique_lock<std::mutex> lock(mutex_);
        if (!instance_)
        {
            instance_.reset(new Singleton{});
        }
        lock.unlock();

        return *instance_;
    }
};

std::unique_ptr<Singleton> Singleton::instance_ = nullptr;

// are here any problems?

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
