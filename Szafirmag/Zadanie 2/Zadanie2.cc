#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <numeric>
#include <vector>

#define assertm(exp, msg) assert(((void)msg, exp))

using namespace std;

/*
    value based on my test for values from 0 to 1 000 000
    when splitting elements equally for 4 threads
*/
constexpr size_t minimumSize = 25000;

decltype(chrono::steady_clock::now()) start3;
decltype(start3) stop3;
decltype(start3) start4;
decltype(start4) stop4;

template <typename IT, typename T>
T pararelAccumulate(IT first, IT last, T init) {
	const size_t size = distance(first, last);
	if (size < minimumSize)
		return accumulate(first, last, init);

	const size_t hardwareThread = thread::hardware_concurrency();
	const size_t neededThreads = min(size / minimumSize, hardwareThread);
	const size_t chunkSize = size / neededThreads;
	vector<thread> threads(neededThreads - 1);
	vector<T> results(neededThreads);

	start3 = chrono::steady_clock::now();
	auto begin = first;
	for (size_t i = 0; i < neededThreads - 1; ++i) {
		auto end = next(begin, chunkSize);
		threads[i] = thread([](IT first, IT last, T& result)
			{
                result = accumulate(first, last, T{});
			}, begin, end, ref(results[i]));
		begin = end;
	}
	stop3 = chrono::steady_clock::now();

	start4 = chrono::steady_clock::now();
	results[neededThreads - 1] = accumulate(begin, last, T{});
	for_each(std::begin(threads), std::end(threads), mem_fn(&thread::join));
	stop4 = chrono::steady_clock::now();
	return accumulate(std::begin(results), std::end(results), init);
}

int main() {
	vector<int> vec(1000000);
	generate(begin(vec), end(vec), [x{ 0 }]()mutable{ return ++x; });
	auto start = chrono::steady_clock::now();
	auto pararelResult = pararelAccumulate(begin(vec), end(vec), 0);
	auto stop = chrono::steady_clock::now();

	auto start2 = chrono::steady_clock::now();
	auto normalResult = accumulate(begin(vec), end(vec), 0);
	auto stop2 = chrono::steady_clock::now();
	
	assertm(pararelResult == normalResult, "Error!Different values for normal and pararel algorithm");

	cout << "\nPararel algorithm: " 
		<< chrono::duration_cast<chrono::microseconds>(stop - start).count() 
		<< "us" << endl;
	cout << "Create threads: "
		<< chrono::duration_cast<chrono::microseconds>(stop3 - start3).count()
		<< "us" << endl;
	cout << "Wait for finish threads: "
		<< chrono::duration_cast<chrono::microseconds>(stop4 - start4).count()
		<< "us" << endl;
	cout << "\nNormal algorithm: "
		<< chrono::duration_cast<chrono::microseconds>(stop2 - start2).count()
		<< "us" << endl;

	return 0;
}
