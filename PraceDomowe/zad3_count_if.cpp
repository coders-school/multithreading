#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

constexpr size_t minSize = 10000;

template <typename It, typename Predicate>
typename std::iterator_traits<It>::difference_type p_count_if(It first, It last, Predicate pred){

    if(std::distance(first, last) == 0)
        return 0;
    const size_t dataSize = std::distance(first, last);
	if (dataSize < minSize){
		return std::count_if(first, last, pred);
    }

	const size_t hardwareThread = std::thread::hardware_concurrency();
	const size_t neededThreads = std::min(dataSize / minSize, hardwareThread);
	const size_t chunkSize = dataSize / neededThreads;
	std::vector<std::thread> threads(neededThreads - 1);
    std::vector<int> results(neededThreads);

    auto threadBegin = first;
   
    for (size_t i = 0; i < neededThreads - 1; ++i) {
            auto threadEnd = std::next(threadBegin, chunkSize);
            threads[i] = std::thread([](It firstIterator, It lastIterator, Predicate threadPred, int& counter)
                {
                    counter = std::count_if(firstIterator, lastIterator, threadPred);
                }, threadBegin, threadEnd, pred, std::ref(results[i]));
            threadBegin = threadEnd;
        }

    results[neededThreads - 1] = std::count_if(threadBegin, last, pred);
    std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));

    int counter = 0;
    for(auto element : results)
        counter += element;

    return counter;
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

    std::vector<int> emptyVector;
    auto emptyResult = p_count_if(emptyVector.begin(), emptyVector.end(), [](int i){return i % 3 == 0;});
    std::cout << "Result of p_count_if for empty vector: " << emptyResult << '\n';

    std::vector<int> testVector(1000000);
    std::iota(testVector.begin(), testVector.end(), 0);

    auto startp_count_if = std::chrono::steady_clock::now();
    p_count_if(testVector.begin(), testVector.end(), [](int i){return i % 3 == 0;});
    auto stopp_count_if = std::chrono::steady_clock::now();

    auto startSTL_count_if = std::chrono::steady_clock::now();
    std::count_if(testVector.begin(), testVector.end(), [](int i){return i % 3 == 0;});
    auto stopSTL_count_if = std::chrono::steady_clock::now();

    std::cout << "Pararel algorithm lasted: "
            << std::chrono::duration_cast<std::chrono::microseconds>(stopp_count_if - startp_count_if).count()
            << "us\n";

    std::cout << "STL algorithm lasted: "
            << std::chrono::duration_cast<std::chrono::microseconds>(stopSTL_count_if - startSTL_count_if).count()
            << "us\n";

    return 0;
}