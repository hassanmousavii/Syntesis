
#include <iostream>
#include<list>
#include<set>
#include<string.h>
#include<iterator>
#include<stdio.h>
#include<math.h>
#include <unistd.h>
using namespace std;
#if !defined(MY_APP__VARS_DEFINED)
 //int var1, var2 = 0;
extern set<std::pair<int, int>> EdgesSet;
 static list<string> varTrans;
 extern list<int> domainsSizes;
 static list<string> varsNames;
#define MY_APP__VARS_DEFINED
#endif
