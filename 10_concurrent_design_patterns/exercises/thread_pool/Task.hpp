#pragma once
#include <vector>
#include <string>
#include <functional>

struct Task {
    using Numbers = std::vector<int>;
    using It = Numbers::iterator;

    std::string id = "None";
    std::function<void(const Numbers &, Numbers &)> algo;
    Numbers input{};
    Numbers output{};
};
