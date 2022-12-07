#include "Logger.h"

#include <fstream>
#include <sstream>

Logger::Logger()
{
	nLines = 1;
	data = std::vector<std::vector<float>>(1);
}

void Logger::WriteValue(float val)
{
	data[nLines - 1].push_back(val);
}

void Logger::NewLine()
{
	nLines++;
	data.emplace_back();
}

void Logger::Reset()
{
	data = std::vector<std::vector<float>>(1);
	nLines = 1;
}

void Logger::LogToFile(std::string fileName)
{
	std::ofstream out(fileName, std::ios::binary);

	for (int i = 0; i < nLines; i++)
	{
		if (i > 0)
			out << '\n';

		for (const auto& f : data[i])
		{
			out << f << " ";
		}
	}

	out.close();
}