#include "World.h"

#include "Types.h"

#include <iostream>
#include <sstream>

std::string trim(const std::string& s)
{
	size_t a = s.find_first_not_of(" \t\r\n");
	if (a == std::string::npos)
		return "";
	size_t b = s.find_last_not_of(" \t\r\n");
	return s.substr(a, b - a + 1);
}

void rule(char c)
{
	std::cout << "  " << std::string(WIDTH, c) << "\n";
}

std::string arrow(const std::string& dir)
{
	if (dir == "north")
		return "^";
	if (dir == "south")
		return "v";
	if (dir == "east")
		return ">";
	if (dir == "west")
		return "<";
	return "?";
}

std::string dirLetter(const std::string& dir)
{
	if (dir == "north")
		return "N";
	if (dir == "south")
		return "S";
	if (dir == "east")
		return "E";
	if (dir == "west")
		return "W";
	return "?";
}

std::vector< std::string > wrap(const std::string& text, int maxLen)
{
	std::vector< std::string > lines;
	std::istringstream in(text);
	std::string word, line;
	while (in >> word)
	{
		if (!line.empty() && (int)(line.size() + 1 + word.size()) > maxLen)
		{
			lines.push_back(line);
			line = word;
		}
		else
		{
			if (!line.empty())
				line += ' ';
			line += word;
		}
	}
	if (!line.empty())
		lines.push_back(line);
	return lines;
}
