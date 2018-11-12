/*
ref : http://egloos.zum.com/sweeper/v/2826160

RTTI ( Real Time Type Information )

// RTTI Doesn't work

#include <stdio.h>
#include <typeinfo>

class Base
{
    public:
        void B(){}
};

class Child1 : public Base
{
    public:
        void C(){}
};

class Child2 : public Base
{
    public:
        void C(){}
};

int main()
{
    Base* cast = new Child1();
    Base* cast2 = new Child2();
    
    printf( "%s \n", typeid(*cast).name() );  
    printf( "%s \n", typeid(*cast2).name() );  
}

// RTTI Work ( by using vftbl ) 
#include <stdio.h>
#include <typeinfo>

class Base
{
    public:
        void B(){}
        virtual void b(){}
};

class Child1 : public Base
{
    public:
        void C(){}
        virtual void b(){}
};

class Child2 : public Base
{
    public:
        void C(){}
        virtual void b(){}
};

int main()
{
    Base* cast = new Child1();
    Base* cast2 = new Child2();
    
    printf( "%s \n", typeid(*cast).name() );  
    printf( "%s \n", typeid(*cast2).name() );  
}
