#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <sstream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>
#include <thread>
#include "Philosopher.hpp"
#include "Book.hpp"
#include "Print.hpp"
#include "Reflection.hpp"
#include "Calculate.hpp"
#include "Wait.hpp"
#include "Write.hpp"


using namespace std::chrono_literals;
std::condition_variable cv_;

Philosopher::Philosopher(const std::string name, std::mutex & left, std::mutex & right, Book & book,
            std::vector<std::string> & questions)
    :
    name_(name),
    forkLeft_(left),
    forkRight_(right),
    book_(book),
    questions_(questions)
{
    generateAnswers();
    lastMeal_ = std::chrono::steady_clock::now();
    //showAllAnswers();
    printFunc("has just born", name_);
}

Philosopher::Philosopher(Philosopher&& obj) :
    name_(std::move(obj.name_)),
    forkLeft_(obj.forkLeft_),
    forkRight_(obj.forkRight_),
    book_(obj.book_),
    questions_(obj.questions_)
    { }

void Philosopher::dine()
{
    while (alive_)
    {
        updateStatus();
        eat();
        think();
        chooseAndAnswer();
    }
    printFunc("just died", name_);
}

void Philosopher::updateStatus()
{
    auto now = std::chrono::steady_clock::now();
    auto periodFromLastMeal = std::chrono::duration_cast<std::chrono::seconds>(now - lastMeal_).count();
    bool isStarving = (periodFromLastMeal >= diesAfterWhileSleeping_);
    if (sleeping_ and isStarving)
    {
        alive_ = false;
    }
    else if (!sleeping_)
    {
        if (periodFromLastMeal >= diesAfterNoEat_)
        {
            alive_ = false;
        }
        else if (periodFromLastMeal >= cantEatBefore_)
        {
            full_ = false;
        }
    }
}

void Philosopher::eat()
{
    bool isAliveNotSleepingAndNotFull = (alive_ and !sleeping_ and !full_ );
    if (isAliveNotSleepingAndNotFull)
    {      
        std::scoped_lock l(forkLeft_, forkRight_);
        printFunc("has just picked up both forks and he starts eating", name_);
        wait();
        printFunc("has finished his meal", name_);
        full_ = true;
        lastMeal_ = std::chrono::steady_clock::now();
    }
    updateStatus();
}

void Philosopher::think()
{
    bool isAliveNotSleepingAndFull = (alive_ && !sleeping_ && full_);
    if (isAliveNotSleepingAndFull)
    {
        wait();
        if (currentQuestion < questions_.size())
        {

            for (size_t i = 0; i < 10; i++)
            {
                updateStatus();
                if (!alive_ || sleeping_)
                    break;
                if (answersAnswered_[i] == true)
                    continue;
                auto start = std::chrono::steady_clock::now();

                std::shared_lock<std::shared_mutex> lock(book_.mutexBook_);
                printFunc("accessed the book (for reading)", name_);
                auto tmpResult = calculate(questions_[currentQuestion], answers_[i]);
                lock.unlock();
                wait();

                auto end = std::chrono::steady_clock::now();
                auto tmpPeriod = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
                writeToBook(name_, answers_[i], tmpResult, tmpPeriod, book_);
                printFunc("writes down his", i, tmpResult, name_);
                answersAnswered_[i] = true;
                updateStatus();
            }
            if (std::all_of(answersAnswered_.begin(), answersAnswered_.end(), [](const bool & obj){ return obj == true; }))
            {
            readyToAnswer_ = true;
            currentQuestion++;
            std::for_each(answersAnswered_.begin(), answersAnswered_.end(), [](bool & obj){ obj = false; });
            }
        }
        else
            printFunc("found no new questions in the queue", name_);
    }
    else if (alive_ && sleeping_)
    {
        printFunc("is sleeping", name_);
        wait();
    }
    updateStatus();
}

void Philosopher::chooseAndAnswer()
{
    if (alive_ && readyToAnswer_ && !sleeping_)
    {
        std::lock_guard<std::shared_mutex> lock(book_.mutexBook_);
        auto search = std::max_element(book_.reflections_.begin(), book_.reflections_.end(),
            [&](const Reflection &obj1, const Reflection &obj2) {
            return (obj1.philosopher_ == this->name_ && obj2.philosopher_ == this->name_
                && (obj1.result_ * obj1.period_) < (obj2.result_ * obj2.period_));
            });

        printFunc(search->result_, currentQuestion, name_);
        search->chosen_ = true;
        readyToAnswer_ = false;
    }
    updateStatus();
}

void Philosopher::generateAnswers()
{
    size_t n = answers_.size();
    for (size_t i = 0; i < n; i++)
        answers_[i] = getRandomAnswer();
}

std::string Philosopher::getRandomAnswer()
{
    srand(time(NULL));
    std::string answer;
    for (size_t i = 0; i < 6; ++i)
        answer += getRandomChar();

    return answer;
}

char Philosopher::getRandomChar()
{
    std::random_device rd;
    std::mt19937 e(rd());

    std::vector<char> v_char;
    v_char.reserve(95);
    constexpr short int firstASCIIcharacter = 32;
    std::generate_n(std::back_inserter(v_char),
                    95,
                    [ASCIIcharacter = firstASCIIcharacter]() mutable { return static_cast<char>(ASCIIcharacter++); });

    std::uniform_int_distribution<> distr(0, sizeof(v_char) - 1);

    return v_char[distr(e)];
}