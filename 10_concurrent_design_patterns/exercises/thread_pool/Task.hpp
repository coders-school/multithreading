#pragma once
#include <vector>
#include <string>
#include <functional>
#include <future>
#include <tuple>

struct Task {
    using Numbers = std::vector<int>;
    using It = Numbers::iterator;
    using PromiseType = std::pair<std::string, Numbers>;

    std::string id = "None";
    std::function<void(const Numbers &, Numbers &)> algo;
    Numbers input{};
    Numbers output{};
    std::promise<PromiseType> promise{};
};
