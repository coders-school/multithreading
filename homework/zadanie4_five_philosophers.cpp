#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <string>
#include <mutex>

using namespace std::chrono_literals;

std::mutex mtx;

class CTable{

public:
    std::vector<bool> forks_pool;

    CTable(int number_of_forks){
        for (size_t i=0; i< number_of_forks; i++){
            forks_pool.push_back(false);
        }
    }
};

class CPhilosoph{

public:
    std::string name_;
    std::string output;
    int number_;
    CTable table_;
    int left_fork_;
    int right_fork_;

    std::chrono::high_resolution_clock::time_point start_of_live_, end_of_live_;
    size_t time=0;

    CPhilosoph(const CTable& table, std::string name, int number, int left_fork, int right_fork) : table_(table), name_(name), number_(number), left_fork_(left_fork), right_fork_(right_fork){
        start_of_live_ = std::chrono::high_resolution_clock::now();
    }

    ~CPhilosoph(){
        delete this;
    }

    void eat(){
        if (table_.forks_pool[left_fork_] == false or table_.forks_pool[right_fork_]){
            std::lock_guard lg(mtx);
            table_.forks_pool[left_fork_] = true;
            table_.forks_pool[right_fork_] = true;
            output = "Philosoph: " + name_ + " is eating...\n";
            std::cout<<output;
            std::this_thread::sleep_for(2s); 
            table_.forks_pool[left_fork_] =false;
            table_.forks_pool[right_fork_] = false;  
        }
    }

    void think(){
        output = "Philosoph: " + name_ + " is thinking...\n";
        std::cout<<output;
        std::this_thread::sleep_for(3s);
    }

    void party(){
        end_of_live_ = std::chrono::high_resolution_clock::now();
        auto time_of_live_ = std::chrono::duration_cast<std::chrono::milliseconds> (start_of_live_ - end_of_live_).count();
        while (time < 3){
            time++;
            think();
            eat();
        }
    }

};


int main(int argc, char* argv[]){

    int numbers_of_philosophers = 5;
    CTable table(numbers_of_philosophers);
    std::vector<std::thread> thread_pool(numbers_of_philosophers);
    std::vector<std::string> names_of_philosophers(numbers_of_philosophers);

    names_of_philosophers[0]="Arystoteles";
    names_of_philosophers[1]="Platon";
    names_of_philosophers[2]="Sokrates";
    names_of_philosophers[3]="Tales";
    names_of_philosophers[4]="Nietzsche";

    for (size_t i=0; i < numbers_of_philosophers; i++){
        if (i == 0){
            thread_pool[i] = std::thread(&CPhilosoph::party, new CPhilosoph(table, names_of_philosophers[i], i+i, numbers_of_philosophers, i));
        }
        else{
            thread_pool[i] = std::thread(&CPhilosoph::party, new CPhilosoph(table, names_of_philosophers[i], i+1, i-1, i));
        }
    }
    for (auto&& th : thread_pool){
        th.join();
    }
    
    return 0;
}
