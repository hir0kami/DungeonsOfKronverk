# Dungeons of Kronverk — структура проекта

Файлы разбиты как у группы. Перед сборкой нужен **json.hpp** (nlohmann/json) —
он у тебя уже есть в проекте (видно в Заголовочных файлах на скрине).

## Файлы

| Файл                    | Что внутри                                              |
|-------------------------|---------------------------------------------------------|
| `Types.h`               | Общая константа `WIDTH`.                                |
| `Item.h`                | Структура `Item` (предмет).                             |
| `Enemy.h` / `.cpp`      | Структура `Enemy` + метод `isAlive()`.                  |
| `Player.h` / `.cpp`     | Структура `Player` + методы боя/инвентаря.              |
| `Room.h`                | Структура `Room` (комната).                             |
| `World.h` / `.cpp`      | Контейнер мира `World` + текстовые утилиты (trim/wrap/rule/arrow). |
| `FileManager.h` / `.cpp`| Загрузка JSON-ассетов через nlohmann/json.              |
| `CommandParser.h`/`.cpp`| Разбор ввода игрока (`take konspekt` → verb+arg).       |
| `Game.h` / `.cpp`       | Класс `Game`: цикл, отрисовка, бой.                     |
| `main.cpp`              | Точка входа.                                            |
| `assets/*.json`         | Данные: предметы, враги, комнаты.                       |

## Подключение json.hpp в Visual Studio (если вдруг нет)

Вариант 1 — просто положить файл: скачай single-header
`json.hpp` с github.com/nlohmann/json (релиз → Assets → `json.hpp`),
кинь в папку проекта, добавь в "Файлы заголовков".

Вариант 2 — vcpkg: `vcpkg install nlohmann-json`, тогда в коде
`#include <nlohmann/json.hpp>` вместо `#include "json.hpp"`.

## Сборка ассетов

JSON-файлы должны лежать в папке `assets` **рядом с .exe**.
В Visual Studio это рабочая папка проекта (где `.vcxproj`) при запуске из IDE.
Если игра пишет "Не удалось открыть файл" — проверь, что `assets` рядом с exe
(или поменяй путь в `Game::run()` / `FileManager::loadAll`).

## Зависимости между файлами (кто кого include)

```
main.cpp        -> Game.h
Game.h          -> World.h, Player.h, CommandParser.h
Game.cpp        -> Game.h, Types.h, FileManager.h, CommandParser.h
FileManager.h   -> World.h
FileManager.cpp -> FileManager.h, json.hpp
World.h         -> Item.h, Enemy.h, Room.h
World.cpp       -> World.h, Types.h
Player.h        -> Item.h
Enemy.h         -> (только <string>)
CommandParser.cpp -> CommandParser.h, World.h (ради trim)
```
