// setlocale.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <iomanip>
#include <string>
#include <codecvt>
#include <windows.h>
#include <assert.h>
#include <vector>
using namespace std;

void test_utf8(){
        std::wstring str = L"123,我是谁？我爱钓鱼岛！";

        //c++11新加入的特性
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

        std::string narrowStr = conv.to_bytes(str);
        {
                std::ofstream ofs ("test.txt");
                cout << errno << endl;
                assert(ofs.is_open());
                ofs << narrowStr;
        }

        std::wstring wideStr = conv.from_bytes(narrowStr);
        {
                std::locale::global(std::locale("Chinese-simplified"));
                std::wofstream ofs (L"testW.txt");
                ofs << wideStr;
        }
}

void testFacet(){
        typedef std::codecvt<wchar_t,char,std::mbstate_t> facet_type;


        std::locale mylocale("");

        const facet_type& myfacet = std::use_facet<facet_type>(mylocale);

        const char mystr[] = "我是中国人ll测试";

        //下面这样转换无法成功, widen只是一个简单的转换宽窄字符，无法进行编码页处理
        vector<wchar_t> wd;
        wd.resize(sizeof(mystr));
        std::use_facet< std::ctype<wchar_t> >(mylocale).widen(mystr,  mystr+sizeof(mystr), &wd[0]);
        wstring ws(&wd[0]);
        std::wcout.imbue(mylocale);

        // prepare objects to be filled by codecvt::in :
        wchar_t pwstr[sizeof(mystr)];              // the destination buffer (might be too short)
        //mbstate_t不用管它， 这个可能是facet同部需要的，使用者不用关心
        std::mbstate_t mystate = std::mbstate_t(); // the shift state object
        const char* pc;                            // from_next
        wchar_t* pwc;                              // to_next

        // translate characters:
        facet_type::result myresult = myfacet.in (mystate,
                mystr, mystr+sizeof(mystr), pc,
                pwstr, pwstr+sizeof(mystr), pwc);

        assert( myresult == facet_type::ok );
        std::wcout << L"Translation successful: ";
        std::wcout << pwstr << std::endl;

        char mystr2[50] = {0};
        const wchar_t* pwc2;
        char *pc2;
        myresult = myfacet.out ( mystate,pwstr, pwstr+sizeof(mystr), pwc2, mystr2, mystr2+sizeof(mystr), pc2 );
        assert( myresult == facet_type::ok );
        cout << mystr2 << endl;

        //windows不支持utf8要报错
         //std::locale mylocale2("en_US.UTF8");
         std::locale mylocale2;
         //虽然msdn，和c++11标准说下面这种facet能进行utf8转换，但是实测windows 2012是不行的。
        typedef std::codecvt<char16_t, char, mbstate_t> utf16to8_facet_type;
        const utf16to8_facet_type& utf16To8 = std::use_facet<utf16to8_facet_type>(mylocale2);
        const char16_t* pwc3;
        memset(mystr2, 0, sizeof(mystr));
        myresult = utf16To8.out ( mystate,(const char16_t*)pwstr, (const char16_t*)pwstr+sizeof(mystr), (const char16_t*)pwc3, mystr2, mystr2+sizeof(mystr), pc2 );
        assert( myresult == facet_type::ok );
        cout << mystr2 << endl;

}

int _tmain(int argc, _TCHAR* argv[])
{
        test_utf8();
        testFacet();
        //获取当前线程环境的locale类型，即a2w这些宏及其它系统函数遵守的规则
        WORD languageID = LOWORD(GetKeyboardLayout(0));
        char szLCData[6+1];
        GetLocaleInfoA(MAKELCID(languageID, SORT_DEFAULT), LOCALE_IDEFAULTANSICODEPAGE,
                szLCData, _countof(szLCData));
        int codepage = atoi(szLCData);
        cout << codepage << endl;

        //设置当前的locale和操作系统本地保持一致
        setlocale(LC_ALL, "");
        cout << "LC_CTYPE: " << setlocale(LC_ALL, NULL) << endl;


        //设置当前local为cp932, 这样就能在标准输出日文932的fn中保存的字符。当然也可以使用mbstowcs进行cp932到unicode的转换了
        char* lc = setlocale( LC_ALL, "Japanese_Japan.932" );
        cout << "LC_CTYPE: " << setlocale(LC_CTYPE, NULL) << endl;
        char *fn = "\x65\x3a\x5c\x96\xf3\x83\x54\x81\x5b";
        cout << fn << endl;

        WCHAR wstr[512] = {0};
        mbstowcs(wstr, fn, 50);

        //因为使用的是unicode, 所以操作系统显示的是正常的文件名
        HANDLE h2 = CreateFile(wstr, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

        //因为fn中保持的是cp932的字符，而操作系统本地使用的是cp936的locale，所以创建为乱码
        HANDLE h = CreateFileA(fn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
        cout << GetLastError() << endl;
        
        //设置locale为cp936的
        setlocale( LC_ALL, "Chinese_China.936" );
        return 0;
}

