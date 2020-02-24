#pragma once
#include <ctime>
#include <cstdlib>

namespace libmail {

inline void randomize_results()
{
    std::srand(std::time(0));
}

}
