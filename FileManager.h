#pragma once
#include "World.h"

#include <string>

class FileManager
{
  public:
	static void loadAll(World& world, const std::string& assetsDir);

  private:
	static void loadItems(World& world, const std::string& path);
	static void loadEnemies(World& world, const std::string& path);
	static void loadRooms(World& world, const std::string& path);

	static std::string readFile(const std::string& path);
};
