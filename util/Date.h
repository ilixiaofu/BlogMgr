#ifndef _DATE_H_
#define _DATE_H_

#include <windows.h>

class Date
{
public:
	Date();
	~Date();
	WORD GetYear();
	WORD GetMonth();
	WORD GetDay();
	WORD GetHour();
	WORD GetMinute();
	WORD GetSecond();
	WORD GetMilliseconds();
	
private:
	SYSTEMTIME m_systemTime;
};

#endif
