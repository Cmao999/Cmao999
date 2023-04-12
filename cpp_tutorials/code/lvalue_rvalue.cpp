#include<iostream>

using namespace std;

static int static_int=10;//静态变量存储在全局区，编译时分配内存，程序结束才释放

namespace value_reference{

    class A{
    public:
        int id;
        A():id(1){}
    };

    void test(){
        cout<<"--value_reference--"<<endl;
        //内存分为四个区:栈区、堆区、全局区、代码区
        //栈区：由编译器自动分配和释放，存放函数参数、局部变量、临时变量等
        //堆区：由程序员分配释放，若程序员未释放，则程序结束之后，编译器自动释放
        //代码区：存放函数体内的二进制代码，由操作系统管理
        //全局区：在main函数体外定义的全局变量，全局静态变量以及全局常量。与之对应的是局部变量,只在某个域内有效。

        //左值:在内存上有显式地址的值，能被赋值，也能被用于赋值
        //常量左值:被限定为常量的左值，实际还是左值，只是限定了不能被赋值
        //右值:将亡值！例如：字面量、或临时变量，在内存上没有显式地址，可能在只读区(字面量)或栈堆全局代码区(临时变量)，不能被赋值，只能用于赋值
        int l=1;
        int tmp=9;
        l=tmp;//9

        //左值引用：只能绑定左值
        //右值引用：只能绑定右值
        //常量左值引用：可以绑定左值也可以绑定右值
        //std::move()可以将左值强制转换成右值
        //注意:无论是左值引用还是右值引用，其变量都为左值
        cout<<"在栈区申明定义左值和右值引用，栈区内存空间变化："<<endl;
        int l_value=9;
        const int const_l_value=10;
        cout<<"sizeof(int)="<<sizeof(l_value)<<endl;
        cout<<"(1)"<<&l_value<<endl;
        int& l_reference1=l_value;
        cout<<"(2)"<<&l_reference1<<endl;
        int&& r_reference2=1;
        cout<<"(3)"<<&r_reference2<<endl;
        int&& r_reference3=std::move(l_value);
        cout<<"(4)"<<&r_reference3<<endl;
        int& l_reference_static=static_int;
        cout<<"(5)"<<&l_reference_static<<endl;//右值引用绑定的内存是哪里？
                                            //当右值引用绑定临时变量时，右值引用接管临时变量的内存(隐式地址转为显示地址),例如临时变量在栈区，则接管其在栈区的内存空间，临时变量在全局区则接管其在全局区的内存空间
                                            //当右值引用绑定字面量时，猜测应该是给右值引用指向一个新空间，将将字面量拷贝进去，没太大意义，一般不建议这样用！
        const int& const_l_reference1=l_value;
        cout<<"(6)"<<&const_l_reference1<<endl;
        const int& const_l_reference2=1;
        cout<<"(7)"<<&const_l_reference2<<endl;

        cout<<"栈区指针指向堆区，堆区分配和释放后，内存空间变化对比："<<endl;
        int* p_int=new int(10);
        cout<<"sizeof(int*)="<<sizeof(p_int)<<endl;//8
        cout<<"(1)"<<*p_int<<endl;//10
        cout<<"(2)"<<p_int<<endl;
        cout<<"(3)"<<&p_int<<endl;
        cout<<"(4)"<<&(*p_int)<<endl;
        delete p_int;//释放了指针指向的堆区内存，但指针本身还存在，指针的值仍不变，即仍指向堆区的那块内存
        cout<<"(5)"<<*p_int<<endl;//0,堆区内存被释放
        cout<<"(6)"<<p_int<<endl;
        cout<<"(7)"<<&p_int<<endl;
        cout<<"(8)"<<&(*p_int)<<endl;

        cout<<"临时变量虽然在栈区中，但没有显式地址(编译器不允许),通过右值引用可以获得该临时变量的栈区内存使用权,这样就不会释放掉临时变量的内存空间了"<<endl;;
        A a;
        cout<<"(1)"<<&a<<endl;
        A();//临时变量在栈区，在这一行结束后就会被释放
        int i1=1;
        cout<<"(2)"<<&i1<<endl;
        A&& a_reference=A();//如果临时变量被右值引用了，那么该左值变量(即右值引用)则接管临时变量的栈区内存
        cout<<"(3)"<<&a_reference<<endl;
    }
}

//类的拷贝构造函数和赋值运算的改进:移动拷贝构造和移动赋值运算
namespace ctor_operator{

    //拷贝构造函数和赋值运算
    //移动拷贝构造函数和移动赋值运算
    class Integer{
    private:
        int* m_pint;

    public:
        Integer(int val):m_pint(new int(val)){
            cout<<"调用Integer::Integer(int val)，内存地址为:"<<m_pint<<endl;
        }

        //深拷贝，不改变source，创造一个新的内存空间，把soucre的指针指向的内容拷贝过去
        Integer(const Integer& source):m_pint(new int(*source.m_pint)){
            cout<<"调用Integer::Integer(const Integer& source),内存地址:"<<m_pint<<endl;
        }

        //浅拷贝，效率比深拷贝高(只作为示例，不建议这样写,需写成移动拷贝构造)
        //直接转让source的指针指向的内存，构造后原source将不能使用(这里只作为演示)
        //但这样写不支持临时变量浅拷贝，临时变量还是需要调用深拷贝，而且逻辑上也不合理，会破坏原source的内容
        Integer(Integer& source):m_pint(source.m_pint){
            source.m_pint=nullptr;
            cout<<"调用Integer::Integer(Integer& source),内存地址:"<<m_pint<<endl;
        }

        //移动拷贝构造：改进的浅拷贝，适用于临时变量(右值)传入，效率比深拷贝高！
        //直接转让source的指针指向的内存，构造后原source将不能使用(合理，因为是临时变量，后面也不会再使用了)
        //如果在程序中，明确左值(本类对象)后续不会使用，则直接调用std:move()将其转换成右值也可以调用此函数
        Integer(Integer&& source):m_pint(source.m_pint){
            source.m_pint=nullptr;
            cout<<"调用Integer::Integer(Integer&& source),内存地址:"<<m_pint<<endl;
        }
        

        //赋值运算深拷贝
        Integer& operator=(const Integer& source){
            m_pint=new int(*source.m_pint);
            cout<<"调用Integer::Integer& operator=(const Integer& source),内存地址:"<<m_pint<<endl;
            return *this;
        }

        //赋值运算浅拷贝，效率比深拷贝的赋值运算高(只作为示例，不能这样写，需写成移动赋值运算)
        //直接转让source的指针指向的内存，构造后原source将不能使用(这里只作为演示)
        //但这样写不支持临时变量浅拷贝，临时变量还是需要调用深拷贝,而且逻辑上也不合理，会破坏原source的内容
        Integer& operator=(Integer& source){
            m_pint=source.m_pint;
            source.m_pint=nullptr;
            cout<<"调用Integer::Integer& operator=(Integer& source),内存地址:"<<m_pint<<endl;
            return *this;
        }

        //移动赋值运算：改进的赋值运算浅拷贝，适用于临时变量(右值)传入，效率比赋值运算深拷贝高！
        //直接转让source的指针指向的内存，构造后原source将不能使用(合理，因为是临时变量，后面也不会再使用了)
        //如果在程序中，明确左值(本类对象)后续不会使用，则直接调用std:move()将其转换成右值也可以调用此函数
        Integer& operator=(Integer&& source){
            m_pint=source.m_pint;
            source.m_pint=nullptr;
            cout<<"调用Integer::Integer& operator=(Integer&& source),内存地址:"<<m_pint<<endl;
            return *this;
        }

        ~Integer(){
            // cout<<"调用Integer:~Integer()析构,释放内存:"<<m_pint<<endl;
            delete m_pint;
        }
    };

    void test(){
        cout<<"--ctor_operator--"<<endl;
        //注意因为是栈区,构造是从上往下,析构是反之,从下往上
        cout<<"(1)ctor"<<endl;
        Integer Int1(9);
        Integer Int2(9);
        Integer Int3(Int1);//调用了Integer::Integer(Integer& source,Int1不能再使用了
        Integer Int4(Integer(9));//临时变量Integer(9)的内存空间被Int4接管
        Integer Int5(std::move(Int2));//Int2强转成右值，内存空间被Int5接管,Int2不能再使用了
    
        cout<<"(2)opertor"<<endl;
        Integer Int6(9);
        Integer Int7(9);
        Integer Int8(9);
        Int6=Int7;//调用Integer::Integer& operator=(Integer& source),Int7不能再使用了
        Int7=std::move(Int6);//调用Integer::Integer& operator=(Integer&& source),Int6不能再使用了
        Int8=Integer(9);//调用Integer::Integer& operator=(Integer&& source),Int8接管临时变量的内存
    }
}

int main(){
    value_reference::test();
    ctor_operator::test();
    return 0;
}