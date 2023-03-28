#include<iostream>

using namespace std;

namespace custom_var{
    //const关键字,a的值不可修改
    const string a="---custom_var---";
    
    //constexpr关键字,常量表达式，在编译时就求值,提升性能
    constexpr int b=1*2*3*4;
    constexpr int func1(int x){
        x=x*10;
        return  x;
    }
    constexpr int c=11*func1(11);
}

namespace ptr_var{
    void test(){
        cout<<"---ptr_var---"<<endl;

        char str1[]="strone";
        char str2[]="strtwo";

        //(1)const char* p
        //const在指针标识外部，表示指针指向的对象的值不能更改,但可以更改指针的值
        cout<<"(1)const char* p"<<endl;
        const char* pa=str1;
        cout<<pa<<endl;
        pa=str2;
        cout<<pa<<endl;

        //(2)char const* p
        //等价于(1)
        cout<<"(2)char const* p等价于(1)"<<endl;

        //(3)char* const p
        //const在指针标识内部，表示指针本身的值不能更改，即不能更改指针指向，但可以更改指针指向的对象的值
        cout<<"(3)char* const p"<<endl;
        char* const pb=str2;
        cout<<pb<<endl;
        for(int i=0;i<sizeof(str2)-1;i++){//sizeof可以计算数组的内存大小(bit)，如果计算指针可能会算成指针的大小(一般为4bit)
            *(pb+i)=*(pb+i)-'a'+'A';
        }
        cout<<pb<<endl;
    
        //(4)const char* const p或const char const* p
        //指针的值不能更改，指针指向的对象的值也不能更改
        cout<<"(4)const char* const p或const char const* p, 指针的值不能更改，指针指向的对象的值也不能更改"<<endl;
    }
}

namespace funType{
    int val=10;

    //const参数类型函数，不能修改参数值
    int get_val(const int val){
        cout<<"(1)const参数类型函数，不能修改参数值"<<endl;
        return val;
    }

    //const返回类型，返回一个const常量,一般用于指针或配合引用&使用
    //const& int返回类型，表示不能修改函数调用后的返回值
    const int & get_val_rc(int &val){
        cout<<"(2)const& int返回类型,表示不能修改函数调用后的返回值"<<endl;
        val++;
        return val;
    }

    //const* int返回类型,表示不能修改返回后指针指向的对象的值
    const int * get_p(int* p){
        cout<<"(3)const* int返回类型,表示不能修改返回后指针的对象的值"<<endl;
        (*p)++;
        return p;
    }
    
    void test(){
        cout<<"---function---"<<endl;
        cout<<get_val(val)<<endl;
        cout<<get_val_rc(val)<<endl;
        cout<<*get_p(&val)<<endl;
    }
}

namespace classType{
    class Base{
    private:
        string id;
        mutable string mutable_id;//mutable申明，与const成员函数对立，可被const成员函数修改
        
    public:
        Base():id("Base"),mutable_id("mutable_id:1"){}
        Base(string id,string mutable_id):id(id),mutable_id(mutable_id){}
             
        void set_id(const string _id){
            id=_id;
        }

        //const成员函数,不能修改的成员变量(除mutable外)
        void get_id()const{
            cout<<"this is \""<<id<<"\""<<endl;
        }

        //const成员函数,不能修改的成员变量(除mutable外)
        void change_mutable()const{
            cout<<mutable_id<<endl;
            mutable_id="mutable_id:2";
            cout<<mutable_id<<endl;
        }
    };

    void test(){
        cout<<"---classType---"<<endl;
        //(1)一般对象
        cout<<"(1)一般对象"<<endl;
        Base object;
        object.get_id();
        object.set_id("一般对象");
        object.get_id();
        object.change_mutable();

        //(2)const对象，不能修改对象的状态(除mutable外)
        //只能调用const成员函数
        cout<<"(2)const对象，不能修改对象的状态, 只能调用const成员函数"<<endl;
        const Base const_object;
        const_object.get_id();
    }
}


int main(){
    cout<<custom_var::a<<endl<<custom_var::b<<" "<<custom_var::c<<endl;
    ptr_var::test();
    funType::test();
    classType::test();
    return 0;
}