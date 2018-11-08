/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <atomic>
#include <chrono>
#include <random>
#include <iostream>
#include <vector>
#include <mutex>
#include <numeric>

constexpr long long size = 100000000;

enum class State : int
{
    Simple,
    Accumulate,
    Single_Lock,
};

int main()
{
    std::vector<int> randValues;

    randValues.reserve(size);

    std::random_device seed;

    std::mt19937 engine(seed());

    std::uniform_int_distribution<> uniformDist(1, 10);

    for(long long i=0; i<size; i++)
    {
        randValues.push_back(uniformDist(engine));
    }

    const auto sta = std::chrono::steady_clock::now();

    unsigned long long sum = {};

    State state = State::Accumulate;

    switch(state)
    {
        case State::Simple:
        {
            for(auto n : randValues)
            {
                sum += n;
            }

            break;
        }
        case State::Accumulate:
        {
            sum = std::accumulate(randValues.begin(), randValues.end(), 0);
            break;
        }
        case State::Single_Lock:
        {
            std::mutex myMutex;

            for(auto i : randValues)
            {
                std::lock_guard<std::mutex> myLockGuard(myMutex);
                sum += i;
            }

            break;
        }
    }

    const std::chrono::duration<double> dur = std::chrono::steady_clock::now() - sta;

    std::cout << "Time for my Sumition " << dur.count() << " seconds " << std::endl;
    std::cout << "Result : " << sum << std::endl;
    std::cout << std::endl;

    return 0;
}
