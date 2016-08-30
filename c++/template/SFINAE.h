#pragma  once

#include <type_traits>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

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

//用户使用时，指定第一个模板参数，不指定第二个模板参数（默认为void）。根据这两个参数，在特化中找是否有最佳匹配，于是下面特化版本才有可能选中
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



template < typename T >
struct yes_t
{ int yes; };

typedef char no_t;

template < typename T >
struct isClass
{
private:
        //1. 这里只有声明， 因为用不到定义，link是就不会用
        //2. value是static， 且sizeof求值, 所以这个函数也要是Static
        template < typename U >
        static yes_t  < int U::* > selector(U); 

        static no_t selector(...);

        static T* this_t();

public:
        static const bool value =
                sizeof(selector(*this_t())) != sizeof(no_t); 
};

class find1{
};

class find2
{
public:
        int find(const string& key){
                return 0;
        }
private:
};


template < typename T >
struct hasFind
{
private:
        template < int (T::*)(const string&) >
        struct yes_t
        { int yes; };


        //1. 这里只有声明， 因为用不到定义，link是就不会用
        //2. value是static， 且sizeof求值, 所以这个函数也要是Static
        //3. &U::find都出来的是一个函数指针，而不是一个类型。所以yes_t要用非类型模板
        template < typename U >
        static yes_t  < &U::find > selector(U);  

        static no_t selector(...);

        static T* this_t();

public:
        static const bool value =
                sizeof(selector(*this_t())) != sizeof(no_t); 
};

template<typename T1, typename T2>
class isSame{
public:
        static const int value = 1;
};

template<typename T>
class isSame<T, T>{
public:
        static const int value = 2;
};

template< typename _T >
class isSame<_T*, _T* >{
public:
        static const int value = 3;
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

        static void testTrait(){
                cout << isClass< vector<int> >::value << endl;
                cout << hasFind< vector<int> >::value << endl;
                cout << hasFind< find2 >::value << endl;

                cout << isSame<int, string>::value << endl;
                cout << isSame<int, int>::value << endl;
                cout << isSame<int*, int*>::value << endl;
        }
private:
};
