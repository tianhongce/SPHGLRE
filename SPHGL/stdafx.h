#ifndef STDAFX_H
#define STDAFX_H

#include<cstdio>
#include<cstring>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<cmath>
#include<vector>
#include<set>
#include<map>
#include<ctime>
#include<queue>
#include<string>
#include<sstream>

const double PI = acos(-1.0);
const double EPS = 1e-10;

using namespace std;

#define IS_SURFACE 0
#define	IS_DIS_FIELD 1
#define	IS_PARTICLE 2
#define	IS_HASH_GRID 3
#define	IS_OUTPUT_FRAME 4


class Param
{
public:
	Param();
	~Param();

public:

	//初始控制参数
	static bool ctrlParam[10];
	static void initParam();
};


#endif