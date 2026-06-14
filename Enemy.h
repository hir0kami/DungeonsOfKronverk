#pragma once
#include <string>

struct Enemy
{
	std::string id;
	std::string name;
	std::string description;
	int hp = 0;
	int damage = 0;

	bool isAlive() const;
};
