#ifndef LEVELTHUMBNAILUI_H
#define LEVELTHUMBNAILUI_H

#include <SFML/graphics.hpp>
#include "RoundedRectangleShape.h"
#include "../Level.h";

struct LevelInfo;

class LevelThumbnailUI
{
	LevelInfo m_levelInfo;
	sf::Text m_levelName{};
	sf::RoundedRectangleShape m_box{};
	sf::Texture m_texture{};
	sf::Sprite m_thumbnail{};
	bool m_locked{ false };
	sf::Sprite m_lockedSprite;

public:

	LevelThumbnailUI(LevelInfo info, sf::Vector2f position, sf::Vector2f size);
	~LevelThumbnailUI();
	LevelInfo* getLevel();
	bool contains(sf::Vector2f);
	void lock(bool locked);
	bool isLocked();

	void draw(sf::RenderWindow& window);
};

#endif
