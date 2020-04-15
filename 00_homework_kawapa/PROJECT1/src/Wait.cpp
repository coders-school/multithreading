#include "Wait.hpp"
#include <chrono>
#include <thread>

void wait()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 2000));
}