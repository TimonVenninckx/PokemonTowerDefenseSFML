#include "GameSpeedUI.h"
#include "GameUI.h"
#include <sstream>

GameSpeedUI::GameSpeedUI(float speed, sf::Vector2f location)
{
	sf::Vector2f boxSize{ 60.f,60.f };
	m_gameSpeed = speed;
	m_box.setSize(boxSize);
	m_box.setPosition(location);
	m_box.setFillColor({ 255,0,0,255 });
	m_box.setOutlineColor(sf::Color::White);
	m_box.setOutlineThickness(1.f);

	m_text.setFont(GameUI::font);
	m_text.setCharacterSize(40);
	m_text.setPosition(location + sf::Vector2{5.f, 5.f});



	std::ostringstream out;
	out.precision(0);
	out << std::fixed << speed << 'x';
	

	m_text.setString(out.str());
	if (speed == 0.f)
		m_text.setString(L"⏸︎");
}

float GameSpeedUI::getGameSpeed()
{
	return m_gameSpeed;
}

void GameSpeedUI::setActive(bool active)
{
	if(active)
		m_box.setFillColor({ 0,255,0,255 });
	else	
		m_box.setFillColor({ 255,0,0,255 });
}

void GameSpeedUI::draw(sf::RenderWindow& window)
{
	window.draw(m_box);
	window.draw(m_text);
}

bool GameSpeedUI::contains(sf::Vector2f position)
{
	return m_box.getGlobalBounds().contains(position);
}
