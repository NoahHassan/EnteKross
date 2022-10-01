#pragma once

#include <exception>
#include <string>

class KrossException : public std::exception
{
public:
	KrossException(int line, std::string file) noexcept;
	KrossException(const KrossException&) = delete;
	KrossException& operator =(const KrossException&) = delete;
public:
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	const int GetLine() const noexcept;
	const char* GetFile() const noexcept;
protected:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};