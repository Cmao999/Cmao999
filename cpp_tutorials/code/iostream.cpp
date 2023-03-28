#include<iostream>
#include <iomanip>

using namespace std;

namespace io_test{
    void cout_test(){
        cout<<"--cout_test--"<<endl;
         //易忽略的转义字符
        cout<<"反斜杠:"<<"\\"<<endl;
        cout<<"单引号:"<<"\'"<<endl;
        cout<<"双引号:"<<"\""<<endl;

        int n = 141;
        //1) 分别以十六进制、十进制、八进制先后输出 n
        cout << "1)" << hex << n << " " << dec << n << " " << oct << n << endl;
        double x = 1234567.89, y = 12.34567;
        //2)保留5位有效数字
        cout << "2)" << setprecision(5) << x << " " << y << " " << endl;
        //3)保留小数点后面5位
        cout << "3)" << fixed << setprecision(5) << x << " " << y << endl;
        //4)科学计数法输出，且保留小数点后面5位
        cout << "4)" << scientific << setprecision(5) << x << " " << y << endl;
        //5)非负数显示正号，输出宽度为12字符，宽度不足则用 * 填补
        cout << "5)" << showpos << fixed << setw(12) << setfill('*') << 12.1 << endl;
        //6)非负数不显示正号，输出宽度为12字符，宽度不足则右边用填充字符填充
        cout << "6)" << noshowpos << setw(12) << left << 12.1 << endl;
        //7)输出宽度为 12 字符，宽度不足则左边用填充字符填充
        cout << "7)" << setw(12) << right << 12.1 << endl;
        //8)宽度不足时，负号和数值分列左右，中间用填充字符填充
        cout << "8)" << setw(12) << internal << -12.1 << endl;
        cout << "9)" << 12.1 << endl;
    }

    void cin_test(){
        cout<<"--cin_test--"<<endl;
        char a;
        int b;
        float c;
        string d;
        cin>>a>>b>>c;
        cout<<a<<" "<<b<<" "<<c<<" "<<endl;
        getline(cin,d);//此时缓冲区还有空格，读取
        getline(cin,d);//此函数可读取整行，包括前导和嵌入的空格
        cout<<d<<endl;
    }
}

int main(){
    io_test::cout_test();
    io_test::cin_test();
    return 0;
}