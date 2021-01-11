#include <thread>
#include <iostream>
#include <string>
#include <random>
using namespace std;

std::random_device rd;

int main() {
    std::exception_ptr thread_exception = nullptr;
    std::string result;

    auto task = [](std::exception_ptr & te, std::string & result) {
        try {
            std::mt19937 gen(rd());
            std::bernoulli_distribution d(0.5);
            if (d(gen)) {
                throw std::runtime_error("WTF");
            } else {
                result = "success";
            }
        } catch (...) {
            te = std::current_exception();
        }
    };

    std::thread t(task, std::ref(thread_exception), std::ref(result));

    std::cout << "Some heave task on main thread\n";
    std::this_thread::sleep_for(1s);

    t.join();

    if (thread_exception) {
        try {
            std::rethrow_exception(thread_exception);
        } catch (const std::exception & ex) {
            std::cout << "Task exited with an exception: "
            << ex.what() << "\n";
        }
    } else {
        std::cout << "Task exited normally with result: " << result << '\n';
    }
    return 0;
}
