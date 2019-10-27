#pragma once
#include "SHLib.h"

namespace SHLib
{
	enum E_TIME
	{
		E_TICKPERSECOND = 1000
	};

	enum E_DATE_FLAG : BYTE
	{
		E_DATE_NONE		= 0,
		E_DATE_SECOND	= 1,
		E_DATE_MINUTE	= 1 << 1,
		E_DATE_HOUR		= 1 << 2,
		E_DATE_DAY		= 1 << 3,
		E_DATE_WEEKDAY	= 1 << 4,
		E_DATE_MONTH	= 1 << 5,
		E_DATE_YEAR		= 1 << 6,
		E_DATE_ALL		= 1 << 7
	};

	class SH_EXPORT CDate
	{
	public:
		static CDate	CreateDate(WORD _wYear, WORD _wMonth, WORD _wDay, WORD _wHour, WORD _wMinute, WORD _wSecond);
		static BOOL		CompareDate(CDate& _date1, CDate& _date2, char _cFlag);
		static CDate	GetNowDate();

	public:
		WORD wYear;		//Year
		WORD wMonth;	//Start Since Jan (1~12)
		WORD wWeekDay;	//Start Since Sunday (0 ~ 6)
		WORD wDay;		//Day (1~31)
		WORD wHour;		//Hour (0 ~ 23)
		WORD wMinute;	//Minute (0 ~ 59)
		WORD wSecond;	//Second (0 ~ 59)
	};
	
	//시간 관련 기능 추후 구현
	class SH_EXPORT CWatch
	{
	public:
		static DWORD	GetTime();
	};
}