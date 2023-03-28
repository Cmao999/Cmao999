#include<iostream>
#include<memory>//智能指针的头文件,且智能指针定义在std的命名空间内

using namespace std;

/*
    C++的new和delete与C中的malloc和free有什么区别？
    new运算符:1.分配内存(operator new)，2.调用构造函数初始化
    delete运算符:1.释放内存(operator delete)，2.调用析构函数
    前者具有对堆分配内存进行初始化的能力，即使用new生成一个类对象时，系统会调用该类的构造函数，使用delete释放对象时，系统会调用析构函数。
*/

namespace custom_var{
    void test(){
        cout<<"--custom_var--"<<endl;
        //(1)指针变量名=new 类型标识符
        cout<<"(1)指针变量名=new 类型标识符"<<endl;
        int* pa = new int;//分配内存返回地址
        if(pa!=nullptr){
            *pa=1;
            cout<<*pa<<endl;
            delete pa;
        }

        //(2)指针变量名=new 类型标识符(初始值)
        cout<<"(2)指针变量名=new 类型标识符(初始值)"<<endl;
        int* pb = new int(2);//分配内存并设内容为2，返回地址
        if(pb!=nullptr){
            cout<<*pb<<endl;
            delete pb;
        }

        //(3)指针变量名=new 类型标识符[内存单元个数]
        cout<<"(3)指针变量名=new 类型标识符[内存单元个数]"<<endl;
        int size=3;
        int* pc = new int[size];//分配3个int大小的内存空间，返回地址
        if(pc!=nullptr){
            for(int i=0;i<size;i++)
                *(pc+i)=i+size;
            for(int i=0;i<size;i++)
                cout<<*(pc+i)<<" ";
            cout<<endl;
            delete[] pc;//注意是delete[]
        }
    }
}

namespace smart_pointer{
    namespace custom_var{
        void myDeleter(int* p){
            cout<<"call myDeleter"<<endl;
            delete p;
        }

        void test(){
            cout<<"--smart_pointer::custom_var--"<<endl;
            shared_ptr<int>p_int1(new int(100));//初始化方式一
            shared_ptr<int>p_int2(make_shared<int>(100));//初始化方式二（推荐)，使用make_shared
            shared_ptr<int>p_int3(p_int2);//p_int2和p_int3指向同一内存空间，引用计数为2

            cout<<"(1)use_conut():返回智能指针指向该内存空间的个数"<<endl;
            cout<<"p_int1.use_conut():"<<p_int1.use_count()<<endl;
            cout<<"p_int2.use_conut():"<<p_int2.use_count()<<endl;
            cout<<"p_int3.use_conut():"<<p_int3.use_count()<<endl;

            cout<<"(2)unique():该智能指针是否独占该内存空间"<<endl;
            if(p_int1.unique()){//执行
                cout<<"p_int1 unique"<<endl;
            }
            if(p_int2.unique()){//不执行
                cout<<"p_int2 unique"<<endl;
            }

            cout<<"(3)reset()/reset(含参)"<<endl;
            p_int1.reset();//智能指针置空，该智能指针不能再使用了
                           //如果原智能指针引用计数为1时，则会释放对象
                           //否则，引用计数不为1，则引用计数减1
            if(p_int1==nullptr){
                cout<<"p_int1被置空"<<endl;
            }

            p_int2.reset();//由于p_int2和p_int3指向同一块内存空间，p_int2被置空，引用计数减1
            cout<<"p_int3.use_conut():"<<p_int3.use_count()<<endl;//1

            p_int1.reset(new int(99));//p_int1为空指针，宁p_int1指向新内存
            p_int2=p_int3;//p_int2重新指向p_int3指向的内存,引用计数加1
            cout<<"p_int3.use_conut():"<<p_int3.use_count()<<endl;//2

            p_int1.reset(new int(99));//p_int1引用计数为1,宁p_int1指向新内存，则原内存空间释放
            p_int2.reset(new int(99));//p_int2引用计数为2，宁p_int2指向新内存，该内存引用计数减一
            cout<<"p_int3.use_conut():"<<p_int3.use_count()<<endl;//1

            cout<<"(4)解引用*"<<endl;
            cout<<"*p_int3="<<*p_int3<<endl;

            cout<<"(5)get():返回智能指针对象所保存的数据指针(裸指针)"<<endl;
            auto raw_p3=p_int3.get();
            *raw_p3=0;
            cout<<"*p_int3="<<*p_int3<<endl;

            cout<<"(6)swap():交换两个智能指针指向的对象"<<endl;
            std::swap(p_int1,p_int3);//本质是智能指针地址（智能指针指向堆中的控制块)交换，原对象的引用计数不变
                                    //p_int1.swap(p_int3)也可以
            cout<<"*p_int1="<<*p_int1<<endl;
            cout<<"*p_int3="<<*p_int3<<endl;

            cout<<"(7)=nullptr"<<endl;
            p_int1=nullptr;//等价于p_int1.reset()
            cout<<"等价于reset()"<<endl;

            cout<<"(8)智能指针作为判断条件"<<endl;
            //若智能指针不为空指针，则成立
            if(p_int3){
                cout<<"*p_int3="<<*p_int3<<endl;
            }

            cout<<"(9)指定删除器"<<endl;
            shared_ptr<int>p_int4(new int(99),myDeleter);//不支持make_shared
            p_int3=nullptr;//引用计数为1，释放对象

            cout<<"定义数组智能指针建议在<>内加入[],否则要指定删除器"<<endl;
            shared_ptr<int[]>p_intArray1(new int[9]);//建议这样定义数组智能指针,释放对象会调用delete[]
            shared_ptr<int>p_intArray2(new int[9],[](int* p){
                                            delete[] p;
                                            cout<<"call匿名函数删除器"<<endl;
                                        });//这样定义数组智能指针需要指定删除器，因为默认使用delete释放
                                        //使用delete可能会导致释放出现问题(内嵌类型不会，但自定义的类，如果用delete释放一个对象数组，将不能够执行所有的析构函数，可能导致内存泄露)

            cout<<"(10)指定分配器(内存分配),暂略"<<endl;
        }
    }

    namespace classType{
        //使用enable_shared_from_this可以将类内的this指针封装为智能指针返回，模板参数为被继承的类
        //原理：该模板有个弱指针weak_ptr,可以观测this,调用shared_from_this()时，内部会调用weak_ptr的lock()，返回shared_ptr
        class Base:public enable_shared_from_this<Base>{
        private:
            string m_name;

        public:
            //将类内的this指针封装为智能指针返回
            shared_ptr<Base> get(){
                return shared_from_this();
            }
            Base(){}

            Base(string m_name):m_name(m_name){
                cout<<m_name+" call Base::Base()"<<endl;
            }

            ~Base(){
                cout<<m_name+" call Base::~Base()"<<endl;
            }
        };

        class extend:public Base{
        public:
            extend(){
                cout<<"call extend::extend1()"<<endl;
            }

            ~extend(){
                cout<<"call extend::~extend1()"<<endl;
            }

        };

        void test(){
            cout<<"--smart_pointer::classType--"<<endl;
            cout<<"(1)使用智能指针初始化类对象"<<endl;
            shared_ptr<Base>p_base1(new Base("p_base1"));//初始化方式一，使用裸指针调用构造函数初始化(不支持隐式转换)
                                                        //此方式需要两次不连续的内存分配，1.new创建数据内存，2.shared_ptr堆上动态创建控制块(用来管理数据)接收数据指针并初始化
                                                        //注意：如果使用相同的裸指针创建shared_ptr，会创建两个控制块，导致内存重复释放
            shared_ptr<Base>p_base2(make_shared<Base>("p_base2"));//初始化方式二（推荐)，使用make_shared
                                                        //写成shared_ptr<Base>p_base2=make_shared<Base>()也可以
                                                        //此方式只需要一次连续内存分配，数据内存和控制块存放在连续内存空间，所以更高效，异常安全
                                                        //异常不安全：因为C++允许参数传递时候打乱顺序，可能出现new创建数据内存之后没有及时传给shared_ptr而执行了别的代码行导致内存泄露
                                                        //make_shared的缺点：1.无法自定义删除器，2.需要保证构造函数，3.可能造成部分内存锁定
            cout<<"p_base2.use_count():"<<p_base2.use_count()<<endl;
            shared_ptr<Base>p_base3(p_base2);//p_base3指向p_base2的内存空间，该内存空间的引用计数增加
            cout<<"p_base2.use_count():"<<p_base2.use_count()<<endl;
            cout<<"p_base3.use_count():"<<p_base3.use_count()<<endl;
            p_base1=nullptr;
            p_base2=nullptr;


            cout<<"(2)智能指针同样适用多态，即父类指针指向子类对象。父类析构不需要写成虚函数，仍可以成功析构！"<<endl;//非智能指针，父类指针要写成虚函数，才能调用父类的析构函数
                                                                                                        //这个特性是因为控制块里面还存储了指向数据指针的指针(即指向子类指针),所以释放智能指针指向的对象会先执行子类析构函数，再执行父类析构函数
            shared_ptr<Base>p_base2extend1(new extend());
            p_base2extend1=nullptr;
            shared_ptr<Base>p_base2extend2(make_shared<extend>());
            p_base2extend2=nullptr;
        }
    }
}


int main(){
    custom_var::test();
    smart_pointer::custom_var::test();
    smart_pointer::classType::test();
    return 0;
}