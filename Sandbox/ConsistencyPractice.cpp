/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <chrono>
#include <random>
#include <iostream>
#include <vector>

constexpr long long size = 100000000;

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
    
    for(auto n : randValues)
    {
        sum += n;
    }
        
    const std::chrono::duration<double> dur = std::chrono::steady_clock::now() - sta;
    
    std::cout << "Time for my Sumition " << dur.count() << " seconds " << std::endl;
    std::cout << "Result : " << sum << std::endl;
    
    std::cout << std::endl;
        
    
    return 0;
}

