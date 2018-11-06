/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

std::unique_ptr is a smart pointer that owns and manages another objet through a pointer and 
dispose of that object when the unique_ptr goes out of scope

- The object is disposed of using the associated deleter in either cases

    1. the managing unique_ptr object is destroyed
    2. the managing unique_ptr object is assigned another pointer via operator= or reset()

*/

#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>

struct B
{
    virtual void bar() { std::cout << "B::bar" << std::endl; }
    virtual void ~B() = default;
};

struct D : B
{
    D() { std::cout << "D::D" << std::endl; }
    ~D() { std::cout << "D::~D" << std::endl; }
    void bar() override { std::cout << "D::bar" << std::endl; }
};

std::unique_ptr<D> pass_through(std::unique_ptr<D> p)
{
    p->bar();
    return p;
}

void close_file(std::FILE* fp) { std::fclose(fp); }

int main()
{
    std::cout << "Unique Ownership Demo" << std::endl;
    {
        auto p = std::make_unique<D>();
        auto q = pass_through(std::move(p));
        assert(!p);
        q->bar();
    }
    
    std::cout << "Runtime Polymorphism Demo" << std::endl;
    {
        std::unique_ptr<B> p = std::make_unique<D>();
        
        p->bar();
        
        std::vecotr<std::unique_ptr<B>> v;
        v.push_back(std::make_unique<D>()));
        v.push_back(std::move(p));
        v.emplace_back(new D);
        for(auto& p : v) p->bar();
    }
    
    std::cout << "Custom Deleter Demo" << std::endl;
    std::ostream("Demo.txt") << 'X';
    {
        std::unique_ptr<std::FILE, decltype(&close_file)> fp(std::fopen("demo.txt", "r"), &close_file);
        
        if(fp)
        {
            std::cout << (char) std::fgetc(fp.get()) << std::endl;
        }
    }
    
    std::cout << "Custom lambda-expression deleter demo" << std::endl;
    {
        std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
        {
            std::cout << "destroying from a custom deleter ... " << std::endl;
            delete ptr;
        });
        
        p->bar();
    }
    
    std::cout << "Array form of unique_ptr demo" << std::endl;
    {
        std::unique_ptr<D[]> p{new D[3]};
    }
}
