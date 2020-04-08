#include <thread>
#include <iostream>
#include <ctime>
#include <memory>
#include <condition_variable>
#include <mutex>
 
using std::cout;
using std::endl;
 
const double M=8e8;
 
//Shared_Work holds the work need to be done
//part1() and part2() separates the work half-and-half
//n1 and n2 are data items
//chunk[64] is the separation attempt.
//we can also separate date with alignas() 
class Shared_Work {
public:
    Shared_Work():n1(1),n2(1),flag1(0),flag2(0) {}
 
    void part1() {
        for(; n1<M; n1+=n1%3);
        flag1=1;
        cond.notify_one();
    }
 
    void part2() {
        for(; n2<M; n2+=n2%3);
        flag2=1;
        cond.notify_one();
    }
 
    long int result() {
        std::unique_lock<std::mutex> lk(mt);
        while(flag1==0||flag2==0)
            cond.wait(lk);
        return n1+n2;
    }
 
private:
    std::mutex mt;
    std::condition_variable cond;
    alignas(64) long int n1;
    // char chuck[64];//cache line seperation
    alignas(64) long int n2;
    bool flag1;
    bool flag2;
};
 
//a template class for generating threads with different member functions
//"action _fptr" will be instantiated by &Shared_Work::part1 and part2.
template<class T>
class Do_Work {
public:
    typedef void (T::*action)();
    Do_Work(std::shared_ptr<Shared_Work>& x, action y):_p(x),_fptr(y) {}
    void operator()() {
        (*_p.*_fptr)();
    }
private:
    std::shared_ptr<Shared_Work> _p;
    action _fptr;
};
 
//single thread work to do
void one_thread() {
    long int n1=1, n2=1;
    for(; n1<M; n1+=n1%3);
    for(; n2<M; n2+=n2%3);
    cout<<n1+n2<<endl;
}
 
 
 
time_t start, end;
double diff;
 
int main(int argc, char* argv[]) {
 
    std::shared_ptr<Shared_Work> p(new Shared_Work);
    Do_Work<Shared_Work> d1(p, &Shared_Work::part1);
    Do_Work<Shared_Work> d2(p, &Shared_Work::part2);
 
    time(&start);
    std::thread t1( d1 );
    std::thread t2( d2 );
    t1.detach();//release the ownership to C++ runtime library.
    t2.detach();//release the ownership to C++ runtime library.
    cout<<p->result()<<endl;
    time(&end);
    diff=difftime(end, start);
    cout<<diff<<" seconds elapsed for 2 threads calculation."<<endl;
 
    time(&start);
    one_thread();
    time(&end);
    diff=difftime(end, start);
    cout<<diff<<" seconds elapsed for 1 thread calculation."<<endl;
 
}