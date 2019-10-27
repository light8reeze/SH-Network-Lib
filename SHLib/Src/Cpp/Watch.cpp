#include "Watch.h"
#include <ctime>

namespace SHLib
{
	DWORD CWatch::GetTime()
	{
		static DWORD dwStartTime = GetTickCount();

		return GetTickCount() - dwStartTime;
	}

	//현재 시간을 반환하는 함수
	CDate CDate::GetNowDate()
	{
		time_t	tTime;
		CDate	date;
		struct tm tmDate;

		tTime	= std::time(nullptr);
		localtime_s(&tmDate, &tTime);
		
		date.wYear		= tmDate.tm_year + 1900;
		date.wMonth		= tmDate.tm_mon + 1;
		date.wWeekDay	= tmDate.tm_wday;
		date.wDay		= tmDate.tm_mday;
		date.wHour		= tmDate.tm_hour;
		date.wMinute	= tmDate.tm_min;
		date.wSecond	= tmDate.tm_sec;

		return date;
	}

	//값을 넣으면 Date구조체로 만들어 주는 함수
	CDate CDate::CreateDate(WORD _wYear, WORD _wMonth, WORD _wDay, WORD _wHour, WORD _wMinute, WORD _wSecond)
	{
		CDate date;
		date.wYear		= _wYear;
		date.wMonth		= _wMonth;
		date.wDay		= _wDay;
		date.wHour		= _wHour;
		date.wMinute	= _wMinute;
		date.wSecond	= _wSecond;

		int t[] = { 0,3,2,5,0,3,5,1,4,6,2,4 };

		_wYear -= _wMonth < 3;

		//요일계산
		date.wWeekDay = (_wYear + _wYear / 4 - _wYear / 100 + _wYear / 400 + t[_wMonth - 1] + _wDay) % 7;

		return date;
	}

	//두 날짜를 비교하여 같은지 반환하는 함수
	//_eFlag에 날짜 조건을 넣는다(ex : day, hour만 같으면 true => (_eFlag = E_DATE_DAY | E_DATE_HOUR))
	BOOL CDate::CompareDate(CDate& _date1, CDate& _date2, char _cFlag)
	{
		char cBit		= 0;
		int nCount		= 0;
		char cFlag		= _cFlag;
		WORD* pDate1	= nullptr;
		WORD* pDate2	= nullptr;

		if (cFlag > E_DATE_ALL)
			cFlag = E_DATE_ALL;

		while (cFlag > 0)
		{
			cBit = cFlag & 1;

			if (cBit == 1)
			{
				pDate1 = ((WORD*)&_date1) + (6 - nCount);
				pDate2 = ((WORD*)&_date2) + (6 - nCount);

				if ((*pDate1) != (*pDate2))
					return FALSE;
			}

			++nCount;
			cFlag = cFlag >> 1;
		}

		return TRUE;
	}
}