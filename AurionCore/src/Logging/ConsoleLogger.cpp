module;

#include <cstdarg>
#include <cstdio>
#include <iostream>

module Aurion.Log;

namespace Aurion
{
	ConsoleLogger* ConsoleLogger::Get()
	{
		static ConsoleLogger s_logger;

		return &s_logger;
	}

	ConsoleLogger::ConsoleLogger()
	{
		
	}

	ConsoleLogger::~ConsoleLogger()
	{

	}

	void ConsoleLogger::Log(const LogLevel& verbosity, const char* format, ...)
	{
		// Get required output string size
		va_list args;
		va_start(args, format);
		size_t required_size = vsnprintf(nullptr, 0, format, args);
		va_end(args);

		// Create output buffer
		char* out_buffer = new char[required_size + 1];

		// Apply arguments to message format
		va_start(args, format);
		size_t result = vsnprintf(out_buffer, required_size + 1, format, args);
		va_end(args);

		// Apply null terminator
		out_buffer[required_size] = '\0';

		if (result == 0)
		{
			delete[] out_buffer;
			return;
		}

		switch (verbosity)
		{
			case AC_LOG_LEVEL_TRACE:
			{
				std::cout << c_log_prefix_trace << c_text_color_white << out_buffer << c_text_color_end << std::endl;
				break;
			}
			case AC_LOG_LEVEL_INFO:
			{
				std::cout << c_log_prefix_info << c_text_color_green << out_buffer << c_text_color_end << std::endl;
				break;
			}
			case AC_LOG_LEVEL_WARN:
			{
				std::cout << c_log_prefix_warn << c_text_color_yellow << out_buffer << c_text_color_end << std::endl;
				break;
			}
			case AC_LOG_LEVEL_ERROR:
			{
				std::cout << c_log_prefix_error << c_text_color_red << out_buffer << c_text_color_end << std::endl;
				break;
			}
			case AC_LOG_LEVEL_CRITICAL:
			{
				std::cout << c_log_prefix_critical << c_text_bg_red << c_text_color_white << " " << out_buffer << " " << c_text_color_end << std::endl;
				break;
			}
			default:
			{
				std::cout << c_log_prefix_trace << c_text_color_white << out_buffer << c_text_color_end << std::endl;
				break;
			}
		}

		delete[] out_buffer;
	}
}