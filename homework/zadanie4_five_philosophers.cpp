#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <string>
#include <random>

using namespace std::chrono_literals;


class CFork{

public:
    mutable std::mutex mtx_fork_;
    
    CFork(){
    }

    CFork(CFork&&){
    }
};

class CPhilosoph{

    std::string name_;
    std::string output_;
    CFork& left_fork_;
    CFork& right_fork_;
    int time=0;
    std::random_device rd;
    int sleep_time_;

public:

    CPhilosoph(std::string name, CFork& left_fork, CFork& right_fork) : name_(name), left_fork_(left_fork), right_fork_(right_fork){
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 3);
        sleep_time_ = dis(gen);
    }

    ~CPhilosoph(){
    }

    void eat(){
        std::scoped_lock sl(left_fork_.mtx_fork_, right_fork_.mtx_fork_);
        output_ = "Philosoph: " + name_ + " is eating...\n";
        std::cout<<output_;
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time_));
    }

    void think(){
        output_ = "Philosoph: " + name_ + " is thinking...\n";
        std::cout<<output_;
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time_));
    }

    void party(){
        while (time < 3){
            time++;
            think();
            eat();
        }
    }
};
 
class CTable{

    int numbers_of_forks_;
    std::vector<std::thread> thread_pool_;
    std::vector<CPhilosoph*> philo_pointers_;
    std::vector<CFork> forks_pool_;

public:
    CTable(int number_of_forks) : numbers_of_forks_(number_of_forks){ 
        for (int i=0; i < numbers_of_forks_; i++){
            forks_pool_.emplace_back(CFork());
        }
    }

    ~CTable(){
        for (auto&& th : thread_pool_){
            th.join();
        }
        for (int i = 0; i < numbers_of_forks_; i++){
            delete philo_pointers_[i];
        }
        std::cout<< "\nParty is ended. Go home ;)\n\n";
    }

    void Start_party(){
        std::cout<< "\nLet's party started!!!\n\n";
        for (size_t i=0; i < numbers_of_forks_; i++){
            if (i == 0){
                philo_pointers_.emplace_back(new CPhilosoph("Philo" + std::to_string(i), forks_pool_[i], forks_pool_[numbers_of_forks_-1]));
                thread_pool_.emplace_back(std::thread(&CPhilosoph::party, philo_pointers_[i]));
            }
            else{
                philo_pointers_.emplace_back(new CPhilosoph("Philo" + std::to_string(i), forks_pool_[i-1], forks_pool_[i]));
                thread_pool_.emplace_back(std::thread(&CPhilosoph::party, philo_pointers_[i]));
            }
        }
    }
};

int main(int argc, char* argv[]){

    const int numbers_of_philosophers = 3;
    CTable table(numbers_of_philosophers);
    table.Start_party();

    return 0;
}

