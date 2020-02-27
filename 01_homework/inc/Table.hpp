#include <mutex>
#include <array>

struct Fork
{
   std::mutex mutex;
};

template<std::size_t S>
class Table
{
    const std::size_t population = S;
public:
    std::array<Fork, S> forks;
    // array for starving values

    Table();
    ~Table();

    print();
};
