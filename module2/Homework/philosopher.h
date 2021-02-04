#ifndef _PHILOSOPHER_H_
#define _PHILOSOPHER_H_

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <algorithm>
#include <random>
#include <memory>
#include <thread>
#include <chrono>
#include <sstream>
#include <functional>


class EatTimeEngine
{
private:
    std::mt19937 m_generator;
    std::uniform_int_distribution<int> m_distribution;
    static EatTimeEngine *pinstance_;
    static std::mutex mutex_;
protected:
    EatTimeEngine(std::random_device::result_type seed) : m_generator(seed), m_distribution(1, 100) {}
    ~EatTimeEngine() {}
public:
    int pickTime() {
        std::lock_guard<std::mutex> lock(mutex_);
        return m_distribution(m_generator);
    }
    static EatTimeEngine* GetInstance();
    EatTimeEngine(EatTimeEngine &other) = delete;           // Singletons should not be cloneable.
    void operator=(const EatTimeEngine &) = delete;         // Singletons should not be assignable.
};

/* ********************************************************************** */
enum class PhilStatus {
    Eating, Thinking, Done
};

class Table;
class Fork;

class Philosopher {
private:
    int id_;
    const int quota = 6; // How many dishes  every philosopher must eat
    EatTimeEngine* lottery;
    std::thread m_t;
    mutable std::mutex mtx_;
    PhilStatus status_;
    Table* p_table;
    std::shared_ptr<Fork> fork_r, fork_l;

    void startEating();
    void stopEating();
    void finished();
public:
    Philosopher(int id, Table* t, std::shared_ptr<Fork> &right, std::shared_ptr<Fork> &left) : 
                id_(id),
                status_(PhilStatus::Thinking),
                p_table(t),
                fork_r(right),
                fork_l(left) {
        lottery = EatTimeEngine::GetInstance();
    }

    ~Philosopher() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (m_t.joinable()) {
            m_t.join();
        }
    }

    Philosopher(const Philosopher&) = delete;               //Delete the copy constructor
    Philosopher& operator=(const Philosopher&) = delete;    //Delete the Assignment opeartor

    std::string readStatus();
    PhilStatus getStatus();
    void threadStartTheFeast();

    void setThread(std::thread &t) {
        m_t = std::move(t);
    }

    void endThread() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (m_t.joinable()) {
            m_t.join();
        }
    }

    int getId() { 
        std::lock_guard<std::mutex> lock(mtx_);
        return id_;
    }
};

#endif //_DINING_TABLE_H_
