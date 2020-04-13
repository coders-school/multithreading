#include <chrono>
#include <iostream>
#include <thread>

struct foo {
    int x;
    int y; 
};

static struct foo f;

/* We are running function sum_a and inc_b
   concurrently. sum_a may need to continually 
   re-read x from main memory (instead of from cache) 
   even though inc_b's concurrent modification of y 
   should be irrelevant. 
   Source: https://en.wikipedia.org/wiki/False_sharing
*/

int sum_a()
{
    int s = 0;
    for (int i = 0; i < 1000000; ++i)
        s += f.x;
    return s;
}

void inc_b()
{
    for (int i = 0; i < 1000000; ++i)
        ++f.y;
}


int main()
{
    int number_of_repeats;
    std::cout <<"How many times run functions?:";
    std::cin >> number_of_repeats;
    int total_time = 0;
    for(int i=0;i < number_of_repeats; i++){
        auto start = std::chrono::steady_clock::now();
        std::thread t1(inc_b);
        std::thread t2(sum_a);
        t1.join();
        t2.join();
	    auto stop = std::chrono::steady_clock::now();
        int this_time = (int) std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	    //std::cout << "Time: " << this_time  << std::endl;
        total_time += this_time;
    }
    std::cout << "Total time: " << total_time << std::endl;
    return 0;
}
