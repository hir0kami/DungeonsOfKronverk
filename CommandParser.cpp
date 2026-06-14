#include "CommandParser.h"

#include "World.h"

Command CommandParser::parse(const std::string& raw)
{
	Command cmd;
	std::string s = trim(raw);

	size_t sp = s.find(' ');
	if (sp == std::string::npos)
	{
		cmd.verb = s;
	}
	else
	{
		cmd.verb = s.substr(0, sp);
		cmd.arg = trim(s.substr(sp + 1));
	}

	if (cmd.verb == "n")
		cmd.verb = "north";
	else if (cmd.verb == "s")
		cmd.verb = "south";
	else if (cmd.verb == "e")
		cmd.verb = "east";
	else if (cmd.verb == "w")
		cmd.verb = "west";

	return cmd;
}
