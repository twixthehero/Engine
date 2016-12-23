#include "Logger.h"
#include <ctime>

Logger* Logger::_instance = nullptr;

void Logger::Init()
{
	if (_instance == nullptr)
		_instance = new Logger();
}

void Logger::Shutdown()
{
	_instance->CloseLog();

	delete _instance;
}

void Logger::Write(std::string message)
{
	_instance->Write(message);
}

void Logger::WriteLine(std::string message)
{
	_instance->WriteLine(message);
}

Logger::Logger()
{
	time_t currentTime = time(0);
	struct tm* timeInfo;
	
	//timeInfo = std::localtime(&currentTime);

	//strftime(_timeBuffer, 80, "%d-%m-%Y_%I-%M-%S", timeinfo);

	//_log = std::ofstream(std::string(_timeBuffer) + ".txt");
}

Logger::~Logger()
{
}

void Logger::CloseLog()
{
	_log.close();
}

void Logger::_Write(std::string message)
{

}

void Logger::_WriteLine(std::string message)
{
	_Write(message + "\n");
}