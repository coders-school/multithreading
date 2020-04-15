#include <catch2/catch.hpp>
#include <mutex>
#include "../src/Philosopher.cpp"
#include "../src/Wait.hpp"
#include "../src/Print.hpp"

void printFunc( [[maybe_unused]] const std::string &str, 
                [[maybe_unused]] const std::string &name) {}

void printFunc( [[maybe_unused]] const std::string &str, 
                [[maybe_unused]] const int &i, 
                [[maybe_unused]] const int &result, 
                [[maybe_unused]] const std::string &name) {}

void printFunc( [[maybe_unused]] const int &i, 
                [[maybe_unused]] const int &j, 
                [[maybe_unused]] const std::string &name) {}

void wait() {}

std::chrono::steady_clock::time_point countTimeFromNow (uint16_t time)
{
    return std::chrono::steady_clock::now() - std::chrono::seconds(time);
}


SCENARIO("Can create philosopher")
{
    std::mutex forkLeft{};
    std::mutex forkRight{};
    Book book;
    std::vector<std::string> v_questions;
    Philosopher philosopher("name", forkLeft, forkRight, book, v_questions);

    REQUIRE(philosopher.answers_.size() == 10);
    REQUIRE(philosopher.alive_);
    REQUIRE_FALSE(philosopher.full_);
    REQUIRE_FALSE(philosopher.readyToAnswer_);
    REQUIRE_FALSE(philosopher.sleeping_);

}

SCENARIO("Philosopher sleeps and reaction for hunger", "[updateStatus]")
{
    std::mutex forkLeft{};
    std::mutex forkRight{};
    Book book;
    std::vector<std::string> v_questions;
    Philosopher philosopher("name", forkLeft, forkRight, book, v_questions);
    philosopher.sleeping_ = true;

    REQUIRE(philosopher.sleeping_);

    GIVEN("Time of last meal that happened EARLIER than 60s ago")
    {
        philosopher.lastMeal_ = countTimeFromNow(59);
        
        WHEN("Called update status method")
        {
            philosopher.updateStatus();

            THEN("Philosohper lives")
            {
                REQUIRE(philosopher.alive_ == true);
            }
        }
    }
     
    GIVEN("Time of last meal that happened EXACTLY 60s ago")
    {
        philosopher.lastMeal_ =  countTimeFromNow(60);

        WHEN("Called updateStatus method")
        {
            philosopher.updateStatus();

            THEN("Philosopher dies")
            {
                REQUIRE_FALSE(philosopher.alive_);
            }
        }
    }

     GIVEN("Time of last meal that happened OVER 60s ago")
    {
        philosopher.lastMeal_ = countTimeFromNow(61);

        WHEN("Called updateStatus method")
        {
            philosopher.updateStatus();

            THEN("Philosopher dies")
            {
                REQUIRE_FALSE(philosopher.alive_);
            }
        }
    }
}

SCENARIO("Philosopher is not sleeping and is full", "[updateStatus]")
{
    std::mutex forkLeft{};
    std::mutex forkRight{};
    Book book;
    std::vector<std::string> v_questions;
    Philosopher philosopher("name", forkLeft, forkRight, book, v_questions);
    
    philosopher.full_ = true;
    
    REQUIRE_FALSE(philosopher.sleeping_);
    REQUIRE(philosopher.full_);

    GIVEN("Time of last meal that happened EARLIER than 20s ago")
    {
        philosopher.lastMeal_ = countTimeFromNow(19);

        WHEN("Called updateStatus method")
        {
            philosopher.updateStatus();

            THEN("Philosopher lives and is not full")
            {
                REQUIRE(philosopher.alive_);
                REQUIRE_FALSE(philosopher.full_);
            }
        }
    }

    GIVEN("Time of last meal that happened EXACTLY 20s ago")
    {
        philosopher.lastMeal_ = countTimeFromNow(20);

        WHEN("Called update status method")
        {
            philosopher.updateStatus();

            THEN("Philosopher dies")
            {
                REQUIRE_FALSE(philosopher.alive_);
            }
        }
    }

    GIVEN("Time of last meal that happened OVER 20s ago")
    {
        philosopher.lastMeal_ = countTimeFromNow(21);

        WHEN("Called update status method")
        {
            philosopher.updateStatus();

            THEN("Philosopher dies")
            {
                REQUIRE_FALSE(philosopher.alive_);
            }
        }
    }

    GIVEN("Time of last meal that happened EARLIER than 3s ago")
    {
        philosopher.lastMeal_ = countTimeFromNow(2);

        WHEN("Called update status method")
        {
            philosopher.updateStatus();

            THEN("Philosopher stays full")
            {
                REQUIRE(philosopher.full_);
            }
        }
    }

    GIVEN("Time of last meal that happened EXACTLY 3s ago")
    {
        philosopher.lastMeal_ = countTimeFromNow(3);

        WHEN("Called update status method")
        {
            philosopher.updateStatus();

            THEN("Philosopher is not full")
            {
                REQUIRE_FALSE(philosopher.full_);
            }
        }
    }
}

SCENARIO("Philosopher is eating", "[eat]")
{
    std::mutex forkLeft{};
    std::mutex forkRight{};
    Book book;
    std::vector<std::string> v_questions;
    Philosopher philosopher("name", forkLeft, forkRight, book, v_questions);

    GIVEN("Philospher is alive, is not sleeping and not full")
    {
        REQUIRE(philosopher.alive_);
        REQUIRE_FALSE(philosopher.sleeping_);
        REQUIRE_FALSE(philosopher.full_);
        
        WHEN("Called eat method")
        {
            auto before = philosopher.lastMeal_;
            philosopher.eat();

            THEN("Philosopher is full and new lastMeal time is setup")
            {
                REQUIRE(philosopher.full_);
                REQUIRE(before != philosopher.lastMeal_);
            }
        }
    }

    GIVEN("Philosopher is alive, is not sleeping and is full")
    {
        philosopher.full_ = true;
        REQUIRE(philosopher.full_);
        REQUIRE(philosopher.alive_);
        REQUIRE_FALSE(philosopher.sleeping_);
       
        WHEN("Called eat method")
        {
            philosopher.eat();
            
            THEN("Nothing changes")
            {
                REQUIRE(philosopher.alive_);
                REQUIRE(philosopher.full_);
                REQUIRE_FALSE(philosopher.sleeping_);
            }
        }
    }

    GIVEN("Philosopher is alive, is sleeping and is not full")
    {
        philosopher.sleeping_ = true;
        REQUIRE(philosopher.sleeping_);
        REQUIRE(philosopher.alive_);
        REQUIRE_FALSE(philosopher.full_);
      
        WHEN("Called eat method")
        {
            philosopher.eat();
            
            THEN("Nothing changes")
            {
                REQUIRE(philosopher.alive_);
                REQUIRE(philosopher.sleeping_);
                REQUIRE_FALSE(philosopher.full_);
            }
        }
    }

    GIVEN("Philosopher is alive, is sleeping and is full")
    {
        philosopher.sleeping_ = true;
        philosopher.full_ = true;
        REQUIRE(philosopher.full_);
        REQUIRE(philosopher.sleeping_);
        REQUIRE(philosopher.alive_);
      
        WHEN("Called eat method")
        {
            philosopher.eat();
            
            THEN("Nothing changes")
            {
                REQUIRE(philosopher.alive_);
                REQUIRE(philosopher.sleeping_);
                REQUIRE(philosopher.full_);
            }
        }
    }

    GIVEN("Philosopher is not alive, is not sleeping and is not full")
    {
        philosopher.alive_ = false;
        REQUIRE_FALSE(philosopher.full_);
        REQUIRE_FALSE(philosopher.sleeping_);
        REQUIRE_FALSE(philosopher.alive_);

        WHEN("Called eat method")
        {
            philosopher.eat();

            THEN("Nothing changes")
            {
                REQUIRE_FALSE(philosopher.full_);
                REQUIRE_FALSE(philosopher.sleeping_);
                REQUIRE_FALSE(philosopher.alive_);
            }
        }
    }

    GIVEN("Philosopher is not alive, is not sleeping and is full")
    {
        philosopher.alive_ = false;
        philosopher.full_ = true;
        REQUIRE(philosopher.full_);
        REQUIRE_FALSE(philosopher.sleeping_);
        REQUIRE_FALSE(philosopher.alive_);

        WHEN("Called eat method")
        {
            philosopher.eat();

            THEN("Nothing changes")
            {
                REQUIRE(philosopher.full_);
                REQUIRE_FALSE(philosopher.sleeping_);
                REQUIRE_FALSE(philosopher.alive_);
            }
        }
    }

    GIVEN("Philosopher is not alive, is sleeping and is not full")
    {
        philosopher.alive_ = false;
        philosopher.sleeping_= true;
        REQUIRE(philosopher.sleeping_);
        REQUIRE_FALSE(philosopher.full_);
        REQUIRE_FALSE(philosopher.alive_);

        WHEN("Called eat method")
        {
            philosopher.eat();

            THEN("Nothing changes")
            {
                REQUIRE(philosopher.sleeping_);
                REQUIRE_FALSE(philosopher.full_);
                REQUIRE_FALSE(philosopher.alive_);
            }
        }
    }

    GIVEN("Philosopher is not alive, is sleeping and is full")
    {
        philosopher.alive_ = false;
        philosopher.sleeping_= true;
        philosopher.full_ = true;
        REQUIRE(philosopher.sleeping_);
        REQUIRE(philosopher.full_);
        REQUIRE_FALSE(philosopher.alive_);

        WHEN("Called eat method")
        {
            philosopher.eat();

            THEN("Nothing changes")
            {
                REQUIRE(philosopher.sleeping_);
                REQUIRE(philosopher.full_);
                REQUIRE_FALSE(philosopher.alive_);
            }
        }
    }
}



