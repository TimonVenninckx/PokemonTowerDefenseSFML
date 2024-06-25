#include "DamageText.h"
#include "GameUI.h"

DamageText::DamageText(sf::Vector2f location,sf::Vector2f direction, int damage)
{
	m_direction = direction;
	m_text.setFont(GameUI::font);
	m_text.setPosition(location - sf::Vector2f{ 0.f,0.f });
	m_text.setString(std::to_string(damage));

	m_text.setCharacterSize(16);
	m_text.setOutlineThickness(1);
	m_duration = 1.f;
}

DamageText::DamageText(sf::Vector2f location, sf::Vector2f direction, std::string& text)
{
	m_direction = direction;
	m_text.setFont(GameUI::font);
	m_text.setPosition(location - sf::Vector2f{ 0.f,0.f });
	m_text.setString(text);

	m_text.setCharacterSize(16);
	m_text.setOutlineThickness(1);
	m_duration = 1.f;
}

void DamageText::draw(sf::RenderWindow& window)
{
	window.draw(m_text);
}

bool DamageText::update(float delta)
{
	m_duration -= delta;
	if (m_duration <= 0.f)
		return false;
	
	m_text.move(m_direction * 50.f * delta);
	if (m_duration < 1.f) {
		m_text.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * m_duration) ));
		m_text.setOutlineColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(255 * m_duration)));
	}
	return true;
}
