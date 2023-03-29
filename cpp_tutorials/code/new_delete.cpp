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

        void shared_ptr_test(){
            cout<<endl<<"--smart_pointer::custom_var::shared_ptr_test()--"<<endl;
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

            cout<<"(3)reset()/reset(裸指针参数)"<<endl;
            p_int1.reset();//智能指针置空，该智能指针不能再使用了
                           //如果原智能指针引用计数为1时，则会释放指针指向的资源
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

            cout<<"(9)指定删除器:定义数组智能指针建议在<>内加入[],否则要指定删除器"<<endl;
            shared_ptr<int>p_int4(new int(99),myDeleter);//不支持make_shared
            p_int4=nullptr;//引用计数为1，释放指针指向的资源
            shared_ptr<int[]>p_intArray1(new int[9]);//建议这样定义数组智能指针,释放对象会调用delete[]
            shared_ptr<int>p_intArray2(new int[9],[](int* p){
                                            delete[] p;
                                            cout<<"call匿名函数删除器"<<endl;
                                        });//这样定义数组智能指针需要指定删除器，因为默认使用delete释放
                                        //使用delete可能会导致释放出现问题(内嵌类型不会，但自定义的类，如果用delete释放一个对象数组，将不能够执行所有的析构函数，可能导致内存泄露)
            p_intArray2=nullptr;//引用计数为1，释放指针指向的资源

            cout<<"(10)指定分配器(内存分配),暂略"<<endl;

            cout<<"(11)移动语义:移动构造函数,移动赋值运算"<<endl;
            shared_ptr<int>p_int5(make_shared<int>(99));
            shared_ptr<int>p_int6(std::move(p_int5));//移动构造，把p_int5指向的资源转让给p_int6，p_int5置为空，原资源的引用计数不变
            cout<<"p_int6.use_count():"<<p_int6.use_count()<<endl;
            p_int5=std::move(p_int6);//移动赋值，把p_int6指向的资源转让给p_int5，p_int6置为空，原资源的引用计数不变
            cout<<"p_int5.use_count():"<<p_int5.use_count()<<endl;

            cout<<"(12)尺寸：裸指针(指向数据内存)+控制块指针(指向控制块)"<<endl;
            cout<<"sizeof(shared_ptr)="<<sizeof(p_int1)<<endl;//16,即2倍裸指针大小
        }

        //弱指针，与shared_ptr相对应，用于辅助shared_ptr工作，监视shared_ptr的生命周期
        void weak_ptr_test(){
            cout<<endl<<"--smart_pointer::custom_var::weak_ptr_test()--"<<endl;
            shared_ptr<int>p_int1(make_shared<int>(99));
            weak_ptr<int>pw_int1(p_int1);//只作为观测，不会增加引用计数
            
            cout<<"(1)use_count():返回弱指针共享对象的shared_ptr数量(即观测资源的强引用个数)"<<endl;
            cout<<"pw_int1.use_count():"<<pw_int1.use_count()<<endl;

            cout<<"(2)expired():是否过期？即判断观测资源是否被释放"<<endl;
            p_int1=nullptr;//引用计数为1,释放指针指向的资源
            if(pw_int1.expired()){
                cout<<"pw_int1指向的资源已被释放"<<endl;
            }

            cout<<"(3)reset():弱引用指针置为空，弱引用个数减1，不影响强引用"<<endl;
            shared_ptr<int>p_int2(make_shared<int>(99));
            weak_ptr<int>pw_int2(p_int2);
            pw_int2.reset();
            cout<<"p_int2.use_count():"<<p_int2.use_count()<<endl;

            cout<<"(4)lock():获取所监视的shared_ptr"<<endl;
            shared_ptr<int>p_int3(make_shared<int>(99));
            weak_ptr<int>pw_int3(p_int3);
            
            if(!pw_int3.expired()){
                //没过期
                auto p_intTmp=pw_int3.lock();//返回一个shared_ptr并用于初始化p_intTmp
                                            //此时，强引用计数加1，为2,弱引用计数不变
                cout<<"p_int3.use_count():"<<p_int3.use_count()<<endl;//2
            }
            //离开上述范围后，局部变量p_intTmp被释放，原资源强引用计数减1，弱引用计数任不变
            cout<<"p_int3.use_count():"<<p_int3.use_count()<<endl;//1
            //以下同理，用{}域包含以下代码，离开域后，局部变量被释放
            {
                auto p_intTmp=pw_int3.lock();
                cout<<"p_int3.use_count():"<<p_int3.use_count()<<endl;//2
            }
            cout<<"p_int3.use_count():"<<p_int3.use_count()<<endl;//1
        }

        //优先使用unique_ptr，相比shared_ptr，不支持复制，赋值操作
        void unique_ptr_test(){
            cout<<endl<<"--smart_pointer::custom_var::unique_ptr_test()--"<<endl;
            unique_ptr<string>pu_string1(make_unique<string>("I am a string"));
            
            cout<<"(1)支持移动语义:移动构造函数,移动赋值运算"<<endl;
            unique_ptr<string>pu_string2(std::move(pu_string1));//pu_string1指向的资源转让给了pu_string2，pu_string1置空
            pu_string1=std::move(pu_string2);//pu_string2指向的资源转让给了pu_string1，pu_string2置空

            cout<<"(2)release():放弃对裸指针控制权，将智能指针置空，返回裸指针(可以用来初始化/赋值另一个智能指针，也可以delete释放)"<<endl;
            unique_ptr<string>pu_string3(pu_string1.release());//pu_string1置空，返回了裸指针用于初始化pu_string3
            if(pu_string1==nullptr){
                cout<<"pu_string1被置空"<<endl;
                cout<<"*pu_string3="<<*pu_string3<<endl;
            }

            cout<<"(3)reset():智能指针置空，reset(裸指针参数):释放原智能指针的资源并置空，指向新内存"<<endl;
            pu_string3.reset();
            if(pu_string3==nullptr){
                cout<<"pu_string3被置空，资源被释放"<<endl;
            }

            pu_string1.reset(new string("I am a new string"));
            cout<<"*pu_string1="<<*pu_string1<<endl;
            pu_string2.reset(pu_string1.release());
            cout<<"*pu_string2="<<*pu_string2<<endl;

            cout<<"(4)=nullptr:等价于reset()"<<endl;
            pu_string2=nullptr;
            if(pu_string2=nullptr){
                cout<<"pu_string2被置空，资源被释放"<<endl;
            }

            cout<<"(5)unique_ptr右值转换为shared_ptr"<<endl;
            unique_ptr<string>pu_string4(make_unique<string>("I am a string"));
            shared_ptr<string>ps_string1(std::move(pu_string4));
            if(pu_string4==nullptr){
                cout<<"pu_string4被置空，资源转让给ps_string1"<<endl;
                cout<<"ps_string1.use_count():"<<ps_string1.use_count()<<endl;
            }
            
            cout<<"(6)尺寸：等同于裸指针尺寸"<<endl;
            unique_ptr<string>pu_string5(make_unique<string>("I am a string"));
            cout<<"sizeof(pu_string5)="<<sizeof(pu_string5)<<endl;//8

            //以下等同于shared_ptr，略
            cout<<"(7)get():返回裸指针"<<endl;
            cout<<"(8)*解引用"<<endl;
            cout<<"(9)swap():交换两个智能指针所指向的对象"<<endl;
            cout<<"(10)智能指针作为判断条件"<<endl;
            cout<<"(11)指定删除器:定义数组智能指针建议在<>内加入[],否则要指定删除器"<<endl;
            cout<<"(12)指定分配器(内存分配),暂略"<<endl;
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
            cout<<endl<<"--smart_pointer::classType::test()--"<<endl;
            cout<<"(1)使用智能指针初始化类对象"<<endl;
            //shared_ptr存储着指向数据的指针和指向控制块(引用计数，弱引用计数以及其他数据)的指针
            shared_ptr<Base>p_base1(new Base("p_base1"));//初始化方式一，使用裸指针调用构造函数初始化(不支持隐式转换)
                                                        //此方式需要两次不连续的内存分配，1.new创建数据内存，2.shared_ptr堆上动态创建控制块(用来管理数据)
                                                        //注意：如果使用相同的裸指针创建shared_ptr，会创建两个控制块，导致内存重复释放
            shared_ptr<Base>p_base2(make_shared<Base>("p_base2"));//初始化方式二（推荐)，使用make_shared
                                                        //写成shared_ptr<Base>p_base2=make_shared<Base>()也可以
                                                        //此方式只需要一次连续内存分配(shared_ptr仍包含数据指针和控制块指针)，数据内存和控制块内存在内存空间中连续，所以更高效，异常安全
                                                        //异常不安全：因为C++允许参数传递时候打乱顺序，可能出现new创建数据内存之后没有及时传给shared_ptr而执行了别的代码行导致内存泄露
                                                        //make_shared的缺点：1.无法自定义删除器，2.需要保证构造函数，3.可能造成部分内存锁定
            cout<<"p_base2.use_count():"<<p_base2.use_count()<<endl;
            shared_ptr<Base>p_base3(p_base2);//p_base3指向p_base2的内存空间，该内存空间的引用计数增加
            cout<<"p_base2.use_count():"<<p_base2.use_count()<<endl;
            cout<<"p_base3.use_count():"<<p_base3.use_count()<<endl;
            p_base1=nullptr;
            p_base2=nullptr;
            p_base3=nullptr;

            cout<<"(2)智能指针同样适用多态。shared_ptr指针父类析构不需要写成虚函数，仍可以成功析构子类！(unique_ptr不行)"<<endl;//非智能指针，父类指针要写成虚函数，才能调用父类的析构函数
                                                                                                        //这个特性是因为shared_ptr的控制块里面还存储了指向数据指针的指针(即指向子类指针),所以释放智能指针指向的对象会先执行子类析构函数，再执行父类析构函数
                                                                                                        //然而unique_ptr其实相当于裸指针，不包含控制块，不具有这个特性
            shared_ptr<Base>p_base2extend1(new extend());
            p_base2extend1=nullptr;//成功调用子类析构函数
            shared_ptr<Base>p_base2extend2(make_shared<extend>());
            p_base2extend2=nullptr;//成功调用子类析构函数

            unique_ptr<Base>p_base2extend3(new extend());
            p_base2extend3=nullptr;//父类析构函数不是虚函数，无法调用子类析构函数
            unique_ptr<Base>p_base2extend4(make_unique<extend>());
            p_base2extend4=nullptr;//父类析构函数不是虚函数，无法调用子类析构函数
        }
    }
}


int main(){
    custom_var::test();
    smart_pointer::custom_var::shared_ptr_test();
    smart_pointer::custom_var::weak_ptr_test();
    smart_pointer::custom_var::unique_ptr_test();
    smart_pointer::classType::test();

    return 0;
}