#include <future>
#include <thread>

int get_number()
{
    return 10;
}

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

int main()
{
    auto future = schedule(get_number); 
    return future.get();
}

