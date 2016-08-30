// template.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <type_traits>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>
#include "SFINAE.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
        TestSfinae::testClass();
        TestSfinae::testMemberFunction();
        TestSfinae::testTrait();

        return 0;
}



