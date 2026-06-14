#include "Game.h"

#include "CommandParser.h"
#include "FileManager.h"
#include "Types.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

void Game::run()
{
	std::srand((unsigned)std::time(nullptr));

	FileManager::loadAll(world, "assets");
	currentRoom = world.startRoom;

	intro();
	loop();

	std::cout << "  Нажми Enter, чтобы выйти...";
	std::string dummy;
	std::getline(std::cin, dummy);
}

void Game::header(const std::string& title)
{
	rule();
	std::string hp = "HP " + std::to_string(player.hp) + "/" + std::to_string(player.maxHp);
	int pad = WIDTH - (int)title.size() - (int)hp.size();
	if (pad < 1)
		pad = 1;
	std::cout << "  " << title << std::string(pad, ' ') << hp << "\n";
	rule();
}

std::string Game::hpBar()
{
	int filled = player.hp * 10 / player.maxHp;
	std::string bar = "[";
	for (int k = 0; k < 10; ++k)
		bar += (k < filled) ? '#' : '-';
	bar += "]";
	return bar;
}

void Game::intro()
{
	std::cout << "\n";
	rule('=');
	std::cout << "          DUNGEONS OF KRONVERK\n";
	rule('=');
	std::cout
		<< "\n"
		   "  The makeup session is over. Everyone\n"
		   "  went home. But you're still here.\n"
		   "  Turnstiles silent. Phone dead.\n"
		   "  One way out: go deeper, find your\n"
		   "  gradebook and face the professor.\n"
		   "\n";
	rule();
	std::cout
		<< "  COMMANDS\n"
		   "    n / s / e / w  - move (shown as ^ v < >)\n"
		   "    look      - describe the room\n"
		   "    take <id> - pick up an item\n"
		   "    items     - show inventory\n"
		   "    use <id>  - use an item\n"
		   "    fight     - attack the enemy\n"
		   "    help      - show this list\n"
		   "    quit      - exit the game\n";
	rule();
	std::cout << "\n";
	describeRoom();
}

void Game::describeRoom()
{
	const Room& r = world.rooms[currentRoom];
	header(r.name);

	for (const std::string& l : wrap(r.description, WIDTH - 2))
		std::cout << "  " << l << "\n";
	std::cout << "\n";

	if (!r.enemyId.empty())
	{
		const Enemy& e = world.enemyTemplates[r.enemyId];
		std::cout << "  ! THREAT  " << e.name << "  (" << e.hp << " HP)\n";
	}

	for (const std::string& id : r.itemIds)
		std::cout << "  * " << world.items[id].name << "  (" << id << ")\n";

	std::cout << "\n  EXITS: ";
	for (const auto& e : r.exits)
		std::cout << "[" << arrow(e.first) << dirLetter(e.first) << "] ";
	std::cout << "\n";
	rule();
	std::cout << "\n";
}

void Game::loop()
{
	std::string raw;
	while (running && player.isAlive())
	{
		std::cout << "  " << hpBar() << " > ";
		if (!std::getline(std::cin, raw))
			break;
		handle(CommandParser::parse(raw));
	}
	if (!player.isAlive())
	{
		std::cout << "\n";
		rule('=');
		std::cout << "  You collapsed. Session failed.\n";
		std::cout << "             GAME OVER\n";
		rule('=');
		std::cout << "\n";
	}
}

void Game::handle(const Command& cmd)
{
	const std::string& verb = cmd.verb;
	const std::string& arg = cmd.arg;

	if (verb.empty())
		return;

	if (verb == "north" || verb == "south" || verb == "east" || verb == "west")
		move(verb);
	else if (verb == "look")
		describeRoom();
	else if (verb == "take")
		takeItem(arg);
	else if (verb == "items")
		showInventory();
	else if (verb == "use")
		useItem(arg);
	else if (verb == "fight")
		fight();
	else if (verb == "help")
		intro();
	else if (verb == "quit")
	{
		running = false;
		std::cout << "  Goodbye.\n\n";
	}
	else
		std::cout << "  Unknown command. Type help.\n\n";
}

void Game::move(const std::string& dir)
{
	Room& r = world.rooms[currentRoom];
	if (!r.enemyId.empty())
	{
		std::cout << "  " << world.enemyTemplates[r.enemyId].name << " blocks the way. Fight first.\n\n";
		return;
	}
	auto it = r.exits.find(dir);
	if (it == r.exits.end())
	{
		std::cout << "  No exit in that direction.\n\n";
		return;
	}
	currentRoom = it->second;
	std::cout << "\n";
	describeRoom();
}

void Game::takeItem(const std::string& id)
{
	Room& r = world.rooms[currentRoom];
	for (size_t i = 0; i < r.itemIds.size(); ++i)
	{
		if (r.itemIds[i] == id)
		{
			player.inventory.push_back(world.items[id]);
			std::cout << "  + " << world.items[id].name << "\n"
					  << "    " << world.items[id].description << "\n\n";
			r.itemIds.erase(r.itemIds.begin() + i);
			return;
		}
	}
	std::cout << "  No such item here.\n\n";
}

void Game::showInventory()
{
	if (player.inventory.empty())
	{
		std::cout << "  Inventory is empty.\n\n";
		return;
	}
	rule();
	std::cout << "  INVENTORY\n";
	for (const Item& it : player.inventory)
		std::cout << "  * " << it.name << "  (" << it.id << ")\n"
				  << "    " << it.description << "\n";
	std::cout << "  ATTACK: " << player.attackPower() << "\n";
	rule();
	std::cout << "\n";
}

void Game::useItem(const std::string& id)
{
	for (size_t i = 0; i < player.inventory.size(); ++i)
	{
		if (player.inventory[i].id == id)
		{
			const Item& it = player.inventory[i];
			if (it.type == "heal")
			{
				int before = player.hp;
				player.hp = std::min(player.maxHp, player.hp + it.value);
				std::cout << "  " << it.name << ": +" << (player.hp - before) << " HP -> " << player.hp << "/" << player.maxHp << "\n\n";
				player.inventory.erase(player.inventory.begin() + i);
			}
			else
			{
				std::cout << "  Can't use " << it.name << " right now.\n\n";
			}
			return;
		}
	}
	std::cout << "  No such item in inventory.\n\n";
}

void Game::fight()
{
	Room& r = world.rooms[currentRoom];
	if (r.enemyId.empty())
	{
		std::cout << "  Nothing to fight here.\n\n";
		return;
	}

	if (currentRoom == world.winRoom && !player.hasWeapon())
	{
		std::cout
			<< "  The professor smirks. Without lecture notes\n"
			<< "  you can't answer a single question.\n"
			<< "  Find the notes (konspekt) first!\n\n";
		return;
	}

	Enemy enemy = world.enemyTemplates[r.enemyId];

	std::cout << "\n";
	rule();
	std::cout << "  ENCOUNTER: " << enemy.name << "\n";
	for (const std::string& l : wrap(enemy.description, WIDTH - 2))
		std::cout << "  " << l << "\n";
	rule('.');

	while (enemy.isAlive() && player.isAlive())
	{
		int dmg = player.attackPower() + (std::rand() % 6 - 2);	   // Атака игрока
		if (dmg < 1)
			dmg = 1;
		enemy.hp -= dmg;
		std::cout << "  You hit for " << dmg << " -> " << enemy.name << ": " << std::max(0, enemy.hp) << " HP\n";
		if (!enemy.isAlive())
			break;

		int edmg = enemy.damage + (std::rand() % 5 - 2);	// Атака врага
		if (edmg < 1)
			edmg = 1;
		player.hp -= edmg;
		std::cout << "  " << enemy.name << " hits for " << edmg << " -> You: " << std::max(0, player.hp) << "/"
				  << player.maxHp << " HP\n";
	}

	rule('.');
	if (player.isAlive())
	{
		std::cout << "  " << enemy.name << " defeated!\n";
		rule();
		std::cout << "\n";
		r.enemyId.clear();
		if (currentRoom == world.winRoom)
			winGame();
	}
	else
	{
		rule();
		std::cout << "\n";
	}
}

void Game::winGame()
{
	std::cout << "\n";
	rule('=');
	std::cout
		<< "  The professor puts down the red pen,\n"
		   "  checks your gradebook and nods.\n"
		   "  The turnstiles click open. You pass.\n"
		   "\n"
		   "               VICTORY!\n";
	rule('=');
	std::cout << "\n";
	running = false;
}
