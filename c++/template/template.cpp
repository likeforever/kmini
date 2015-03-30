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
        //1. ͨ���̳еķ�ʽ�������ػ�ʱ����copy���д���
        //2. ����enable_if��Ϊinterge��float�������º���
        //3. ʹ����ƫ�ػ��͵�λ��ģ�������β����ﵽ��Ŀ��
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



