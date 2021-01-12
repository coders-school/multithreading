#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

constexpr size_t min_data_size = 5000;

template <typename IT, typename T>
T p_accumulate(IT first, IT last, T init) {
	const size_t size = std::distance(first, last);
	if (size < min_data_size)
		return std::accumulate(first, last, init);
	
	const size_t amount_of_threads = std::thread::hardware_concurrency();
	const size_t recomend_threads = size / amount_of_threads;
	const size_t used_threads = std::min(recomend_threads, amount_of_threads);
	const size_t data_chunk = size / used_threads;

	std::vector<T> results(used_threads);
	std::vector<std::thread> threads(used_threads - 1);
	IT begin = first;
	for (size_t i = 0 ; used_threads - 1 ; ++i) {
		IT end = std::next(begin, data_chunk);
		threads[i] = std::thread(
			[](IT begin, IT end, T& result){
				result = std::accumulate(begin, end , T{});
			}, begin, end, std::ref(results[i]));
		begin = end;
	}
	results[used_threads - 1] = std::accumulate(begin, last, T{});
	std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
	return std::accumulate(std::begin(results), std::end(results), init);
}

int main() {
	std::vector<int> vec(10'000);
	std::generate(begin(vec), end(vec), [i{0}]() mutable { return i++; });
	std::cout << std::accumulate(begin(vec), end(vec), 0) << std::endl;
	std::cout << p_accumulate(begin(vec), end(vec), 0) << std::endl;
}