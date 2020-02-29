#include <iostream>
#include <chrono>
#include <array>
#include <thread>
#include <numeric>

constexpr int reps = 1000000;

void stress_function(int a, int &b) {
    for(int i = 0; i < reps; i++) {
        b = a^b;
    }
}

int main() {
    std::array<int, 66> data;
    std::iota(data.begin(), data.end(), 1);

    std::cout << "========== False sharing ===========\n";
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1(stress_function, data[1], std::ref(data[2]));
    std::thread t2(stress_function, data[5], std::ref(data[6]));
    t1.join();
    t2.join();
    auto stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n';
    std::thread t3(stress_function, data[1], std::ref(data[2]));
    std::thread t4(stress_function, data[65], std::ref(data[66]));
    t3.join();
    t4.join();
    std::cout << "====== Fixed sharing ======\n";
    start = std::chrono::high_resolution_clock::now();


    stop = std::chrono::high_resolution_clock::now();

    duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n';

    std::cout << "======== Sequentional ========\n";
    start = std::chrono::high_resolution_clock::now();
    stress_function(data[1], std::ref(data[2]));
    stress_function(data[5], std::ref(data[6]));
    stop = std::chrono::high_resolution_clock::now();
    duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n';

/**********************************************************************************************
 *
 * Zjawisko false sharingu występuje poniewaz watki probuja dostac sie do tej samej komorki
 * pamieci mimo zeznajduja sie tam dwie niezalezne zmienne. Zeby uniknac tego zjawiska trzeba
 * sie upewnic ze dane na ktorych pracuja osobne watki znajduja sie w osobnych komorkach
 * pamieci. Ich rozsuwanie (padding można zrealizować poprzez wstawianie dodatkowcyh
 * sztucznych zmiennych albo za pomoca odpowiednich komend kompilatora (__declspec (align(64)))
 *
 **********************************************************************************************/

    return 0;
}
