#include "Fork.hpp"
#include "Philosopher.hpp"
#include "Table.hpp"


void diner(){
    std::stringstream dinerStart;
    dinerStart <<"--START OF DINING---" << std::endl;
    std::cout << dinerStart.rdbuf();

    Table table(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while(not table.stopEating()){
        table.showTable();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::stringstream dinerStop;
    dinerStop <<" ---END OF DININD---." << std::endl;
    std::cout << dinerStop.rdbuf();

}

int main(){
    diner();
    return 0;
}
