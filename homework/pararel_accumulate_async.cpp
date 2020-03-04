#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <future>
#include <vector>
#include <chrono>
#include <functional>

constexpr size_t minimumSize = 200;

struct debugTimers
{
	decltype(std::chrono::steady_clock::now()) start3;
	decltype(start3) stop3;
	decltype(start3) start4;
	decltype(start4) stop4;
};

debugTimers dummy_dt;

template <typename IT, typename T>
T pararelAccumulate(IT first, IT last, T init, size_t minSizeForThread = minimumSize, debugTimers& dt = dummy_dt ) {
	const size_t size = std::distance(first, last);
	if (size < minSizeForThread)
		return std::accumulate(first, last, init);

	size_t hardwareThread = std::thread::hardware_concurrency();

    if (hardwareThread == 0)
        hardwareThread = 1;

	const size_t neededThreads = std::min(size / minSizeForThread, hardwareThread);
	const size_t chunkSize = size / neededThreads;
	std::vector<T> results(neededThreads);
    std::vector<std::future<T>> futures(neededThreads);

    auto sumFunction = [](IT first, IT last)
    {
        return(std::accumulate(first, last, T{}));
    };

	dt.start3 = std::chrono::steady_clock::now();
	auto begin = first;
	for (size_t i = 0; i < neededThreads; ++i) {
		auto end = std::next(begin, chunkSize);

        futures[i] = std::async(sumFunction, begin, end);
		begin = end;
	}
	dt.stop3 = std::chrono::steady_clock::now();

	dt.start4 = std::chrono::steady_clock::now();
    for( int i = 0; i < neededThreads; ++i )
    {
        results[i] = futures[i].get();
    } 

	dt.stop4 = std::chrono::steady_clock::now();
	return std::accumulate(std::begin(results), std::end(results), init);
}

void pararelAccumulate_test(int numberOfElements, int minSizeForThread)
{
	std::cout << std::endl;
	std::cout << "Number of elements in vector: " 
		<< numberOfElements << std::endl;
	std::cout << "Minimum elements for one thread: " 
		<< minSizeForThread << std::endl;
    std::cout << "Number of hardware threads: "
        << std::thread::hardware_concurrency() << std::endl;

	std::vector<int> vec(numberOfElements);
	debugTimers dt;

	std::generate(begin(vec), end(vec), [x{ 0 }]()mutable{ return ++x; });
	auto start = std::chrono::steady_clock::now();
	pararelAccumulate(std::begin(vec), std::end(vec), 0, minSizeForThread, dt);
	auto stop = std::chrono::steady_clock::now();

	auto start2 = std::chrono::steady_clock::now();
	std::accumulate(std::begin(vec), std::end(vec), 0);
	auto stop2 = std::chrono::steady_clock::now();
	
	std::cout << "\nCreate threads: "
		<< std::chrono::duration_cast<std::chrono::microseconds>(dt.stop3 - dt.start3).count()
		<< "us" << std::endl;
	std::cout << "Wait for finish threads: "
		<< std::chrono::duration_cast<std::chrono::microseconds>(dt.stop4 - dt.start4).count()
		<< "us" << std::endl;

	std::cout << "\nPararel algorithm: " 
		<< std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() 
		<< "us" << std::endl;
	std::cout << "\nNormal algorithm: "
		<< std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() 
		<< "us" << std::endl;

	std::cout << "\n ----- end test ----" << std::endl;
}

int main() {

	pararelAccumulate_test(1'000, 200);
	pararelAccumulate_test(1'000'000, 200);
    pararelAccumulate_test(1'000'000, 256);

	pararelAccumulate_test(1'000, 500);
	pararelAccumulate_test(1'000'000, 500);
    pararelAccumulate_test(1'000'000, 512);
    pararelAccumulate_test(1'000'000, 2000);
    pararelAccumulate_test(1'000'000, 2048);

	return 0;
}

