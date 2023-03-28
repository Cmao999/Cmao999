#include<iostream>

//使用命名空间std
//一般来说，最好不要这样用，有可能出现命名重复的问题
using namespace std;

//定义命名空间N1
namespace N1{
    string n1="命名空间N1";
    void test(){
        cout<<n1<<",N1::test()执行"<<endl;
    }
    //命名空间嵌套
    namespace N2{
        string n2="命名空间N2";
        void test(){
            cout<<n2<<",N2::test()执行"<<endl;
        }
    }
}

//同一工程允许多个同名命名空间
//最终会合并到一起
namespace N1{
    string get_str(string str){
        return str;
    }
}

//命名空间使用
int main(){
    N1::test();
    N1::N2::test();
    cout<<N1::get_str(N1::n1)<<endl;
    string str="函数体main中的str";
    cout<<N1::get_str(str)<<endl;

    return 0;
}