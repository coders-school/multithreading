#include <future>
#include <thread>
#include <functional>
#include <iostream>
#include <string>

int get_number()
{
    return 10;
}

// TODO: Change schedule function to be more generic
// It should take any callable of any signature
std::future<int> schedule(std::function<int()> func)
{
    std::promise<int> p;
    std::future<int> f = p.get_future();
    auto wrapped_func = [func] (std::promise<int> p) {
        try {
            p.set_value(func());
        } catch(...) {
            p.set_exception(std::current_exception());
        }
    };
    std::thread t(wrapped_func, std::move(p));
    t.detach();
    return f;
}

// TODO: Commented code should work after proper implementation 
//auto temporary() {
//    return schedule([](std::string s){ return s + " appended"; }, "Something");
//}

int main()
{
    auto fut1 = schedule(get_number); 
    std::cout << fut1.get() << '\n';
    //auto fut2 = schedule([](int i){ return i*i; }, 15);
    //std::cout << fut2.get() << '\n';
    //auto fut3 = temporary();
    //std::cout << fut3.get() << '\n';
    return 0;
}

