#include "prettyLogs.h"
#include <sstream>
#include <iostream>

#define CONSOLE_GREEN "\033[32m"
#define CONSOLE_YELLOW "\033[33m"
#define CONSOLE_RED "\033[31m"
#define CONSOLE_RESET "\033[0m"

void pretty_logs::question(const char* message, bool end_with_newline)
{
	std::stringstream ss;

	ss << "[" << CONSOLE_YELLOW << "?" << CONSOLE_RESET << "] " << message;

	if (end_with_newline)
	{
		ss << std::endl;
	}

	std::cout << ss.str();
}

void pretty_logs::info(const char* message, bool end_with_newline)
{
	std::stringstream ss;

	ss << "[!] " << message;

	if (end_with_newline)
	{
		ss << std::endl;
	}

	std::cout << ss.str();
}

void pretty_logs::success(const char* message, bool end_with_newline)
{
	std::stringstream ss;

	ss << "[" << CONSOLE_GREEN << "+" << CONSOLE_RESET << "] " << message;

	if (end_with_newline)
	{
		ss << std::endl;
	}

	std::cout << ss.str();
}

void pretty_logs::error(const char* message, bool end_with_newline)
{
	std::stringstream ss;

	ss << "[" << CONSOLE_RED << "x" << CONSOLE_RESET << "] " << message;

	if (end_with_newline)
	{
		ss << std::endl;
	}

	std::cout << ss.str();
}