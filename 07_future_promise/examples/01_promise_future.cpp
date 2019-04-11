#include <future>
#include <thread>
#include <iostream>

int main()
{
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    auto function = [] (std::promise<int> promise)
    {
        // ...
        promise.set_value(10);
    };
    std::thread t(function, std::move(promise));
    std::cout << future.get() << std::endl;
    t.join();
    return 0;
}
