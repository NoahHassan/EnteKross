#include "KrossException.h"

#include <sstream>

KrossException::KrossException(int line, std::string file) noexcept
	:
	line(line),
	file(file)
{}

const char* KrossException::what() const noexcept
{
	std::ostringstream err;
	err << GetType() <<
		"\nLine: " << GetLine() <<
		"\nFile: " << GetFile() << std::endl;

	whatBuffer = err.str();
	return whatBuffer.c_str();
}

const char* KrossException::GetType() const noexcept
{
	return "EnteKross Exception";
}

const int KrossException::GetLine() const noexcept
{
	return line;
}

const char* KrossException::GetFile() const noexcept
{
	return file.c_str();
}