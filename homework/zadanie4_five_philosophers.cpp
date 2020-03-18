#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <string>
#include <mutex>

using namespace std::chrono_literals;


class CFork{
public:
    mutable std::mutex mtx_fork_;
};

 
class CTable{

    int numbers_of_forks_;

public:
    std::vector<CFork*> forks_pool_;

    CTable(int number_of_forks) : numbers_of_forks_(number_of_forks){
        for (size_t i=0; i < numbers_of_forks_; i++){
            forks_pool_.push_back(new CFork());
        }
    }

    ~CTable(){
        for (int i = 0; i < numbers_of_forks_; i++){
            delete forks_pool_[i];
        }
    }
};

class CPhilosoph{

    std::string name_;
    std::string output_;
    CTable& table_;
    CFork* left_fork_;
    CFork* right_fork_;
    int time=0;

public:
    CPhilosoph( CTable& table, std::string name, CFork* left_fork, CFork* right_fork) : table_(table), name_(name), left_fork_(left_fork), right_fork_(right_fork){
    }

    ~CPhilosoph(){
    }

    void eat(){
        std::scoped_lock sl(left_fork_->mtx_fork_, right_fork_->mtx_fork_);
        output_ = "Philosoph: " + name_ + " is eating...\n";
        std::cout<<output_;
        std::this_thread::sleep_for(2s); 
    }

    void think(){
        output_ = "Philosoph: " + name_ + " is thinking...\n";
        std::cout<<output_;
        std::this_thread::sleep_for(3s);
    }

    void party(){
        while (time < 3){
            time++;
            think();
            eat();
        }
    }
};


int main(int argc, char* argv[]){

    const int numbers_of_philosophers = 3;
    CTable table(numbers_of_philosophers);
    std::vector<std::thread> thread_pool(numbers_of_philosophers);
    std::vector<std::string> names_of_philosophers(numbers_of_philosophers);
    std::vector<CPhilosoph*> philo_pointers;

    for (int i=0; i< numbers_of_philosophers; i++){
        names_of_philosophers[i] = "Philo" + std::to_string(i);
    }

    for (size_t i=0; i < numbers_of_philosophers; i++){
        if (i == 0){
            philo_pointers.push_back(new CPhilosoph(table, names_of_philosophers[i], table.forks_pool_[i], table.forks_pool_[numbers_of_philosophers-1]));
            thread_pool[i] = std::thread(&CPhilosoph::party, philo_pointers[i]);
        }
        else{
            philo_pointers.push_back(new CPhilosoph(table, names_of_philosophers[i], table.forks_pool_[i-1], table.forks_pool_[i]));
            thread_pool[i] = std::thread(&CPhilosoph::party, philo_pointers[i]);
        }
    }
    for (auto&& th : thread_pool){
        th.join();
    }

    for (int i=0; i < numbers_of_philosophers; i++){
        delete philo_pointers[i];
    }

    return 0;
}

