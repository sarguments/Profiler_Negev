#pragma once

#include <windows.h>
#include <Strsafe.h>
#include <time.h>  
#include <locale>

#define STRUCT_NUM 30
#define TEXT_SIZE 20

#define GIRLS_FRONTLINE

#ifdef GIRLS_FRONTLINE
#define P_BEGIN(X) PRO_BEGIN(X)
#define P_END(X) PRO_END(X)
#define P_SAVE() SaveProfile()
#else
#define P_BEGIN(X)
#define P_END(X)
#define P_SAVE()
#endif

struct ProfileSt
{
	WCHAR _name[TEXT_SIZE];
	LARGE_INTEGER _totalTime;
	LARGE_INTEGER _begin;
	__int64 _min[2];
	__int64 _max[2];
	int _count;
};

extern void PRO_END(WCHAR* pName);
extern void PRO_BEGIN(WCHAR* pName);
extern void SaveProfile();
extern ProfileSt g_proArr[STRUCT_NUM];
extern double g_freq;