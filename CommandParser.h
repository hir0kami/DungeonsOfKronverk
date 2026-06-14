#pragma once
#include <string>

struct Command
{
	std::string verb;
	std::string arg;
};

class CommandParser
{
  public:
	static Command parse(const std::string& raw);
};
