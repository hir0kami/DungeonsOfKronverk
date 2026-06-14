#pragma once
#include <map>
#include <string>
#include <vector>

struct Room
{
	std::string id;
	std::string name;
	std::string description;
	std::map< std::string, std::string > exits;
	std::vector< std::string > itemIds;
	std::string enemyId;
};
