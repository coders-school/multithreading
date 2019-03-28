#include <thread>
#include <iostream>
#include <string>
using namespace std;

class Car {
    int production_year;
    string model_name;
public:
    void setData(int year, const string & model) {
        production_year = year;
        model_name = model;
    }
    void print() {
        cout << model_name << " " << production_year << endl; 
    }
};

int main() {
    Car toyota;
    // set year to 2015, model to "Corolla" in a thread
    toyota.print();
    return 0;
}

