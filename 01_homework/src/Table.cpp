#include "Table.hpp"
#include <sstream>

//Table::Table();
Table::~Table();

Table::print() {
    stringstream buff;
    for(int i = 0; i < population; i++) {
        // buff << table status;

    }
    //buff << first fork as it is shared between first and last philosopher
    //buff << return carrige

    cout << buff;
}
