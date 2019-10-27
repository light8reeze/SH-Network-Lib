#include "Watch.h"
#include <ctime>

namespace SHLib
{
	DWORD CWatch::GetTime()
	{
		static DWORD dwStartTime = GetTickCount();

		return GetTickCount() - dwStartTime;
	}

	//���� �ð��� ��ȯ�ϴ� �Լ�
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

	//���� ������ Date����ü�� ����� �ִ� �Լ�
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

		//���ϰ��
		date.wWeekDay = (_wYear + _wYear / 4 - _wYear / 100 + _wYear / 400 + t[_wMonth - 1] + _wDay) % 7;

		return date;
	}

	//�� ��¥�� ���Ͽ� ������ ��ȯ�ϴ� �Լ�
	//_eFlag�� ��¥ ������ �ִ´�(ex : day, hour�� ������ true => (_eFlag = E_DATE_DAY | E_DATE_HOUR))
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