#include <catch2/catch.hpp>
#include "../src/Reflection.hpp"

TEST_CASE("Can create reflection", "[reflection]")
{
    Reflection reflection("name", "answer", 55, 64);

    SECTION("Construct sets 'chosen_' field to false")
    {
        REQUIRE_FALSE(reflection.chosen_);
    }
}