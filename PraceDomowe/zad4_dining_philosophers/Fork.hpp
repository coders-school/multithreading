#pragma once

#include <mutex>

class Fork{
private:
    std::mutex forkMutex;

public:
    std::mutex& getMutex();
};
