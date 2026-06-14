#include "Player.h"

bool Player::isAlive() const {
    return hp > 0;
}

bool Player::hasWeapon() const {
    for (const Item& it : inventory)
        if (it.type == "weapon") return true;
    return false;
}

int Player::attackPower() const {
    int bonus = 0;
    for (const Item& it : inventory)
        if (it.type == "weapon") bonus += it.value;
    return baseDamage + bonus;
}
