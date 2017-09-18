#pragma once

#include <windows.h>

#define STRUCT_NUM 30
#define TEXT_SIZE 20

struct ProfileSt
{
	WCHAR _name[TEXT_SIZE];
	LARGE_INTEGER _totalTime;
	LARGE_INTEGER _begin;
	__int64 _min[2];
	__int64 _max[2];
	int _count;
};
