#include "GameOverUI.h"
#include "GameUI.h"
#include <iostream>

GameOverUI::GameOverUI()
{
	m_box.setSize({700.f, 500.f});
	m_box.setOrigin(m_box.getSize() / 2.f);
	m_box.setPosition(960.f,540.f);
	m_box.setCornersRadius(25);
	m_box.setCornerPointCount(5);
	m_box.setFillColor(GameUI::backgroundColor);


	m_button.setSize({ 400.f, 125.f });
	m_button.setOrigin(m_button.getSize() / 2.f);
	m_button.setPosition(960.f, 700.f);
	m_button.setCornersRadius(25);
	m_button.setCornerPointCount(5);
	m_button.setFillColor(GameUI::buttonColor);

	m_text.setFont(GameUI::font);
	m_text.setCharacterSize(30);
	m_text.setString("wow you won\nyou are so good at this game");
	m_text.setPosition(m_box.getPosition() - m_box.getSize() / 2.f + sf::Vector2f{25.f, 25.f});

	m_buttonText.setFont(GameUI::font);
	m_buttonText.setString("Return");
	m_buttonText.setCharacterSize(70);
	m_buttonText.setOrigin(m_buttonText.getGlobalBounds().getSize().x / 2.f,m_buttonText.getGlobalBounds().getSize().y / 1.5f);
	m_buttonText.setPosition(960.f, 700.f);

	m_sprite.setPosition(m_box.getPosition());

}

void GameOverUI::draw(sf::RenderWindow& window)
{
	window.draw(m_box);
	window.draw(m_button);
	window.draw(m_text);
	window.draw(m_buttonText);

	window.draw(m_sprite);
}

bool GameOverUI::exitUI(sf::Vector2f mousePosition)
{
	return m_button.getGlobalBounds().contains(mousePosition);
}

void GameOverUI::setText(const std::string& text)
{
	m_text.setString(text);
}

void GameOverUI::setSprite(const std::string& imageName)
{
	m_texture.loadFromFile("textures/characters/" + imageName);
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);
	float scale = 200.f / static_cast<float>(m_texture.getSize().x);
	m_sprite.setScale(scale,scale);
}
