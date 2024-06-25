#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Level.h"
#include "Player.h"
#include "ui/LevelMenu.h"
#include "ui/SaveMenu.h"

class Game
{

public:
	Game();

	void run();



private:

	Level* level;

	sf::RenderWindow window{};
	sf::View uiView{};

	sf::Text fpsCounter{};

	LevelMenu* levelMenu;
	Player player{};
	int currentLevel{};
	SaveMenu* saveMenu{};
};

#endif