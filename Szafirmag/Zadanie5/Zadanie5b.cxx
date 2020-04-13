#include <chrono>
#include <iostream>
#include <thread>

struct foo {
    int x;
    int y; 
};

static struct foo f;

/* Here we have fixed problem of false
   sharing by having functions creating
   local copies of variables they are
   working with.
*/

int sum_a()
{
    int s = 0;
    int local_x = f.x;
    for (int i = 0; i < 1000000; ++i)
        s += local_x;
    return s;
}

void inc_b()
{
    int local_y = f.y;
    for (int i = 0; i < 1000000; ++i)
        ++local_y;
    f.y = local_y;
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
