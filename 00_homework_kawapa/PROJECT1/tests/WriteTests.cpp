#include <catch2/catch.hpp>
#include "../src/Write.hpp"


SCENARIO("Writing to book")
{
    Book book;

    GIVEN("Result, period, answer and name")
    {
        auto result = GENERATE(take(25, random(-100'000, 100'000)));
        auto period = GENERATE(take(25, random(-100'000, 100'000)));
        std::string answer = "answer";
        std::string name = "name";

        WHEN("Called writeToBook method")
        {
            writeToBook(name, answer, result, period, book);

            THEN("Reflections should be written to the book")
            {
                auto it = std::find_if(book.reflections_.begin(), book.reflections_.end(), [&name](const Reflection &reflection){ return reflection.philosopher_ == name; });
                REQUIRE(it->result_ == result);
                REQUIRE(it->period_ == period);
                REQUIRE(it->answer_ == answer);
            }
        }
    }
}