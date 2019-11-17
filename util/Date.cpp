#include "../stdafx.h"
#include "Date.h"


Date::Date()
{
	GetLocalTime(&m_systemTime);
}


Date::~Date()
{
}

WORD Date::GetYear()
{
	return m_systemTime.wYear;
}
WORD Date::GetMonth()
{
	return m_systemTime.wMonth;
}
WORD Date::GetDay()
{
	return m_systemTime.wDay;
}
WORD Date::GetHour()
{
	return m_systemTime.wHour;
}
WORD Date::GetMinute()
{
	return m_systemTime.wMinute;
}
WORD Date::GetSecond()
{
	return m_systemTime.wSecond;
}

WORD Date::GetMilliseconds()
{
	return m_systemTime.wMilliseconds;
}