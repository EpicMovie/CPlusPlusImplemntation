#include <iostream>
#include <string>
#include <utility>

/*
CV-Qualified 연습
CV-Qualified란? 타입의 선언절에서 Const, Volatile, Mutable을 명시해주어야 한다.

int main()
{
    int n1 = 0;
    const int n2 = 0;
    int const n3 = 0; // n2 == n3
    volatile int n4 = 0;
    
    const struct
    {
        int n1;
        mutable int n2;
    } x = {0, 0};
    
    n1 = 1;
//  n2 = 2; Error will be occured 
    n4 = 3; 
//  x.n1 = 4; Member of a const object is const
    x.n2 = 4; // Ok, mutalbe member of a const object isn't const
    
    const int& r1 = n1; // reference to const bound to non-const object
//  r1 = 2; Attempt to modify through reference to const 
    const_cast<int&>(r1) = 2; // 
    
    const int& r2 = n2; // reference to const bound to const object
//  r2 = 2;
//  const_cast<int&>(r2) = 2; Undefined behavior : attempt to modify const object n2
}
*/

/*
L, R-Value Reference 연습

int main()
{
    std::string s1 = "Test";
//  std::string&& r1 = s1; // Error : can't bind to lvalue

    const std::string& r2 = s1 + s1;
//  r2 += "Test"; // can't modify through reference to const 
//  std::cout << r2 << std::endl;

    std::string&& r3 = s1 + s1; 
    r3 += "Test";
    
    std::cout << r3 << std::endl;
    
    return 0;
}
*/

/*
L, R-Value Reference 연습

void f(int& x)
{
    std::cout << "L-Value reference overload f(" << x << ")" << std::endl;
}

void f(const int& x)
{
    std::cout << "L-Value reference to const overload f(" << x << ")" << std::endl;
}

void f(int&& x)
{
    std::cout << "R-Value reference overload f(" << x << ")" << std::endl;
}

int main()
{
    int i = 1;
    const int ci = 2;
    f(i);  // call f(int&)
    f(ci); // call f(const int &)
    f(3);  // call f(int &&)
    
    f(std::move(i)); // call f(int&&)
    
    int&& x = 1;     // RValue 참조 변수가 expression으로 사용될 경우 lvalue로 취급된다.
    f(x);            // calls f(int& x)
    f(std::move(x)); // calls f(int&& x)
    f(std::move(i)); // calls f(int&& x)
}
*/



