#include <cstdarg>
#include <cstdio>
#include <cassert>

#include <iostream>

import Aurion.Log;
import Aurion.FileSystem;

namespace Aurion
{
	FileLogger* FileLogger::Get()
	{
		if (!s_file_system)
		{
#ifdef AURION_PLATFORM_WINDOWS
			s_file_system = new WindowsFileSystem();
#endif
		}

		static FileLogger s_logger(s_file_system, "temp/debug_output.txt");

		return &s_logger;
	}

	FileLogger::FileLogger()
	{

	}

	FileLogger::FileLogger(IFileSystem* fs, const char* out_file_path)
	{
		m_log_file.Register(fs, out_file_path, true);
	}

	FileLogger::~FileLogger()
	{
		
	}

	void FileLogger::Log(const LogLevel& verbosity, const char* format, ...)
	{
		size_t prefix_size = 0;
		switch (verbosity)
		{
		case LL_TRACE: prefix_size = strlen(c_log_prefix_trace); break;
		case LL_INFO: prefix_size = strlen(c_log_prefix_info); break;
		case LL_WARN: prefix_size = strlen(c_log_prefix_warn); break;
		case LL_ERROR: prefix_size = strlen(c_log_prefix_error); break;
		case LL_CRITICAL: prefix_size = strlen(c_log_prefix_critical); break;
		default: prefix_size = 0;
		}

		// Get required formatted string size
		va_list args;
		va_start(args, format);
		int formatted_size = vsnprintf(nullptr, 0, format, args);
		va_end(args);

		// Create output buffer
		char* out_buffer = new char[prefix_size + formatted_size + 1];

		// Copy prefix
		switch (verbosity)
		{
		case LL_TRACE: strncpy_s(out_buffer, prefix_size + 1, c_log_prefix_trace, prefix_size); break;
		case LL_INFO: strncpy_s(out_buffer, prefix_size + 1, c_log_prefix_info, prefix_size); break;
		case LL_WARN: strncpy_s(out_buffer, prefix_size + 1, c_log_prefix_warn, prefix_size); break;
		case LL_ERROR: strncpy_s(out_buffer, prefix_size + 1, c_log_prefix_error, prefix_size); break;
		case LL_CRITICAL: strncpy_s(out_buffer, prefix_size + 1, c_log_prefix_critical, prefix_size); break;
		default: break;
		}

		// Apply arguments to message format
		va_start(args, format);
		int result = vsnprintf(out_buffer + prefix_size, formatted_size + 1, format, args);
		va_end(args);

		if (result < 0)
			return;

		//out_buffer[prefix_size + formatted_size] = '\0';
		
		// Write to file
		m_log_file.Write(out_buffer, prefix_size + formatted_size, -1);

		delete[] out_buffer;
	}

	/* WORKING EXAMPLE
	void FileLogger::Log(const LogLevel& verbosity, const char* format, ...)
	{
		// Get required formatted string size
		va_list args;
		va_start(args, format);
		int required_size = vsnprintf(nullptr, 0, format, args);
		va_end(args);

		// Create output buffer
		char* out_buffer = new char[required_size + 1];

		// Apply arguments to message format
		va_start(args, format);
		int result = vsnprintf(out_buffer, required_size + 1, format, args);
		va_end(args);

		if (result < 0)
			return;

		out_buffer[required_size] = '\0';
		
		// Write to file
		m_log_file.Write(out_buffer, required_size, -1);

		delete[] out_buffer;
	}
	*/
}