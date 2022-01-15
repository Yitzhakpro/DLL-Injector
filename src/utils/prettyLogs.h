#pragma once

namespace pretty_logs
{
	void question(const char* message, bool end_with_newline = false);

	void info(const char* message, bool end_with_newline = true);

	void success(const char* message, bool end_with_newline = true);

	void error(const char* message, bool end_with_newline = true);
}