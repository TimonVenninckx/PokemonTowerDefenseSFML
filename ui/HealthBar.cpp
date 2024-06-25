#include "HealthBar.h"
#include "GameUI.h"

#include <iomanip>
#include <iostream>
#include <sstream>

HealthBar::HealthBar()
{
	m_size = { 40.f, 5.f };
	m_shape.setSize(m_size);
	m_shape.setFillColor(sf::Color(0, 255, 0, 255));
	m_shape.setOutlineThickness(1);
	m_shape.setOutlineColor(sf::Color::Black);
	m_shape.setOrigin(m_shape.getLocalBounds().getSize() / 2.f);
	// reset size because we now have a thickness

	m_healthText.setOrigin(m_healthText.getLocalBounds().getSize() / 2.f);
	m_healthText.setFont(GameUI::font);
	m_healthText.setCharacterSize(15);
	m_healthText.setOutlineThickness(1.f);
	m_lvlText.setFont(GameUI::font);
	m_lvlText.setCharacterSize(8);
	m_lvlText.setOutlineThickness(1.3f);
	m_lvlText.setString("LVL 100");

	m_statusSprite.setTexture(GameUI::statusIconTexture);
	m_statusSprite.setTextureRect({ 0,0,static_cast<int>(GameUI::statusIconTexture.getSize().x) / 6,static_cast<int>(GameUI::statusIconTexture.getSize().y) });
	m_statusSprite.setOrigin(static_cast<float>(GameUI::statusIconTexture.getSize().x + 2/* this is the outline */) / 6.f / 2.f, 0.f);
}

void HealthBar::canCatch(bool cancatch)
{
	m_canCatch = cancatch;
	if(cancatch)
		m_shape.setFillColor(sf::Color(0, 255, 0, 255));
	else	
		m_shape.setFillColor(sf::Color{ 0,0,255,255 });
}

void HealthBar::update(float health, float maxhealth)
{
	if (health / maxhealth < 0.3f && m_canCatch)
		m_shape.setFillColor(sf::Color::Red);
	m_shape.setSize({ std::max( m_size.x * (health / maxhealth),2.f), m_size.y });
	m_shape.setOrigin(m_shape.getGlobalBounds().getSize() / 2.f);

	std::stringstream healthstring;
	healthstring << std::fixed << std::setprecision(0) << health;
	m_healthText.setString(healthstring.str());
}

void HealthBar::updateLevel(int level) {
	m_lvlText.setString("LVL " + std::to_string(level));
	if (level < 10)
		m_lvlText.setOrigin(13.f, 5.f);
	else
		m_lvlText.setOrigin(15.f,5.f);
	//std::cout << "M_lvltext: " << m_lvlText.getCharacterSize() << ' ' << m_lvlText.getGlobalBounds().getSize().x << '\n';
}

void HealthBar::draw(sf::RenderTarget& window)
{
	window.draw(m_statusSprite);
	window.draw(m_shape);
	window.draw(m_lvlText);

	//window.draw(m_healthText);
}

void HealthBar::setPosition(sf::Vector2f position)
{
	m_shape.setPosition(position);
	//m_healthText.setPosition(position - sf::Vector2f{8.f, 0.f });
	m_lvlText.setPosition(position - sf::Vector2f{ 0.f, 9.f });
	m_statusSprite.setPosition(position - sf::Vector2f{0.f, -4.f});
}

void HealthBar::changeStatusEffect(StatusEffect::EffectType effect)
{
	m_statusSprite.setTextureRect({ static_cast<int>(effect) * static_cast<int>(GameUI::statusIconTexture.getSize().x / 6),0,static_cast<int>(GameUI::statusIconTexture.getSize().x) / 6,static_cast<int>(GameUI::statusIconTexture.getSize().y) });
}


