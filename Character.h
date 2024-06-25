#ifndef CHARACTER_H
#define CHARACTER_H

#include "ui/GameUI.h"
#include <string>

struct SpriteAndPosition {
	std::string sprite;
	sf::Vector2f pos;
};

class Character
{
	sf::Texture m_texture;
	sf::Sprite	m_sprite{};

public:
	Character(std::string spriteLocation, sf::Vector2f position);
	void draw(sf::RenderWindow& window);

};

#endif
