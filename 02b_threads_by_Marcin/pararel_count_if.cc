#include <bits/stdc++.h>
#include <iostream>
#include <thread>
#include <iterator>

// #include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
// #include <functional>

#include <bits/stdc++.h> 

constexpr size_t minimumSize = 200;

struct debugTimers
{
	decltype(std::chrono::steady_clock::now()) start3;
	decltype(start3) stop3;
	decltype(start3) start4;
	decltype(start4) stop4;
};

// Check is numbe is even
bool isEven(int i) 
{ 
    return (i % 2 == 0) ? true : false;
} 

debugTimers dummy_dt;

template <typename IT, typename T>
typename std::iterator_traits<IT>::difference_type
pararelCountIf(IT first, IT last, T fun, size_t minSizeForThread = minimumSize, debugTimers& dt = dummy_dt ) {
	const size_t size = std::distance(first, last);
	if (size < minSizeForThread)
		return std::count_if(first, last, fun);

	size_t hardwareThread = std::thread::hardware_concurrency();

    if (hardwareThread == 0)
        hardwareThread = 1;

	const size_t neededThreads = std::min(size / minSizeForThread, hardwareThread);
	const size_t chunkSize = size / neededThreads;
	
    typedef typename std::iterator_traits<IT>::difference_type result_type;
    
    std::vector<std::thread> threads(neededThreads - 1);
	std::vector<result_type> results(neededThreads);

	dt.start3 = std::chrono::steady_clock::now();
	auto begin = first;
	for (size_t i = 0; i < neededThreads - 1; ++i) {
		auto end = std::next(begin, chunkSize);
		threads[i] = std::thread([](IT first, IT last, T fun, result_type& result)
			{
				result = std::count_if(first, last, fun);
			}, begin, end, fun, std::ref(results[i]));
		begin = end;
	}
	dt.stop3 = std::chrono::steady_clock::now();

	dt.start4 = std::chrono::steady_clock::now();
	results[neededThreads - 1] = std::count_if(begin, last, fun);
	std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
	dt.stop4 = std::chrono::steady_clock::now();
	return std::accumulate(std::begin(results), std::end(results), 0);
}

void pararelCountIf_test(int numberOfElements, int minSizeForThread)
{
	std::cout << std::endl;
	std::cout << "Number of elements in vector: " 
		<< numberOfElements << std::endl;
	std::cout << "Minimum elements for one thread: " 
		<< minSizeForThread << std::endl;	

	std::vector<int> vec(numberOfElements);
    int res1;
    int res2; 
	debugTimers dt;

	std::generate(begin(vec), end(vec), [x{ 0 }]()mutable{ return ++x; });
	auto start = std::chrono::steady_clock::now();
	res1 = pararelCountIf(std::begin(vec), std::end(vec), isEven, minSizeForThread, dt);
	auto stop = std::chrono::steady_clock::now();

	auto start2 = std::chrono::steady_clock::now();
	res2 = std::count_if(std::begin(vec), std::end(vec), isEven);
	auto stop2 = std::chrono::steady_clock::now();
	
    std::cout << "res1: " << res1 << std::endl;
    std::cout << "res2: " << res2 << std::endl;

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

	pararelCountIf_test(1'000, 200);
	pararelCountIf_test(1'000'000, 200);

	pararelCountIf_test(1'000, 5000);
	pararelCountIf_test(1'000'000, 5000);

	return 0;
}
