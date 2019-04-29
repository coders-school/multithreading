#include <thread>
#include <chrono>
#include <iostream>

struct foo {
    int x;
	int y; 
};

static struct foo f;

int sum_a()
{
	int s = 0;
	for (int i = 0; i < 100'000'000; ++i)
		s += f.x;
	return s;
}

void inc_b()
{
	for (int i = 0; i < 100'000'000; ++i)
		++f.y;
}

int main () {
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1(sum_a);
    std::thread t2(inc_b);
    t1.join();
    t2.join();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Threads: " << duration.count() << '\n';

    start = std::chrono::high_resolution_clock::now();
    sum_a();
    inc_b();
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Sequential: " << duration.count() << '\n';

    return 0;
}
