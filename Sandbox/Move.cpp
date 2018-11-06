// Move Constructor
// 
// A Move Constructor of class T is a non-template constructor whose first parameter is T&&, const T&&, volatile T&& or const volatile T&&
//
// initialization : T a = std::move(b); or T a(std::move(b));
// func args passing : f(std::move(a)); void f(T t);
// func return : return a;
// 
// Implicitly-declared move constructor 
// Move constructor is implicitly created when all four conditions are true
// no user-declared copy constructors
// no user-declared copy assignment operators
// no user-declared move assignment operators
// no user-declared destructors
//

#include <string>
#include <iostream>
#include <iomanip>
#include <utility>
 
struct A
{
    std::string s;
    int k;
    A() : s("test"), k(-1) { }
    A(const A& o) : s(o.s), k(o.k) { std::cout << "move failed!\n"; }
    A(A&& o) noexcept :
           s(std::move(o.s)),       // explicit move of a member of class type
           k(std::exchange(o.k, 0)) // explicit move of a member of non-class type
    { }
};
 
A f(A a) 
{
    return a;
}

struct B : A
{
    std::string s2;
    int n;
    // implicit move constructor B::(B&&)
    // calls A's move constructor
    // calls s2's move constructor
    // and make bitwise copy fo n
};

struct C : B
{
    ~C() {} // this prevents calling of implicit move constructor C::(C&&)
};

struct D : B
{
    D() {}
    ~D() {}             // this prevents calling of implicit move constructor C::(C&&)
    D(D&&) = default;   // Force a move constructor anyway
};

int main()
{
    std::cout << "Trying to move A\n";
    A a1 = f(A()); // return by value move-constructs the target from the function parameter
    std::cout << "Befoer move, a1.s = " << std::quoted(a1.s) << " a1.k = " << a1.k << '\n';
    A a2 = std::move(a1); // move-constructs from xvalue
    std::cout << "After move, a1.s = " << std::quoted(a1.s) << " a1.k = " << a1.k << '\n';
 
    std::cout << "Trying to move B\n";
    B b1;
    std::cout << "Before move, b1.s = " << std::quoted(b1.s) << "\n";
    B b2 = std::move(b1); // calls implicit move constructor
    std::cout << "After move, b1.s = " << std::quoted(b1.s) << "\n";
 
    std::cout << "Trying to move C\n";
    C c1;
    C c2 = std::move(c1); // calls copy constructor
 
    std::cout << "Trying to move D\n";
    D d1;
    D d2 = std::move(d1);
}

// Move Assignment Operator
//
// T& T::operator=(T&&)              - Declaration of assinment operator
// T& T::operator=(T&&) = default    - Force compiler to make move assingment operator
// T& T::operator=(T&&) = delete;    - remove implicit move assingment operator

// Implicitly decalred move assingment operator
// Same condition as move constructors

#include <string>
#include <iostream>
#include <utility>

struct A
{
    std::string s;
    A() : s("Test") {}
    A(const A& o) : s(o.s) { std::cout << "Move failred" << std::endl; }
    A(A&& o) : s(std::move(o.s)) {}
    A& operator=(const A& other)
    {
        s = other.s;
        std::cout << "Copy assingment operator" << std::endl;
        return *this;
    }
    A& operator=(const A&& other)
    {
        s = std::move(other.s);
        std::cout << "Move assignment operator" << std::endl;
        return *this;
    }
};

A f(A s) { return a; }

struct B : A
{
    std::string s2;
    int n;
    
};

struct C : B
{
    ~C() {}
};

struct D : B
{
    D() {}
    ~D() {}
    D& operator=(D&&) = default;
};

int main()
{
    A a1, a2;
    std::cout << "Trying to move-assign A from rvalue temporary\n";
    a1 = f(A()); // move-assignment from rvalue temporary
    std::cout << "Trying to move-assign A from xvalue\n";
    a2 = std::move(a1); // move-assignment from xvalue
 
    std::cout << "Trying to move-assign B\n";
    B b1, b2;
    std::cout << "Before move, b1.s = \"" << b1.s << "\"\n";
    b2 = std::move(b1); // calls implicit move assignment
    std::cout << "After move, b1.s = \"" << b1.s << "\"\n";
 
    std::cout << "Trying to move-assign C\n";
    C c1, c2;
    c2 = std::move(c1); // calls the copy assignment operator
 
    std::cout << "Trying to move-assign D\n";
    D d1, d2;
    d2 = std::move(d1);
}

/*
Forward
Forwards lvalues as either lvalues or as rvalues, depending on T
 
template <class T> 
T&& forward(typename std::remove_reference<T>::type& t) noexcept;
 
template <class T>
constexpr T&& forward(typename std::remove_reference<T>::type& t) noexcept;

template <class T>
T&& forward(typename std::remove_reference<T>::type&& t) noexcept;

template<class T>
constexpr T&& forward(typename std::remove_reference<T>::type&& t) noexcept;

template<class T>
void wrapper(T&& arg)
{
    foo(std::forward<T>(arg));
}

arg : rvalue -> forward : rvalue
arg : const lvalue -> forward : const lvalue
arg : non-const lvalue -> forward : non-const lvalue

template<class T>
void wrapper(T&& arg)
{
    foo(forward<decltype(forward<T>(arg).get())>(forward<T>(arg).get()));
}

forward can also be used as an expression 
*/


*******************************************************************************/
#include <iostream>
#include <memory>
#include <utility>
#include <array>

struct A
{
    A(int&& n) { std::cout << "rvalue overload, n = " << n << std::endl;
    A(int& n) { std::cout << "lvalue overload, n = " << n << std::endl;
};

class B
{
public:
    template<typename T1, typename T2, typename T3>
    B(T1&& t1, T2&& t2, T3&& t3) :
        a1_{std::forward<T1>(t1)},
        a2_{std::forward<T2>(t2)},
        a3_{std::forward<T3>(t3)}
    {
    }

private:
    A a1_, a2_, a3_;
};

template <typename T, typename U>
std::unique_ptr<T> make_unique1(U&& u)
{
    return std::unique_ptr<T>(new T(std::froward<U>(u)));
}

template <typename T, typename ... U>
std::unique_ptr<T> make_unique(U&& ... u)
{
    return std::unique_ptr<T>(new T(std::forward<U>(u) ... ));
}

int main()
{
    auto p1 = make_unique1<A>(2);
    int i = 1;
    auto p2 = make_unique1<A>(i);
    
    std::cout << "B" << std::endl;
    
    auto t = make_unique<B>(2, i, 3);
}



