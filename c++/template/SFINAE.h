#pragma  once

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

class RtObj
{
public:
        RtObj(int ii, int jj){
                i = ii;
                j = jj;
        }
        int i;
        int j;
};


static const int s_sfMode1 = 1; 
static const int s_sfMode2 = 2;
static const int s_sfMode3 = 3;
template<int _mode, typename _ret>
class MemberSfinae {
public:
        template<int _m>
        _ret* init(typename enable_if<_m==s_sfMode1, void*>::type x = NULL){
                printf("go mode1\n");
                _ret* t= new _ret(_m, 1);
                return t;
        }


        template<int _m>
        _ret* init(typename enable_if<_m==s_sfMode2, void*>::type x = NULL){
                printf("go mode2\n");
                return (_ret*)new char[sizeof(_ret)];
        }

        template<int _m>
        _ret* init(typename enable_if<_m==s_sfMode3, void*>::type x = NULL){
                printf("go mode3\n");
                return NULL;
        }

        _ret* get(){
                _ret* t = init<_mode>();
                return t;
        }
};


class TestSfinae
{
public:
        static void testClass(){
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
        }

        static void testMemberFunction(){
                MemberSfinae<s_sfMode1, RtObj> m1;
                MemberSfinae<s_sfMode2, int> m2;
                MemberSfinae<s_sfMode3, void> m3;
                RtObj * k = m1.get();
                int * k2 = m2.get();
                void * k3 = m3.get();

        }
private:
};
