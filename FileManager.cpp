#include "FileManager.h"

#include "json.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

std::string FileManager::readFile(const std::string& path)
{
	std::ifstream in(path);
	if (!in)
	{
		std::cerr << "Не удалось открыть файл: " << path << "\n";
		std::cerr << "Проверь, что папка assets лежит рядом с игрой.\n";
		std::cin.get();
		std::exit(1);
	}
	std::stringstream ss;
	ss << in.rdbuf();
	return ss.str();
}

void FileManager::loadAll(World& world, const std::string& assetsDir)
{
	loadItems(world, assetsDir + "/items.json");
	loadEnemies(world, assetsDir + "/enemies.json");
	loadRooms(world, assetsDir + "/rooms.json");
}

void FileManager::loadItems(World& world, const std::string& path)
{
	json data = json::parse(readFile(path));

	for (const auto& j : data["items"])
	{
		Item it;
		it.id = j.value("id", "");
		it.name = j.value("name", "");
		it.description = j.value("desc", "");
		it.type = j.value("type", "");
		it.value = j.value("value", 0);
		world.items[it.id] = it;
	}
}

void FileManager::loadEnemies(World& world, const std::string& path)
{
	json data = json::parse(readFile(path));

	for (const auto& j : data["enemies"])
	{
		Enemy e;
		e.id = j.value("id", "");
		e.name = j.value("name", "");
		e.description = j.value("desc", "");
		e.hp = j.value("hp", 0);
		e.damage = j.value("damage", 0);
		world.enemyTemplates[e.id] = e;
	}
}

void FileManager::loadRooms(World& world, const std::string& path)
{
	json data = json::parse(readFile(path));

	world.startRoom = data.value("start", "");
	world.winRoom = data.value("win", "");

	for (const auto& j : data["rooms"])
	{
		Room r;
		r.id = j.value("id", "");
		r.name = j.value("name", "");
		r.description = j.value("desc", "");

		if (j.contains("exits"))
		{
			for (auto [dir, room] : j["exits"].items())
			{
				r.exits[dir] = room.get< std::string >();
			}
		}

		if (j.contains("items"))
		{
			for (const auto& id : j["items"])
			{
				r.itemIds.push_back(id.get< std::string >());
			}
		}

		r.enemyId = j.value("enemy", "");

		world.rooms[r.id] = r;
	}
}
