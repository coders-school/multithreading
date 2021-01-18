#include <future>
#include <iostream>

int get_number()
{
    return 10;
}

int throw_sth() {
    throw std::runtime_error{"Sorry"};
}

std::future<int> schedule(std::function<int()> func) {
    std::promise<int> p;
    auto f = p.get_future();

    auto task = [func](std::promise<int> p){
        try {
            p.set_value(func());
        } catch(...) {
            p.set_exception(std::current_exception());
        }
    };
    std::thread t{task, std::move(p)};
    t.detach();

    return f;
}

int main()
{
    auto future = schedule(throw_sth);
    try {
        return future.get();
    } catch(const std::exception & e) {
        std::cerr << e.what() << '\n';
    }
}
