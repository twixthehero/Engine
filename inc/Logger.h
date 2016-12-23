#pragma once
#include <string>
#include <fstream>

class Logger
{
public:
	static void Init();
	static void Shutdown();

	static void Write(std::string message);
	static void WriteLine(std::string message);
private:
	Logger();
	~Logger();

	void CloseLog();
	void _Write(std::string message);
	void _WriteLine(std::string message);

	static Logger* _instance;

	char _timeBuffer[80];
	std::ofstream _log;
};

