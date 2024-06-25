#ifndef LEVELMENU_H
#define LEVELMENU_H

#include <SFML/Graphics.hpp>
#include "../Level.h"
#include "../Player.h"
#include "LevelThumbnailUI.h"
#include "PokemonPickMenu.h"
#include "TwoOptionMenu.h"

class LevelMenu
{
	std::vector<LevelThumbnailUI*> m_thumbnails;
	Player& m_player;
	PokemonPickMenu m_pickPokemonMenu;
	LevelThumbnailUI* m_chosenLevel{ nullptr };
	sf::View& m_view;

	sf::RoundedRectangleShape m_saveButton;
	sf::Text m_saveText;

	sf::RoundedRectangleShape m_backButton;
	sf::Text m_backText;

	bool m_goingBack{ false };
	TwoOptionMenu m_twoOptionSaveMenu;

public:
	LevelMenu(sf::RenderWindow& window,Player& player,sf::View& uiview);
	~LevelMenu();

	bool clickedBackButton(sf::RenderWindow& window,sf::Event& event);
	LevelInfo* chosenLevelId(sf::RenderWindow& window, float delta,sf::Event& event);
	void reload();
	void draw(sf::RenderWindow& window);

};

#endif