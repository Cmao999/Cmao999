#include<iostream>
#include<cstring>

using namespace std;

namespace function{
    //(1)类型模板参数函数
    //编译器支持类型推导，不用显式指出模板参数
    template<typename T>
    T add1(T a,T b){
        return a+b;
    }

    //(2)非类型模板参数函数(固定类型常量)
    //参数只支持常量表达式，由用户提供或编译器推导
    //参数类型只支持整型、指针、引用
    //浮点数float和double,字符串，类不能作为参数类型
    template<int a,int b>
    int add2(){
        return a+b;
    }

    template<unsigned L1,unsigned L2>
    int chars_comp(char const (&p1)[L1],char const (&p2)[L2]){
        return strcmp(p1,p2);
    }

    //(3)混合使用类型模板参数和非类型模板参数
    template<typename T,int a,int b>
    T add3(T t){
        return t+a+b;
    }

    void test(){
        cout<<"--function--"<<endl;
        cout<<"(1)类型模板参数函数"<<endl;
        cout<<"add1(1,2) = "<<add1(1,2)<<endl;//编译器推断出add1<int>(int a,int b)
        int a=1,b=2;
        cout<<"add1(a,b) = "<<add1(a,b)<<endl;
        cout<<"add1(1.1,2.2) = "<<add1(1.1,2.2)<<endl;
        

        cout<<"(2)非类型模板参数函数(固定类型常量)"<<endl;
        cout<<"add2<1,2>() = "<<add2<1,2>()<<endl;//需显式给出非类型模板参数的值，编译器无法推断
        cout<<"chars_comp(char const (&p1)[L1],char const (&p2)[L2]) = "<<chars_comp("test1","test2")<<endl;//由编译器推断模板参数的值

        cout<<"(3)混合使用类型模板参数和非类型模板参数"<<endl;
        cout<<"add3<int,1,2>(3) = "<<add3<int,1,2>(3)<<endl;
    }   
}

namespace classType{
    //类模板不支持参数推导
    //我的vector容器，只支持一般类型，不支持类类型
    template<typename T>
    class myvector{
    public:
        typedef T* iterator;
    
    protected:
        iterator start;//使用空间头
        iterator finish;//使用空间尾
        iterator end_of_storage;//总可用空间尾

    public:
        iterator begin(){return start;}
        iterator end(){return finish;}
        size_t size(){return size_t(end()-begin());}
        size_t capacity(){return size_t(end_of_storage-begin());}
        void push_back(const T& val){//T val会拷贝一份，效率低，T& val只能接收左值。所以使用const T& val，可以接收左值和右值，也不会拷贝。
            //将元素插入尾部
            if(finish!=end_of_storage){
                *finish=val;
                ++finish;
            }
            //容量满,扩容为(原大小+1)*2
            else{
                int original_size=size();
                int new_size=(original_size+1)*2;
                iterator new_start=new T[new_size];
                memcpy(new_start,begin(),original_size*sizeof(T));
                delete[] begin();
                start=new_start;
                finish=start+original_size;
                end_of_storage=start+new_size;
            }
        }

        void pop_back(){
            --finish;
        }
    
    public:
        myvector():start(0),finish(0),end_of_storage(0){}
        myvector(int n):start(new T[n]),finish(start),end_of_storage(start+n+1){}//初始化顺序为申明变量的顺序，非此处的顺序
        ~myvector(){
            delete[] begin();
        }
    };
    
    void test(){
        cout<<"--classType--"<<endl;
        myvector<int>v_int;
        myvector<double>v_double;
        myvector<char>v_char;
        for(int i=0;i<10;i++){
            v_int.push_back(i);
            v_double.push_back(i*1.1);
            v_char.push_back(i-1+'a');
        }

        for(auto itor=v_int.begin();itor!=v_int.end();itor++)
            cout<<*itor<<" ";
        cout<<endl;
        for(auto itor=v_double.begin();itor!=v_double.end();itor++)
            cout<<*itor<<" ";
        cout<<endl;
        for(auto itor=v_char.begin();itor!=v_char.end();itor++)
            cout<<*itor<<" ";
        cout<<endl;
    }

}

int main(){
    function::test();
    classType::test();
}