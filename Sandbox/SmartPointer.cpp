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

/*
make_unique

template <typename T, typename ... Args>
unique_ptr<T> make_unique(Args&& ... args);

template <typename T>
unique_ptr<T> make_unique(std::size_t size); 

Constructs an object of type T and wraps it in a std::unique_ptr

1. Constructs a non-array type T : unique_ptr<T>(new T(std::forward<Args>(args)...))

2. Constructs an array of unknown bound T : unique_ptr<T>(new typename std::remove_extend<T>::type[size]())

3. Construction of arrays of know bound is disallowed
*/

#include <iostream>
#include <memory>

struct Vec3
{
    int x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(int x, int y, int z) : x(x), y(y), z(z) {}
    friend std::ostream& operator<<(std::ostream& os, Vec3& v)
    {
        return os << "{" << "X : " << v.x << " Y : " << v.y << " Z : " << v.z "}" << std::endl;
    }
}

int main()
{
    std::unique_ptr<Vec3> v1 = std::make_unique<Vec3>();
    std::unique_ptr<Vec3> v2 = std::make_unique<Vec3>(3, 3, 3);
    std::unique_ptr<Vec3[]> v3 = std::make_unique<Vec3[]>(5);
    
    std::cout << "make_unique<Vec3>():      " << *v1 << '\n'
              << "make_unique<Vec3>(0,1,2): " << *v2 << '\n'
              << "make_unique<Vec3[]>(5):   " << '\n';
              
    for (int i = 0; i < 5; i++) {
        std::cout << "     " << v3[i] << '\n';
}

/******************************************************************************
std::shared_ptr

#include <memory>

template <typename T> class shared_ptr;

std::shared_ptr is a smart pointer that retains shared ownership of an object through a pointer

Several shared_ptr objects may own the same object

The object is destroyed and its memory deallocated when either of the following happens

1. the last reamining shared)ptr owning the object is destroyed

2. the last remaining shared_ptr owning the object is assigned another pointer via operator= or reset()

********************************************************************************************

std::allocate_shared

template <typename T, typename Alloc, typename ... Args>
std::shared_ptr<T> allocate_shared<const Alloc& alloc, Args&& ... args);

template <typename T, typename Arr>
std::shared_ptr<T> allocate_shared(const Arr& arr, std::size_t N);

template <typename T, typename Arr>
std::shared_ptr<T> allocate_shared(const Arr& arr);

template <typename T, typename Arr>
std::shared_ptr<T> allocate_shared(const Arr& arr, std::size_t N, const std::remove_extent_t<T>& u);

template <typename T, typename Arr>
std::shared_ptr<T> allocate_shared(const Arr& arr, const std::remove_extent_t<T>& u);

Constructs an object of type T and wraps it in a std::shared_ptr using args as the parameter list for the constructor of T.

*/

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

struct Base
{
    Base() { std::cout << "Base::Base()" << std::endl; }
    ~Base() { std::cout << "Base::~Base()" << std::endl; }
};

struct Derived : public Base
{
    Derived() { std::cout << "Derived::Derived()" << std::endl; }
    ~Derived() { std::cout << "Derived::~Derived()" << std::endl; }
};

struct Third : public Base, public std::enable_shared_from_this<Third> 
{
};

void thr(std::shared_ptr<Base> p)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::shared_ptr<Base> lp = p;
    
    {
        static std::mutex io_mutex;
        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << "Local pointer in a thread : " << std::endl;
        std::cout << " lp.get() = " << lp.get() 
                  << " lp.use_count() = " << lp.use_count() << std::endl;
    }
}

int main()
{
    auto del = [](Base* InBase)
    {
        std::cout << "Q is removed" << std::endl;
        delete InBase; 
    };
    
    std::shared_ptr<Base> p = std::make_shared<Derived>();
    
    {
        std::shared_ptr<Base> q (new Base, del);    
    }
    
    std::cout << "Created a shared Derived (as a pointer to Base)\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';
    std::thread t1(thr, p), t2(thr, p), t3(thr, p);
    p.reset(); // release ownership from main
    std::cout << "Shared ownership between 3 threads and released\n"
              << "ownership from main:\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';
    t1.join(); t2.join(); t3.join();
    std::cout << "All threads completed, the last one deleted Derived\n";   
}

