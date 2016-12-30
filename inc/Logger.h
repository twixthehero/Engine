#pragma once
#include <string>
#include <fstream>

namespace VoxEngine
{
	class Logger
	{
	public:
		static void Init();
		static void Shutdown();

		static void SetUseStdOut(bool use);

		static void Write(std::string message);
		static void WriteLine(std::string message);
	private:
		Logger();
		~Logger();

		void CloseLog();
		void _Write(std::string message);
		void _WriteLine(std::string message);

		static Logger* _instance;

		static std::string PATH_SEPARATOR;

		char _timeBuffer[80];
		bool _initialized;
		bool _useStdout = true;
		std::ofstream _log;
	};
}
