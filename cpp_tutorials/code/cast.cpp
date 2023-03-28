#include<iostream>

using namespace std;

namespace custom_var{
    void test(){
        //隐式类型转换
        cout<<"--隐式类型转换--"<<endl;
        int a=3+3.3;
        double b=3+3.3;
        cout<<a<<" "<<b<<endl;

        //显式类型转换
        cout<<"--显式类型转换--"<<endl;
        //(1)C语言风格
        cout<<"(1)C语言风格"<<endl;
        int k=5%(int)3.2;
        cout<<k<<endl;

        //(2)static_cast:静态转换,等价于(1)
        cout<<"(2)static_cast:静态转换,等价于(1)"<<endl;
        double f=3.14f;
        int n=static_cast<int>(f);
        cout<<f<<" "<<n<<endl;

        //(3)dynamic_cast:运行时转换，应用于类型识别和检查, 保证转换安全
        cout<<"(3)dynamic_cast:运行时转换，应用于类型识别和检查, 保证转换安全"<<endl;
        
        //(4)const_cast:去除指针或者引用的const属性,不是指针和引用不能使用
        cout<<"(4)const_cast:去除指针或者引用的const属性,不是指针和引用不能使用"<<endl;
        const int i=9;
        const int* pi=&i;
        cout<<*pi;
        // 错误：pi=const_cast<int*>(pi); 
        int *pj=const_cast<int*>(pi); 
        (*pj)++;
        cout<<" "<<*pj<<endl;

        int &j=const_cast<int&>(i);
        cout<<j;
        j++;
        cout<<" "<<j<<endl;

        //(5)reinterpret_cast:编译时就进行类型转换，重新解读类型
        cout<<"(5)reinterpret_cast:编译时就进行类型转换，重新解读类型"<<endl;
        cout<<"一般不使用，不安全"<<endl;
    }
}

namespace classType{
    class Base{
    private:
        string name;
    public:
        Base():name("Base"){}
        virtual void get_name(){
            cout<<"this is "<<name<<endl;
        }
    };

    class A:public Base{
    private:
        string name;
    public:
        A():name("A"){}
        virtual void get_name() override{
            cout<<"this is "<<name<<endl;
        }
        void change_name(){
            name="newA";
        }
    };

    void test(){
        cout<<"--classType--"<<endl;
        cout<<"(1)使用static_cast,将派生类(子类)强制转换为基类(父类)"<<endl;
        A a;
        a.get_name();
        Base b=static_cast<Base>(a);
        b.get_name();

        cout<<"(2)使用dynamic_cast,将基类(父类)指针或引用动态转换为派生类(子类)指针或引用"<<endl;//也可以理解为子类隐私转换成父类
        Base* pb=new A;//父类指针指向子类
        try{
            cout<<"多态，由于父类指针指向子类，并执行了虚函数，所以此处执行的还是子类的函数"<<endl;
            pb->get_name();
            cout<<"使用dynamic_cast,将基类(父类)指针或引用动态转换为派生类(子类)指针或引用,以此可以执行子类的其他函数,注意父类必须存在虚函数"<<endl;
            A* pa=dynamic_cast<A*>(pb);//动态转换回子类指针
            pa->change_name();
            pa->get_name();
        }   
        catch(bad_cast){
            //转换失败
            cout<<"指针指向或引用不是A类型"<<endl;
        }
    }
}


int main(){
    
    custom_var::test();
    classType::test();

    return 0;
}