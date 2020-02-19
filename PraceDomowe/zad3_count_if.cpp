#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>


constexpr size_t minSize = 10000;

template <typename It, typename Predicate, typename T>
T p_count_if(It first, It last, Predicate pred){

    const size_t dataSize = std::distance(first, last);
	if (dataSize < minSize){
		return std::count_if(first, last, pred);
    }

	const size_t hardwareThread = std::thread::hardware_concurrency();
	const size_t neededThreads = std::min(dataSize / minSize, hardwareThread);
	const size_t chunkSize = dataSize / neededThreads;
	std::vector<std::thread> threads(neededThreads - 1);
	std::vector<T> countedResults(neededThreads);


}

/* cppreference implementation

template<class InputIt, class UnaryPredicate>
typename iterator_traits<InputIt>::difference_type
    count_if(InputIt first, InputIt last, UnaryPredicate p)
{
    typename iterator_traits<InputIt>::difference_type ret = 0;
    for (; first != last; ++first) {
        if (p(*first)) {
            ret++;
        }
    }
    return ret;
}
*/

int main(){


    std::vector<int> v{ 1, 9, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10 };
 
    int counter = std::count_if(v.begin(), v.end(), [](int i){return i % 3 == 0;});
    std::cout << "Amout of counted numbers divided by 3: " << counter << '\n';

    return 0;
}