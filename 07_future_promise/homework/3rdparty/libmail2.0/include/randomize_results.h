#pragma once
#include <cstdlib>
#include <ctime>

namespace libmail {

inline void randomize_results()
{
    std::srand(std::time(0));
}

}
