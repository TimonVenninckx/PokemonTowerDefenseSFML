#ifndef GAMESPEEDUI_H
#define GAMESPEEDUI_H

#include <SFML/Graphics.hpp>

class GameSpeedUI
{
private:
	sf::RectangleShape m_box;
	sf::Text m_text;
	float m_gameSpeed;

public:
	GameSpeedUI(float speed, sf::Vector2f location);

	float getGameSpeed();
	void setActive(bool active);
	void draw(sf::RenderWindow& window);
	bool contains(sf::Vector2f position);
};

#endif