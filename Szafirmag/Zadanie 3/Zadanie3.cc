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

constexpr size_t minimumSize = 25000;

decltype(chrono::steady_clock::now()) start3;
decltype(start3) stop3;
decltype(start3) start4;
decltype(start4) stop4;

template <typename IT, typename V>
int pararelCountIF(IT first, IT last, V value) {
	const size_t size = distance(first, last);
	if (size < minimumSize)
		return count_if(first,last,value);

	const size_t hardwareThread = thread::hardware_concurrency();
	const size_t neededThreads = min(size / minimumSize, hardwareThread);
	const size_t chunkSize = size / neededThreads;
	vector<thread> threads(neededThreads - 1);
	vector<int> results(neededThreads);

	start3 = chrono::steady_clock::now();
	auto begin = first;
	for (size_t i = 0; i < neededThreads - 1; ++i) {
		auto end = next(begin, chunkSize);
		threads[i] = thread([](IT first, IT last,V value, int& result)
			{
                result = count_if(first, last, value);
			}, begin, end,value, ref(results[i]));
		begin = end;
	}
	stop3 = chrono::steady_clock::now();

	start4 = chrono::steady_clock::now();
	results[neededThreads - 1] = count_if(begin, last, value);
	for_each(std::begin(threads), std::end(threads), mem_fn(&thread::join));
	stop4 = chrono::steady_clock::now();
	return accumulate(std::begin(results), std::end(results), 0);
}

int main() {
	vector<int> vec(1'000'000);
	auto predicate = [](int i){return i % 200 == 0;};
	generate(begin(vec), end(vec), [x{ 0 }]()mutable{ return ++x; });
	auto start = chrono::steady_clock::now();
	auto pararelResult = pararelCountIF(begin(vec), end(vec), predicate);
	auto stop = chrono::steady_clock::now();

	auto start2 = chrono::steady_clock::now();
	auto normalResult = count_if(begin(vec), end(vec), predicate);
	auto stop2 = chrono::steady_clock::now();
	
	/*cout << pararelResult << endl;
	cout << normalResult << endl;*/

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
