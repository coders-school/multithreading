#include "Table.hpp"
#include <mutex>
#include <array>

struct fork
{
   std::mutex mutex;
};

struct table
{
   std::array<fork, 5> forks;
};
