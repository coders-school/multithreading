#pragma once
#include <array>
#include <atomic>
#include <chrono>
#include <mutex>
#include <random>
#include <string>
#include <queue>
#include "Book.hpp"

struct Philosopher
{
    std::string name_;
    std::mutex & forkLeft_;
    std::mutex & forkRight_;
    std::array<std::string, 10> answers_;
    Book & book_;
    std::vector<std::string> & questions_;
    int currentQuestion = 0;

    bool alive_ = true;
    bool full_ = false;
    bool readyToAnswer_ = false;
    std::atomic<bool> sleeping_;
    
    std::chrono::steady_clock::time_point lastMeal_;
    int64_t diesAfter_ = 20;
    int64_t diesAfterWhileSleeping_ = 60;
    int64_t cantEatBefore_ = 3;

    Philosopher(const std::string, std::mutex &, std::mutex &, Book &, std::vector<std::string> &);
    Philosopher(Philosopher&& obj);

    void dine();
    void eat();
    void think();
    void chooseAndAnswer();
    void write(std::string &, std::string &, int, int64_t, int);
    void showAllAnswers();

    void generateAnswers();
    std::string getRandomAnswer();
    char getRandomChar();
    int calculate(std::string &, std::string &);
    void print(std::string str);
    void print(std::string str, int, int);
    void print(int, int);
    void updateStatus();
    void wait();
};