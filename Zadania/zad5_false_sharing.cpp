#include <array>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <thread>

struct falseSharingData
{
    int firstValue, secondValue;

    void operator++ ()
    {
        ++firstValue;
        ++secondValue;
    }

    falseSharingData& operator= (int newValue)
    {
        firstValue += newValue;
        secondValue += newValue;

        return *this;
    }
};

class falseSharingContainer
{
public:

    falseSharingContainer()
    {
        std::iota(falseArray.begin(), falseArray.end(), 0);
    }

    void alternatingAddition(int i)
    {
        for(i; i < falseArray.size(); i += 4)
            ++falseArray[i];
    }

private:

    std::array<falseSharingData, 1000000> falseArray;
};

int main()
{
    falseSharingContainer testObject;

    auto start = std::chrono::steady_clock::now();
    std::thread t1(&falseSharingContainer::alternatingAddition, &testObject, 0);
    std::thread t2(&falseSharingContainer::alternatingAddition, &testObject, 1);

    t1.join();
    t2.join();
    auto stop = std::chrono::steady_clock::now();

    std::cout << "Program with false sharing: "
            << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
            << "us" << '\n';

    return 0;
}