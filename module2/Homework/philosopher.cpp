#include "philosopher.h"
#include "dining_table.h"

using namespace std::chrono_literals;

EatTimeEngine* EatTimeEngine::pinstance_{nullptr};
std::mutex EatTimeEngine::mutex_;

EatTimeEngine* EatTimeEngine::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(pinstance_ == 0)
    {
        std::random_device rd;
        pinstance_ = new EatTimeEngine(rd());
    }
    return pinstance_;
}

/* ***********************************************************
                    class Philosopher
   *********************************************************** */ 
std::string Philosopher::readStatus() { 
    std::lock_guard<std::mutex> lock(mtx_);
    std::string s;
    switch (status_)
    {
    case PhilStatus::Done:
        s = "|  ******  ";
        break;
    case PhilStatus::Eating:
        s = "|  Eating  ";
        break;
    default:
        s = "|          ";
        break;
    }
    return s;
}

PhilStatus Philosopher::getStatus() {
    std::lock_guard<std::mutex> lock(mtx_);
    return status_;
}

void Philosopher::startEating()
{
    std::lock_guard<std::mutex> lock(mtx_);
    status_ = PhilStatus::Eating;
}

void Philosopher::stopEating() {
    std::lock_guard<std::mutex> lock(mtx_);
    status_ = PhilStatus::Thinking;
}

void Philosopher::finished() {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        status_ = PhilStatus::Done;
    }
//    std::stringstream ss;
//    ss << "Philosopher " << id_ << " finished " << quota << " dishes." << std::endl;
//    std::cout << ss.rdbuf();
    p_table->updateCallback(id_);
}

void Philosopher::threadStartTheFeast() {
    int counter = this->quota;
    std::stringstream ss;

    while(counter--) {
        int t;

        {
            std::scoped_lock both_forks(fork_r->mtx_, fork_l->mtx_);
            startEating();
            t = lottery->pickTime();
            p_table->updateCallback(id_);
            std::this_thread::sleep_for(t * 10ms);
        }

        stopEating();   // Eating finished
        p_table->updateCallback(id_);

    }
    finished();
}
