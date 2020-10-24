#include <thread>
#include <iostream>
#include <string>
using namespace std;

class Car {
    int production_year;
    string model_name;
    static int a;
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
    std::string corolla = "Corolla";
    std::thread t(&Car::setData, &toyota, 2015, "Corolla");
    // set year to 2015, model to "Corolla" in a thread
    t.join();
    toyota.print();
    return 0;
}
