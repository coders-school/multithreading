#include <future>

int get_number()
{
    return 10;
}

std::future<int> get_number_async()
{
    // TODO: Your implementation goes here
    return std::future<int>{};
}

int main()
{
    auto future = get_number_async();
    return future.get();
}

