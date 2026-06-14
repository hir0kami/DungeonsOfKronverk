#pragma once
#include "Enemy.h"
#include "Item.h"
#include "Room.h"

#include <map>
#include <string>
#include <vector>

struct World
{
	std::map< std::string, Item > items;
	std::map< std::string, Enemy > enemyTemplates;
	std::map< std::string, Room > rooms;
	std::string startRoom;
	std::string winRoom;
};

std::string trim(const std::string& s);

void rule(char c = '-');

std::string arrow(const std::string& dir);

std::string dirLetter(const std::string& dir);

std::vector< std::string > wrap(const std::string& text, int maxLen);
