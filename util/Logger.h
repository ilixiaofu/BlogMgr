#ifndef _LOGGER_H_
#define _LOGGER_H_

class Logger
{
public:

	static void Info(const char* szFormat, ...);
	//static void Error(const wchar_t* sFormat, ...);
	//static void Debug(const wchar_t* sFormat, ...);

private:
	Logger() = delete;
	~Logger() = delete;
	Logger(const Logger&) = delete;
	Logger(const Logger&&) = delete;
	Logger& operator =(const Logger&) = delete;
	Logger& operator =(const Logger&&) = delete;
};

#endif // !_LOGGER_H_