#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "Philosopher.hpp"
#include "Book.hpp"
#include "Reflection.hpp"

// number of philosophers and forks
constexpr size_t n = 5;
std::mutex forks[n];

using namespace std::chrono_literals;

void userInterface(std::vector<Philosopher> &, std::vector<std::string> &);

int main()
{
    std::vector<Philosopher> philosophers;
    philosophers.reserve(n);

    std::vector<std::thread> threads(n);
    std::vector<std::string> questions {"How are you?"};
    Book book;   

    std::string names[]{"Arystoteles", "Platon", "Epikur", "Siron",
                        "Demokryt", "Sokrates", "Zenon", "Parmenides", "Heraklit", "Tales"};

    for (size_t i = 0; i < n - 1; i++)
        philosophers.emplace_back(names[i], forks[i], forks[i+1], book, questions);
    philosophers.emplace_back(names[n - 1], forks[n - 1], forks[0], book, questions);

    for (size_t i = 0; i < n; i++)
        threads[i] = std::thread(&Philosopher::dine, &philosophers[i]);

    userInterface(philosophers, questions);

    for (auto &&i : threads)
        i.join();
    
    return 0;
}

void userInterface(std::vector<Philosopher> & philosophers, std::vector<std::string> & questions)
{
    std::string command;
    std::string question;
    std::string philo;

    while (1)
    {
        std::cin >> command;
        if (command == "question")
        {
            std::getline(std::cin, question);
            questions.emplace_back(question);
            std::cout << "Successfully added question!\n";
        }
        else if (command == "sleep")
        {
            std::cin >> philo;
            auto search = std::find_if(philosophers.begin(), philosophers.end(), [&](Philosopher & obj){
                return obj.name_ == philo; });
            search->sleeping_ = true;
            std::cout << search->name_ << " went to sleep\n";
        }
        else if (command == "wakeup")
        {
            std::cin >> philo;
            auto search2 = std::find_if(philosophers.begin(), philosophers.end(), [&](Philosopher & obj){
                return obj.name_ == philo; });
            search2->sleeping_ = false;
        }
        else
            std::cout << "Incorrect instruction\n";
    }
}