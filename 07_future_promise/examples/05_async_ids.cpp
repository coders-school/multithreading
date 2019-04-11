#include <future>
#include <vector>
#include <chrono>
#include <iostream>

int main() 
{
    auto mainThreadId = std::this_thread::get_id();
    std::cout << "Main thread id: " << mainThreadId << '\n';
    std::vector<std::future<void>> futures;
    for (int i = 0; i < 20; ++i)
    {
        auto fut = std::async(/* play with policies here :) */ [=]
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            auto threadId = std::this_thread::get_id();
            //std::cout << threadId << '\n';
            if (mainThreadId == threadId)
                std::cout << "    I was deferred!\n"; 
        });
        futures.emplace_back(std::move(fut));
    }
    for (auto && fut : futures)
        fut.wait();
    std::cout << '\n';
}
