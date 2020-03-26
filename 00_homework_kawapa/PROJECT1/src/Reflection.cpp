#include "Reflection.hpp"
#include <string>

Reflection::Reflection(std::string philosopher, std::string answer, int result, int64_t period, bool chosen)
        :
        philosopher_(philosopher_),
        answer_(answer),
        result_(result),
        period_(period),
        chosen_(chosen) { }