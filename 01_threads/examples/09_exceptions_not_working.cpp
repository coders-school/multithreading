#include <thread>
#include <iostream>

int main() {
    try {
        std::thread t1([]{
        	throw std::runtime_error("WTF - What a Terrible Failure"); 
        });
        t1.join();
    } catch (const std::exception & ex) {
        std::cout << "Thread exited with exception: " << ex.what() << "\n";
    }
    return 0;
}
