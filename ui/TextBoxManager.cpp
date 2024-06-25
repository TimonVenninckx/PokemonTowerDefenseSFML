#include "TextBoxManager.h"
#include "../sqlite/Database.h"

TextBoxManager::TextBoxManager(int levelId)
{


	m_textBox.setPosition(10.f, 780.f);
	m_textBox.setSize({1900.f, 300.f});
	m_textBox.setCornerPointCount(5);
	m_textBox.setCornersRadius(25.f);
	m_textBox.setFillColor(sf::Color{ 255,255,255,220 });
	m_textBox.setOutlineThickness(-5.f);
	m_textBox.setOutlineColor(GameUI::backgroundColor);


	m_sprite.setPosition(m_textBox.getPosition() + sf::Vector2f{ 150.f,150.f });

	m_line.setPosition(m_textBox.getPosition() + sf::Vector2f{ 300.f,0.f });
	m_line.setFillColor(GameUI::backgroundColor);
	m_line.setSize({ 2.f, 300.f });

	m_text.setFont(GameUI::font);
	m_text.setPosition(m_line.getPosition() + sf::Vector2f{ 30.f,30.f } );
	m_text.setFillColor(sf::Color::Black);
	m_text.setCharacterSize(60);

	m_nextButton.setFillColor(GameUI::buttonColor);
	m_nextButton.setSize({ 200.f, 100.f });
	m_nextButton.setCornerPointCount(5);
	m_nextButton.setCornersRadius(15);
	m_nextButton.setOrigin(m_nextButton.getGlobalBounds().getSize() / 2.f);
	m_nextButton.setPosition(1795.f, 1015.f);


	m_nextText.setFont(GameUI::font);
	m_nextText.setCharacterSize(60);
	m_nextText.setString("Next");
	m_nextText.setOrigin(m_nextText.getGlobalBounds().getSize().x / 2.f, m_nextText.getCharacterSize() / 1.4f);
	m_nextText.setPosition(m_nextButton.getPosition());

	Database::getLevelMessages(m_messages, levelId);
	if (!m_messages.empty()) {
		m_active = true;
		m_text.setString(m_messages.front().text);
		m_texture.loadFromFile("textures/levelmessage/" + m_messages.front().characterLocation);
		m_sprite.setTexture(m_texture);
		m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);
		m_messages.pop();
	}
}


bool TextBoxManager::isActive()
{
	return m_active;
}

void TextBoxManager::update(sf::Event& event, sf::Vector2f mousePos)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (m_nextButton.getGlobalBounds().contains(mousePos)) {
			if (m_messages.empty())
				m_active = false;
			else {
				m_text.setString(m_messages.front().text);
				m_texture.loadFromFile("textures/levelmessage/" + m_messages.front().characterLocation);
				m_sprite.setTexture(m_texture);
				m_sprite.setOrigin(static_cast<float>(m_texture.getSize().x) / 2.f, static_cast<float>(m_texture.getSize().y) / 2.f);
				m_messages.pop();
			}
		}
	}
}

void TextBoxManager::draw(sf::RenderWindow& window)
{
	window.draw(m_textBox);
	window.draw(m_line);
	window.draw(m_sprite);
	window.draw(m_text);
	window.draw(m_nextButton);
	window.draw(m_nextText);
}
