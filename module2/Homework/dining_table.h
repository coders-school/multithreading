#ifndef _DINING_TABLE_H_
#define _DINING_TABLE_H_

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

#include "philosopher.h"

class Fork {
    int id_;
public:
    mutable std::mutex mtx_;
    Fork(int id) : id_(id) {}

};


class Table {
    int seats_;
    int wait_for;
    std::shared_ptr<Table> self_;
    std::vector<std::shared_ptr<Fork> > forks;
    std::vector<std::shared_ptr<Philosopher> > philosophers;
    std::mutex m_update_mtx;
    std::condition_variable m_cond_update;
    
    void drawHeader();
    void drawStatus();
    void joinPhilosopherToTheFeast(std::shared_ptr<Philosopher> p);
public:
    Table(int seats);
    void updateCallback(int id);
    void Run();
    int seats() { return seats_; }
};


#endif // _DINING_TABLE_H_