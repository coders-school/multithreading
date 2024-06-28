#include "philosopher.h"
#include "dining_table.h"


Table::Table(int seats) : seats_(seats) {
//    self_ = std::shared_ptr<Table>(this);
    wait_for = seats;
    for (int n = 0; n < seats_; ++n) {
        forks.emplace_back(std::make_shared<Fork>(n));
    }
    for (int n = 0; n < seats_; ++n) {
        int r = n;
        int l = (n+1) % seats_;
        philosophers.emplace_back(std::make_shared<Philosopher>(n, this, forks[r], forks[l]));
    }
}

void Table::updateCallback(int id) {
    std::lock_guard<std::mutex> lock(m_update_mtx);

    if (philosophers[id]->getStatus() == PhilStatus::Done) {
        wait_for--;
//        std::cout << "Philosopher " << id << " reported as done. wait_for = " << wait_for << std::endl;

    }

//    std::cout << "Status update from phil id=" << id << std::endl;
    m_cond_update.notify_one();

}

void Table::drawHeader() {
    std::stringstream ss;
    for (auto p: philosophers) {
        ss << "|  Phil " << p->getId() << "  ";
    }
    ss << std::endl << std::string(philosophers.size() * 12, '-') << std::endl;
    std::cout << ss.rdbuf();
}

void Table::drawStatus() {
    std::stringstream ss;
    for (auto p: philosophers) {
        ss << p->readStatus();
    }
    ss << std::endl;
    std::cout << ss.rdbuf();
}

void Table::joinPhilosopherToTheFeast(std::shared_ptr<Philosopher> p) {
    std::thread t = std::thread(&Philosopher::threadStartTheFeast, &(*p));
    p->setThread(t);
    std::cout << "Started " << p->getId() << std::endl;
}

void Table::Run() {
    for (auto p: philosophers) {
        joinPhilosopherToTheFeast(p);
    }
    std::cout << "Start monitoring Eating Philosophers" << std::endl;
    drawHeader();
    do {
        std::unique_lock<std::mutex> mlock(m_update_mtx);
        m_cond_update.wait(mlock);
        drawStatus();
    } while (wait_for);
}


int main() {
    Table table(15);
    table.Run();

    return 0;
}
