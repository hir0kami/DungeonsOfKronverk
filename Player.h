#pragma once
#include "Item.h"

#include <string>
#include <vector>

struct Player
{
	int hp = 100;
	int maxHp = 100;
	int baseDamage = 10;
	std::vector< Item > inventory;

	bool isAlive() const;
	bool hasWeapon() const;
	int attackPower() const;
};
