#pragma once
#include "CommandParser.h"
#include "Player.h"
#include "World.h"

#include <string>

class Game
{
	World world;
	Player player;
	std::string currentRoom;
	bool running = true;

  public:
	void run();

  private:
	void header(const std::string& title);
	std::string hpBar();
	void intro();
	void describeRoom();

	void loop();
	void handle(const Command& cmd);

	void move(const std::string& dir);
	void takeItem(const std::string& id);
	void showInventory();
	void useItem(const std::string& id);
	void fight();
	void winGame();
};
