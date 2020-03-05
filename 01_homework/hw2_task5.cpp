#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <thread>
#include <numeric>

constexpr int reps = 2000000;

template<typename T>
void stress_function(T a, T &b) {
    for(int i = 0; i < reps; i++) {
        b = a*b;
    }
}

struct Int64 {
    alignas(64) int value;

    Int64 operator*(const Int64& rhs) {
        value *= rhs.value;
        return *this;
    }
};

int main() {
    std::array<int, 66> data;
    std::iota(data.begin(), data.end(), 1);

    std::array<Int64,4> aligned_data;

    std::cout << "=========== False sharing ===========\n";
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1(stress_function<int>, data[0], std::ref(data[1]));
    std::thread t2(stress_function<int>, data[2], std::ref(data[3]));
    t1.join();
    t2.join();
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n';

    std::cout << "====== Fixed sharing - spacing ======\n";
    start = std::chrono::high_resolution_clock::now();
    std::thread t3(stress_function<int>, data[1], std::ref(data[2]));
    std::thread t4(stress_function<int>, data[65], std::ref(data[66]));
    t3.join();
    t4.join();
    stop = std::chrono::high_resolution_clock::now();
    duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n';

    std::cout << "====== Fixed sharing - alignas ======\n";
    start = std::chrono::high_resolution_clock::now();
    std::thread t5(stress_function<Int64>, aligned_data[0], std::ref(aligned_data[1]));
    std::thread t6(stress_function<Int64>, aligned_data[2], std::ref(aligned_data[3]));
    t5.join();
    t6.join();
    stop = std::chrono::high_resolution_clock::now();
    duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n';

    std::cout << "=========== Sequentional ============\n";
    start = std::chrono::high_resolution_clock::now();
    stress_function<int>(data[1], std::ref(data[2]));
    stress_function<int>(data[3], std::ref(data[4]));
    stop = std::chrono::high_resolution_clock::now();
    duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n';

/*****************************************************************************
 *
 * Zjawisko false sharingu występuje poniewaz watki probuja dostac sie do tej
 * samej linii cache'u mimo zeznajduja sie tam dwie niezalezne zmienne. Zeby
 * uniknac tego zjawiska trzeba sie upewnic ze dane na ktorych pracuja osobne
 * watki znajduja sie w osobnych komorkach pamieci. Ich rozsuwanie (padding
 * można zrealizować poprzez wstawianie dodatkowcyh sztucznych zmiennych albo
 * za pomoca odpowiednich komend kompilatora (__declspec (align(64)) lub funkcji alignas(64))
 *
 *****************************************************************************/
    return 0;
}
