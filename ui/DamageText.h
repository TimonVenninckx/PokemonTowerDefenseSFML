#ifndef DAMAGETEXT_H
#define DAMAGETEXT_H

#include <SFML/Graphics.hpp>

class DamageText
{
	sf::Text m_text;
	float m_duration;
	sf::Vector2f m_direction;

public:
	DamageText(sf::Vector2f location, sf::Vector2f direction, int damage);
	DamageText(sf::Vector2f location, sf::Vector2f direction, std::string& text);
	void draw(sf::RenderWindow& window);
	bool update(float delta);
};


#endif
