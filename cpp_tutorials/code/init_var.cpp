#include<iostream>

using namespace std;

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
        char* p=nullptr;
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
            cout<<"调用Time::Time(int hour,int minute,int second)这个构造函数"<<endl;
        }
        Time(int):hour(0),minute(0),second(0){
            cout<<"调用Time::Time(int)这个构造函数"<<endl;
        }

        explicit Time(string):hour(0),minute(0),second(0){
            cout<<"调用explicit Time::Time(string)这个构造函数"<<endl;
        }

        // 拷贝构造函数，编译器默认提供，此处自己重写(原有拷贝构造函数消失)
        Time(const Time &time){
            *this=time;
            cout<<"调用Time::Time(const Time &time)这个拷贝构造函数"<<endl;
        }

        // 析构函数，编译器默认提供
        //~Time(){}

        void get_time(){
            cout<<hour<<" "<<minute<<" "<<second<<endl;
        }
    };
    int Time::static_id=1;//注意静态变量类外定义还需要重新申明类型,不然报错！

    void test(){
        cout<<"--classType--"<<endl;
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
            cout<<"执行了Human::Human()构造函数"<<endl;
        }
        Human(int age){
            cout<<"执行了Human::Human(int age)构造函数"<<endl;
        }
    };

    class Man:public Human{
    public:
        Man(){
            cout<<"Man::Man()构造函数"<<endl;
        }
    };

    class Woman:public Human{
    public:
        Woman():Human(0){
            cout<<"Woman::Woman()构造函数"<<endl;
        }
    };

    void test(){
        cout<<"--ctor--"<<endl;
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