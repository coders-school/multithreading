#include "Table.hpp"
#include <thread>

class Philosopher {
    Table const &     table_;
    Fork&             fork_left_;
    Fork&             fork_right_;
    std::thread       thread_;

public:
    Philosopher(Table const &table, Fork& f_left, Fork& f_right, std::string name) noexcept;
    ~Philosopher();

    void dine();
};
