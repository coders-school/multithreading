#include "Fork.hpp"

std::mutex& Fork::getMutex()
{
    return forkMutex;
}
