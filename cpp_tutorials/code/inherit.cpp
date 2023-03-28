#include<iostream>

using namespace std;

//注意：子类会继承父类的所有东西(除同名函数，同名函数会被子类覆盖),只是子类不一定有权限访问
namespace access_authority{
    
    class Base{
    //补充，友元函数，友元类，友元成员函数
    //友元类和友元成员函数同理，此处不赘述
    friend void get_priv();//申明友元函数

    public:
        Base():m_pub(0),m_pro(0),m_priv(0){}

    //按照子类继承方式,得到相应的访问权限
    public:
        int m_pub;
        void fun(){
            cout<<"执行Base::fun()"<<endl;
        }
        void func_pub(){
            cout<<"按照子类继承方式,得到相应的访问权限"<<endl;
        }
           
    //子类得到的访问权限还是protected
    protected:
        int m_pro;
        void func_pro(){
            cout<<"子类得到的访问权限还是protected"<<endl;
        }
    
    //子类永远无权访问
    private:
        int m_priv;
        void func_priv(){
            cout<<"子类永远无权访问"<<endl;
        }
    };

    //定义友元函数，可以访问类内的protected、private属性的成员
    void get_priv(){
        Base base;
        cout<<"执行友元函数get_priv()，访问private属性的priv:"<<base.m_priv<<endl;
    }

    class A:public Base{
    public:
        A(){
            cout<<"public继承方式，父类所有成员在子类的访问权限不变"<<endl;
        }
        void fun(){
            cout<<"调用A::fun(),继承过来的父类的同名函数被子类覆盖"<<endl;
            cout<<"可以通过调用Base::fun()调用父类函数:";
            Base::fun();//为什么可以调用非静态函数，因为子类执行这个函数时，父类必存在，子类实例化会先调用父类构造再调用子类构造
        }
    };
    
    class B:protected Base{
    public:
        B(){
            cout<<"protected继承方式，父类成员(除pirvate)在子类访问权限为protected"<<endl;
        }
    };

    class C:private Base{
    public:
        C(){
            cout<<"private继承方式，父类所有成员在子类访问权限为private"<<endl;
        }
    };

    
    void test(){
        cout<<"--access_authority--"<<endl;
        A a;
        B b;
        C c;
        a.fun();
        get_priv();
    }
}

//虚函数/纯虚函数，多态
//父类存在虚函数，则会生成虚函数表(被子类共享)，存放指向类中各个虚函数的地址
namespace Polymorphism{
    class Animal{
    public:
        Animal(){}
        //虚函数(如果子类不需要，子类可以不重写)
        virtual void get_name(){
            cout<<"我是动物"<<endl;
        }
        virtual void test_final()final{}//final标识为父类虚函数特有标志，与override对立，不允许子类重写此虚函数

        //纯虚函数(子类必须定义)
        //由于纯虚函数只有申明没有定义，所以含纯虚函数的类为抽象类，不能用来生成对象
        //一般来说，一个类写了纯虚函数，那类的其他的函数也应该写成纯虚函数，不然没有意义，此处只是作为示例
        virtual void action()=0;
    };

    class Cat:public Animal{
    public:
        Cat(){}
        //子类重写虚函数, 申明(名字和形参)必须和父类相同，否则无法重写，添加override就是避免这一点
        //override标识为子类虚函数特有，避免写错虚函数申明
        virtual void get_name() override {
            cout<<"我是一只猫"<<endl;
        }
        //子类定义纯虚函数
        virtual void action(){
            cout<<"释放技能：撕裂猫爪"<<endl;
        }
    };

    class Dog:public Animal{
    public:
        Dog(){}
        //子类重写虚函数
        virtual void get_name() override {
            cout<<"我是一只狗"<<endl;
        }
        //子类定义纯虚函数
        virtual void action(){
            cout<<"释放技能：狗吠"<<endl;
        }
    };

    //多态性
    //利用父类指针可以指向子类对象这一特点，配合虚函数，可以执行传入的对象特有的函数
    //没有虚函数，就不存在多态
    void animal_fun(Animal* animal){
        //输出该动物名字
        animal->get_name();
        //释放技能
        animal->action();
    }

    void test(){
        cout<<"--Polymorphism--"<<endl;

        //父类指针指向子类Cat
        Animal* cat=new Cat;
        animal_fun(cat);
        delete cat;

        //父类指针指向子类Dog
        Animal* dog=new Dog;
        animal_fun(dog);
        delete dog;
    }
}

namespace ctor_dtor{
    class Human{
    public:
        Human(){
            cout<<"执行了Human::Human()构造函数"<<endl;
        }
        Human(int age){
            cout<<"执行了Human::Human(int age)构造函数"<<endl;
        }

        //注意：建议父类的析构函数都写成虚函数！
        //因为多态会使用父类指针指向子类对象，delete父类指针指向的内存的时候，只会执行父类的析构函数，不会执行子类析构函数！！！
        //所以必须将父类析构函数写成虚函数，才能执行子类析构函数！！！
        virtual ~Human(){
            cout<<"执行了Human::~Human()析构函数"<<endl;
        }
    };

    class Man:public Human{
    public:
        Man(){
            cout<<"Man::Man()构造函数"<<endl;
        }
        ~Man(){
            cout<<"Man::~Man()析构函数"<<endl;
        }
    };

    void test(){
        cout<<"--ctor_dtor--"<<endl;
        cout<<"(1)子类,父类构造函数和析构函数的顺序"<<endl;
        //子类实例化会先调用父类构造函数，再调用子类构造函数
        //析构反之，先调用子类析构函数，再调用父类析构函数
        Man* man_a=new Man;
        delete man_a;

        cout<<"(2)多态的析构陷阱:必须将父类析构函数写成虚函数，才能执行子类析构函数"<<endl;
        Human* man_b=new Man;//父类指针指向子类对象
        delete man_b;
    }
}

//RTTI(Run Time Type Identification)
//(1)dynamic_cast：父类指针指向子类(或引用)时，指针想要执行子类的函数，除了虚函数的方法外，还可以利用动态类型转换，转成子类指针(引用)
//(2)typeid:返回指针或引用的实际类型type_info
//注意:父类必须存在虚函数，否则无法使用RTTI特性!!!因为不是多态不会将父类指针指向子类!!!
namespace RTTI{
    class Human{
    public:
        Human(){}
        virtual void v(){}//父类必须存在虚函数，否则无法使用RTTI特性!!!
    };

    class Man:public Human{
    public:
        Man(){}
        void get_name(){
            cout<<"I am a man"<<endl;
        }
    };

    class Woman:public Human{
    public:
        Woman(){}
        void get_name(){
            cout<<"I am a woman"<<endl;
        }
    };

    void test(){
        cout<<"--RTTI--"<<endl;
        cout<<"(1)dynamic_cast：将父类指针或引用动态转换成子类指针或引用,父类一定要存在虚函数"<<endl;
        Human* phuman=new Man;//此时man是父类指针，无法直接调用子类的函数(除了虚函数)
        try{
            Man* pman=dynamic_cast<Man*>(phuman);
            pman->get_name();
        }
        catch(bad_cast){
            cout<<"这不是一个man类对象"<<endl;
        }
        delete phuman;

        cout<<"(2)typeid:返回指针或引用的实际类型type_info"<<endl;
        Human* phuman1=new Man;
        Human* phuman2=new Man;
        Human* phuman3=new Woman;
        Human& human1=*phuman1;

        cout<<"用于引用时, 返回对象的动态类型:"<<endl;
        cout<<typeid(*phuman1).name()<<endl;
        cout<<typeid(human1).name()<<endl;
        if(typeid(phuman2)==typeid(phuman3))
            cout<<"动态类型相同,都是Man类"<<endl;

        cout<<"用于指针时, 返回指针的静态类型:"<<endl;
        cout<<typeid(phuman1).name()<<endl;
        cout<<typeid(phuman3).name()<<endl;
        if(typeid(phuman1)==typeid(phuman3))
            cout<<"静态类型相同,都是Human类"<<endl;
    }
}

int main(){
    access_authority::test();
    Polymorphism::test();
    ctor_dtor::test();
    RTTI::test();
    return 0;
}