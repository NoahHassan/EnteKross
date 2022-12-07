#pragma once

#include <vector>
#include <string>

class Logger
{
public:
	Logger();
public:
	void WriteValue(float val);
	void NewLine();
	void Reset();
	void LogToFile(std::string fileName);
private:
	std::vector<std::vector<float>> data;
	int nLines;
};