#pragma once
#include <mutex>

class Fork
{
    public:
        mutable std::mutex mutex;
        Fork(Fork &&) = default;
        Fork(const Fork &) = default;
        Fork() = default;

};