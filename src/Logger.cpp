#include "Logger.h"
#include <ctime>
#include <sys\types.h>
#include <sys\stat.h>
#include <iostream>

#if defined _WIN32
#include <direct.h>
std::string Logger::PATH_SEPARATOR = "\\";
#else
std::string Logger::PATH_SEPARATOR = "/";
#endif

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
	_instance->_Write(message);
}

void Logger::WriteLine(std::string message)
{
	_instance->_WriteLine(message);
}

Logger::Logger()
{
	struct stat info;
	int createError = 0;

	if (stat("logs", &info) != 0)
	{
		_Write("logs/ does not exist...creating...");

		const char* path = "logs";

#if defined _WIN32
		createError = _mkdir(path);
#else
		mode_t mode = 0733;
		createError = mkdir(path, mode);
#endif

		if (createError != 0)
			_WriteLine("Error creating logs/ directory: " + std::string(strerror(errno)));
		else
			_WriteLine("success!");
	}
	else if (info.st_mode & S_IFDIR)
	{
		_WriteLine("logs/ already exists...skipping creation");
	}
	else
	{
		_WriteLine("logs/ is not a directory!");
		createError = -1;
	}

	_initialized = createError == 0;

	if (_initialized)
	{
		time_t currentTime = time(0);
		struct tm timeInfo;

		localtime_s(&timeInfo, &currentTime);

		strftime(_timeBuffer, 80, "%d-%m-%Y_%I-%M-%S", &timeInfo);

		_log = std::ofstream("logs" + PATH_SEPARATOR + std::string(_timeBuffer) + ".txt");
		_WriteLine("Created logfile");
	}
	else
		_WriteLine("Failed to logfile");
}

Logger::~Logger()
{
}

void Logger::SetUseStdOut(bool use)
{
	_instance->_useStdout = use;
}

void Logger::CloseLog()
{
	if (_initialized)
		_log.close();
}

void Logger::_Write(std::string message)
{
	if (_initialized)
		_log << message;
	
	if (_useStdout)
		std::cout << message;
}

void Logger::_WriteLine(std::string message)
{
	_Write(message + "\n");
}