#include "TwoOptionMenu.h"

TwoOptionMenu::TwoOptionMenu()
{
	m_box.setPosition(960.f, 400.f);
	m_box.setSize({ 600.f, 400.f });
	m_box.setFillColor(GameUI::backgroundColor);
	m_box.setCornerPointCount(5);
	m_box.setCornersRadius(15.f);
	m_box.setOrigin(m_box.getGlobalBounds().getSize() / 2.f);
	m_box.setOutlineThickness(2.f);


	m_button1.setPosition(810.f, 540.f);
	m_button1.setSize({ 250.f, 80.f });
	m_button1.setFillColor(GameUI::buttonColor);
	m_button1.setCornerPointCount(5);
	m_button1.setCornersRadius(15.f);
	m_button1.setOrigin(m_button1.getGlobalBounds().getSize() / 2.f);


	m_button2.setPosition(1110.f, 540.f);
	m_button2.setSize({ 250.f, 80.f });
	m_button2.setFillColor(GameUI::buttonColor);
	m_button2.setCornerPointCount(5);
	m_button2.setCornersRadius(15.f);
	m_button2.setOrigin(m_button2.getGlobalBounds().getSize() / 2.f);

	m_button1Text.setPosition(m_button1.getPosition());
	m_button2Text.setPosition(m_button2.getPosition());
	m_infoText.setPosition(680.f, 220.f);
	m_button1Text.setFont(GameUI::font);
	m_button2Text.setFont(GameUI::font);
	m_infoText.setFont(GameUI::font);
}

void TwoOptionMenu::setInfoText(const std::string& text)
{
	m_infoText.setString(text);
}

void TwoOptionMenu::setButton1Text(const std::string& text)
{
	m_button1Text.setString(text);
	m_button1Text.setOrigin(m_button1Text.getGlobalBounds().getSize().x / 2.f, m_button1Text.getCharacterSize() / 1.4f);
}

void TwoOptionMenu::setButton2Text(const std::string& text)
{
	m_button2Text.setString(text);
	m_button2Text.setOrigin(m_button2Text.getGlobalBounds().getSize().x / 2.f, m_button2Text.getCharacterSize() / 1.4f);
}

int TwoOptionMenu::pickedOption(sf::Vector2f mousePos)
{
	if (m_button1.getGlobalBounds().contains(mousePos)) {
		GameUI::clickSound.play();
		return 1;
	}
	if (m_button2.getGlobalBounds().contains(mousePos)) {
		GameUI::clickSound.play();
		return 2;
	}

	return 0;
}

void TwoOptionMenu::draw(sf::RenderWindow& window)
{
	window.draw(m_box);
	window.draw(m_infoText);
	window.draw(m_button1);
	window.draw(m_button1Text);
	window.draw(m_button2);
	window.draw(m_button2Text);
}
