#include <future>

int get_number()
{
    return 10;
}

std::future<int> get_number_async()
{
    std::promise<int> p;
    auto f = p.get_future();

    auto task = [](std::promise<int> p){
        p.set_value(get_number());
    };
    std::thread t{task, std::move(p)};
    t.detach();

    return f;
}

int main()
{
    auto future = get_number_async();
    return future.get();
}
