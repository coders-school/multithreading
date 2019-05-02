#include "ActiveObject.hpp"
#include "Object.hpp"
#include <iostream>
#include <thread>

int main()
{
    auto mainThreadId = std::this_thread::get_id();
    std::cout << "mainThreadId: " << mainThreadId << '\n';

    ActiveObject ao{std::make_unique<Object>()};
    auto pushFuture = ao.push(50);
    auto popFuture = ao.pop();
    
    std::cout << popFuture.get() << '\n';

    return 0;
}