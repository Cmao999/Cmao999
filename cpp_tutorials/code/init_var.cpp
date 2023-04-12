#include<iostream>
#include"init_var.h"
using namespace std;

static int global_static_int=1;//全局静态变量(常量表达式则在编译时就初始化了，非常量表达式，运行时才初始化)

// 静态变量的初始化时机取决于它的声明位置和初始化方式。
// 如果静态变量是在全局作用域内定义的(全局静态变量)，那么它在编译时被初始化。如果静态变量是在函数内部定义的(局部静态变量)，那么它在第一次调用该函数时被初始化。
// 无论静态变量是在函数内部还是全局作用域内定义，如果它的初始值是一个编译时常量表达式，那么它在编译时就被初始化了。这种情况下，初始化发生在程序运行之前，因此可以说是编译时初始化。
// 如果静态变量的初始值不是编译时常量表达式，那么它在运行时被初始化。这种情况下，初始化发生在程序运行时，因此可以说是运行时初始化。
// 需要注意的是，无论是编译时初始化还是运行时初始化，静态变量只会被初始化一次。在后续的函数调用或程序运行中，静态变量的值都将保持不变。

#define MACRO_INT 1+2;//传统写法，宏定义，编译时初始化
constexpr int constexpr_int=3+4;//新写法，替代宏定义的写法，编译时初始化,只支持常量表达式

// 静态变量和constexpr都可以用来定义常量值，但它们有几个不同之处：
// 可变性不同：静态变量可以是可变的，也可以是不可变的，而constexpr定义的常量是不可变的。
// 初始化时刻不同：静态变量在程序开始时或函数被调用时被初始化，而constexpr常量在编译时就被计算出来。
// 类型要求不同：constexpr常量必须是字面值类型，而静态变量可以是任何类型。
// 存储方式不同：静态变量在内存中有实际的存储空间，而constexpr常量在编译时就被替换成其值，因此不会有存储空间。
// 总的来说，如果需要一个可变的常量值，可以使用静态变量；如果需要一个不可变的常量值，并且需要在编译时计算，可以使用constexpr常量。此外，静态变量也可以用于存储在程序运行期间需要保持不变的值，例如计数器、状态标志等。

extern int global_int;//9

namespace custom_var{
    void test(){
        cout<<"--custom_var--"<<endl;
        //(1)初始化写法
        cout<<"(1)初始化写法"<<endl;
        //传统初始化
        int a=5;
        cout<<a<<endl;
        //新标准初始化
        int b{5};
        int c={5};
        int d(5);
        cout<<b<<" "<<c<<" "<<d<<endl;

        //(2)auto关键字+for新写法
        cout<<"(2)auto关键字+for新写法"<<endl;
        auto e=1;
        auto f=1.1;
        auto g="abc";
        cout<<e<<" "<<f<<" "<<g<<endl;
        
        int h[]{1,2,3};
        int i[]={4,5,6};
        int j[3]={4,5,6};
        for(auto &x:h)
            cout<<x<<" ";
        cout<<endl;
        for(auto &x:i)
            cout<<x<<" ";
        cout<<endl;
        for(auto &x:j)
            cout<<x<<" ";
        cout<<endl;

        //(3)nullptr替代NULL
        cout<<"(3)nullptr替代NULL"<<endl;
        char* p=nullptr;

        //(4)extern全局变量
        cout<<"(4)extern全局变量"<<endl;
        cout<<"global_int="<<global_int<<endl;

        //(5)constexpr:替代宏定义的写法，编译时初始化,只支持常量表达式
        cout<<"(5)constexpr:替代宏定义的写法，编译时初始化,只支持常量表达式"<<endl;
        cout<<"constexpr_int="<<constexpr_int<<endl;

        //(6)static:全局静态变量和局部静态变量，常量一般直接用constexpr了
        cout<<"(6.1)全局静态变量(常量表达式则在编译时就初始化了，非常量表达式，运行时才初始化)"<<endl;
        cout<<"global_static_int"<<global_static_int<<endl;
        
        cout<<"(6.2)局部静态变量(在第一次调用函数时才被初始化)"<<endl;
        static int static_int=9;
        cout<<"static_int"<<static_int<<endl;

    }
}

namespace classType{
    class Time{
    private:
        static const int id=0;//静态成员常量可以在此处初始化，也可以在类外初始化
        int hour,minute,second;
    
    public:
        static int static_id;//静态成员变量只能在类内申明，在类外进行初始化，不能在类内初始化

        // 构造函数，编译器默认提供，此处自己重写(原有默认构造函数消失)，使用构造函数初始化列表，摒弃在函数体内初始化
        // Time()=default；//引入默认构造函数
        // Time()=delete;  //禁用默认构造函数
        Time():hour(0),minute(0),second(0){}
        Time(int hour,int minute,int second):hour(hour),minute(minute),second(second){
            // cout<<"调用Time::Time(int hour,int minute,int second)这个构造函数"<<endl;
            cout<<"调用"<<__PRETTY_FUNCTION__<<"这个构造函数"<<endl;
        }
        Time(int):hour(0),minute(0),second(0){
            cout<<"调用"<<__PRETTY_FUNCTION__<<"这个构造函数"<<endl;
        }

        explicit Time(string):hour(0),minute(0),second(0){
            cout<<"调用"<<__PRETTY_FUNCTION__<<"这个构造函数"<<endl;
        }

        // 拷贝构造函数，编译器默认提供，此处自己重写(原有拷贝构造函数消失)
        Time(const Time &time){
            *this=time;
            cout<<"调用"<<__PRETTY_FUNCTION__<<"这个拷贝构造函数"<<endl;
        }

        // 析构函数，编译器默认提供
        //~Time(){}

        void get_time(){
            cout<<hour<<" "<<minute<<" "<<second<<endl;
        }
    };
    int Time::static_id=1;//注意静态变量类外定义还需要重新申明类型,不然报错！

    void test(){
        cout<<endl<<"--classType--"<<endl;
        //(1)支持多种构造写法，调用构造函数，单参构造函数注意隐式转换
        cout<<"(1)支持多种构造写法，调用构造函数"<<endl;
        Time time1(1,2,3);
        Time time2=Time(1,2,3);
        Time time3=Time{1,2,3};
        Time time4{1,2,3};
        Time time5={1,2,3};

        //单参构造函数存在隐式转换
        Time time6=1;
        Time time7={1};
        //添加explicit要求不能隐私转换
        // 错误：Time time="time";
        // 错误：Time time={"time"};
        Time time8("time");
        Time time9{"time"};
        Time time10=Time("time");
        Time time11=Time{"time"};
        
        //(2)拷贝构造函数
        cout<<"(2)拷贝构造函数"<<endl;
        Time time(9,9,9);
        Time time_c1(time);
        Time time_c2=time;
        Time time_c3={time};
        Time time_c4{time};
        Time time_tmp;
        time_tmp=time;//由于time6已经存在，所以没有调用拷贝构造函数，编译器直接复制赋值，如果成员变量含指针，需自己重载operatorr=()
        time_tmp.get_time();
    }
}

namespace ctor{
    class Human{
    public:
        Human(){
            cout<<"调用"<<__PRETTY_FUNCTION__<<"这个构造函数"<<endl;
        }
        Human(int age){
            cout<<"调用"<<__PRETTY_FUNCTION__<<"这个构造函数"<<endl;
        }
    };

    class Man:public Human{
    public:
        Man(){
            cout<<"调用"<<__PRETTY_FUNCTION__<<"这个构造函数"<<endl;
        }
    };

    class Woman:public Human{
    public:
        Woman():Human(0){
            cout<<"调用"<<__PRETTY_FUNCTION__<<"这个构造函数"<<endl;
        }
    };

    void test(){
        cout<<endl<<"--ctor--"<<endl;
        cout<<"(1)子类,父类构造函数的顺序"<<endl;
        Man* man=new Man;
        delete man;
        
        cout<<"(2)子类实例化，需要调用父类的有参构造函数:利用初始化列表"<<endl;
        Woman* woman=new Woman;
        delete woman;
    }
}

int main(){
    custom_var::test();
    classType::test();
    ctor::test();
    return 0;
}