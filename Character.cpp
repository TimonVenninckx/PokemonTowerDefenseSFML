#include "Character.h"
#include <iostream>

Character::Character(std::string spriteLocation, sf::Vector2f position)
{
	std::cout << "Created charcter\n";
	if (!m_texture.loadFromFile("textures/characters/" + spriteLocation)) {
		std::cout << "charcter already used\n";
	}
	m_sprite.setPosition(position);
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);
}


void Character::draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}
