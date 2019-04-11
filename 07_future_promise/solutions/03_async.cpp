#include <future>
#include <thread>
#include <functional>
#include <iostream>
#include <string>

int get_number()
{
    return 10;
}

template<typename Function, typename... Args>
auto schedule(Function&& func, Args &&... args)
{
    using FunctionReturnType = std::invoke_result_t<std::decay_t<Function>, 
                                                    std::decay_t<Args>...>;
    std::promise<FunctionReturnType> p;
    auto f = p.get_future();
    auto wrapped_func = [&] (std::promise<FunctionReturnType> p) {
        try {
            p.set_value(func(std::forward<Args>(args)...));
        } catch(...) {
            p.set_exception(std::current_exception());
        }
    };
    std::thread t(wrapped_func, std::move(p));
    t.detach();
    return f;
}

auto temporary() {
    // Data race - appending a temporary value, because lambda captures by &
    // Solution is not easy in C++17: https://stackoverflow.com/questions/47496358/c-lambdas-how-to-capture-variadic-parameter-pack-from-the-upper-scope
    // It will be easy in C++20: [... args = std::forward<Args>(args)]
    return schedule([](std::string s){ return s + " appended"; }, "Something");
}

int main()
{
    auto fut1 = schedule(get_number); 
    std::cout << fut1.get() << '\n';
    auto fut2 = schedule([](int i){ return i*i; }, 15);
    std::cout << fut2.get() << '\n';
    auto fut3 = temporary();
    std::cout << fut3.get() << '\n';
    return 0;
}

