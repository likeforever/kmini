// template.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <type_traits>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;



template<class _pg>
class ProgressBase
{
public:
        void print(_pg pg){
                cout << pg << endl;
                m_pg = pg;
        }
protected:
        _pg m_pg;
};

template<class _pg, class _placeHolder = void>
class Progr : public ProgressBase<_pg>
{
public:
private:
}; 

template<class _pg>
class Progr<_pg, typename std::enable_if< std::is_integral<_pg>::value || std::is_floating_point<_pg>::value  >::type> : public ProgressBase<_pg>
{
public:
        void inc(){
                m_pg++;
        }

        void justPrint(){
                cout << m_pg << endl;
        }
private:
};



int _tmain(int argc, _TCHAR* argv[])
{
        //1. 通过继承的方式，让类特化时不用copy所有代码
        //2. 利用enable_if来为interge和float型增加新函数
        //3. 使用了偏特化和点位的模板类型形参来达到此目标
        Progr<string> sp;
        sp.print("x");

        Progr<int> sp2;
        sp2.print(23);
        sp2.inc();
        sp2.justPrint();

        Progr<float> sp3;
        sp3.print(23.4f);
        sp3.inc();
        sp3.justPrint();

        return 0;
}



