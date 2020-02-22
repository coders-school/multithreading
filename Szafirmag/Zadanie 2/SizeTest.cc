#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
#include <functional>

#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

template <typename IT, typename T>
T pararelAccumulate(IT first, IT last, T init) {
	const size_t size = std::distance(first, last);
	const size_t hardwareThread = std::thread::hardware_concurrency();
	const size_t chunkSize = size / hardwareThread;
	std::vector<std::thread> threads(hardwareThread - 1);
	std::vector<T> results(hardwareThread);

	auto begin = first;
	for (size_t i = 0; i < hardwareThread  - 1; ++i) {
		auto end = std::next(begin, chunkSize);
		threads[i] = std::thread([](IT first, IT last, T& result)
			{
				result = std::accumulate(first, last, T{});
			}, begin, end, std::ref(results[i]));
		begin = end;
	}

	results[hardwareThread - 1] = std::accumulate(begin, last, T{});
	std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));

	return std::accumulate(std::begin(results), std::end(results), init);
}

void TestForSize(size_t sizeTest){
	std::vector<int> vec(sizeTest);
	std::generate(begin(vec), end(vec), [x{ 0 }]()mutable{ return ++x; });
	auto start = std::chrono::steady_clock::now();
	auto pararelResult = pararelAccumulate(std::begin(vec), std::end(vec), 0);
	auto stop = std::chrono::steady_clock::now();

	auto start2 = std::chrono::steady_clock::now();
	auto normalResult =std::accumulate(std::begin(vec), std::end(vec), 0);
	auto stop2 = std::chrono::steady_clock::now();
	
	assertm(pararelResult == normalResult, "Error!Different values for normal and pararel algorithm");

	auto pararelTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	auto normalTime = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count();
    	std::cout << sizeTest << " elemenents Pararel time: " << pararelTime  << " Normal time: " << normalTime ; 
	if(pararelTime < normalTime){
        std::cout << " Pararel rules :)" << std::endl;
	} else {
        std::cout << " Pararel sucks :(" << std::endl;
    }

}

int main() {
    for(int i=0;i <= 1'000'000; i+=100){
        TestForSize(i);
    }
	return 0;
}
