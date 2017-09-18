#include "stdafx.h"
#include "ProfilerNegev.h"

ProfileSt g_proArr[STRUCT_NUM]; // 저장용 전역 배열

class CInit
{
public:
	CInit()
	{
		for (int i = 0; i < STRUCT_NUM; i++)
		{
			memset(g_proArr[i]._name, 0x00, sizeof(WCHAR) * TEXT_SIZE);
			g_proArr[i]._count = 0;
			g_proArr[i]._max[0] = 0;
			g_proArr[i]._max[1] = 0;
			g_proArr[i]._min[0] = INT64_MAX;
			g_proArr[i]._min[1] = INT64_MAX;
			g_proArr[i]._totalTime.QuadPart = 0;
			g_proArr[i]._totalTime.QuadPart = 0;
		}
	}
};

CInit g_Cinit; // 초기화용

void PRO_BEGIN(WCHAR* pName) // BEGIN
{
	for (int i = 0; i < STRUCT_NUM; i++)
	{
		// 0x00 이면 새로 공간 할당
		if (g_proArr[i]._name[0] == 0x00)
		{
			wcscpy_s(g_proArr[i]._name, TEXT_SIZE, pName);
			QueryPerformanceCounter(&g_proArr[i]._begin);

			break;
		}

		// 기본적으로 이름이 맞지 않으면 continue
		if (wcscmp(g_proArr[i]._name, pName) != 0)
		{
			continue;
		}

		// begin QueryPerformanceCounter 구해서 넣는다.
		QueryPerformanceCounter(&g_proArr[i]._begin);

		break;
	}
}

void PRO_END(WCHAR* pName) // END
{
	for (int i = 0; i < STRUCT_NUM; i++)
	{
		// 이름이 맞지 않으면 continue
		if (wcscmp(g_proArr[i]._name, pName) != 0)
		{
			continue;
		}

		// end QueryPerformanceCounter 구한다.
		LARGE_INTEGER endTime;
		QueryPerformanceCounter(&endTime);

		// end - begin
		__int64 diffTime = endTime.QuadPart - g_proArr[i]._begin.QuadPart;
		g_proArr[i]._totalTime.QuadPart += diffTime;
		// count++
		g_proArr[i]._count++;

		// 1초당 타이머 주파수를 얻는다, min, max
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		
		//cout << ((double)diffTime / freq.QuadPart) / 1000000 << endl;
		wprintf(L"%.4f㎲\n", (double)diffTime / (freq.QuadPart / 1000000));
		
		break;
	}
}

void SaveProfile()
{
	FILE* fp;
	errno_t err = _wfopen_s(&fp, L"PROFILE.txt", L"wb, ccs=UNICODE");
	if (err != 0)
	{
		wprintf(L"FILE OPEN ERROR\n");

		return;
	}

	for (int i = 0; i < STRUCT_NUM; i++)
	{
		if (g_proArr[i]._name[0] == 0x00)
		{
			continue;
		}

		WCHAR tempWchar[200];
		ZeroMemory(tempWchar, sizeof(WCHAR) * 200);

		// 1초당 타이머 주파수를 얻는다, min, max
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);

		// 평균
		double average = (double)g_proArr[i]._totalTime.QuadPart / freq.QuadPart / g_proArr[i]._count;

		// wsprintf 대용
		StringCbPrintf(tempWchar, 200, L"name : %ws, average : %.4f, min : x, max : x, count : %d\n", g_proArr[i]._name, average, g_proArr[i]._count);
		
		// 한줄씩 파일에 쓰기
		fputws(tempWchar, fp);
	}
}

int main()
{
	setlocale(LC_ALL, "");

	for(int i = 0; i < STRUCT_NUM; i++)
	{
		PRO_BEGIN(L"abc");
		PRO_END(L"abc");
	}

	PRO_BEGIN(L"kkk");
	Sleep(2345);
	PRO_END(L"kkk");

	for (int i = 0; i < STRUCT_NUM; i++)
	{
		if (g_proArr[i]._name[0] == 0x00)
		{
			continue;
		}

		wprintf(L"%d번\n", g_proArr[i]._count);

		// 1초당 타이머 주파수를 얻는다, min, max
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);

		// 평균
		wprintf(L"평균 : %.4f\n", (double)g_proArr[i]._totalTime.QuadPart / freq.QuadPart / g_proArr[i]._count);
	}

	SaveProfile();

    return 0;
}