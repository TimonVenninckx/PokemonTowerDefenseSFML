#include "SaveInfoUI.h"

SaveInfoUI::SaveInfoUI(sf::Vector2f position)
{
	m_box.setCornerPointCount(5);
	m_box.setCornersRadius(15.f);
	m_box.setSize({ 500.f, 250.f });
	m_box.setOrigin(m_box.getGlobalBounds().getSize() / 2.f);
	m_box.setPosition(position);
	m_box.setFillColor(GameUI::buttonColor);


	m_nameText.setFont(GameUI::font);
	m_nameText.setCharacterSize(60);
	m_nameText.setPosition(position - sf::Vector2f{ 0.f,70.f });

	m_moneyText.setFont(GameUI::font);
	m_moneyText.setCharacterSize(60);
	m_moneyText.setPosition(position);

	m_pokemonText.setFont(GameUI::font);
	m_pokemonText.setCharacterSize(60);
	m_pokemonText.setPosition(position + sf::Vector2f{ 0.f,70.f });

}

void SaveInfoUI::loadInfo(SaveInfo info)
{
	m_info = info;
	if (info.pokemonAmount == 0) {
		m_pokemonText.setString("");
		m_nameText.setString("");
		m_moneyText.setString("EMPTY SAVE");
		m_moneyText.setOrigin(m_moneyText.getGlobalBounds().getSize().x / 2.f, m_moneyText.getCharacterSize() / 1.4f);
		return;
	}

	m_pokemonText.setString(std::to_string(info.pokemonAmount) + " Pokemon");
	m_pokemonText.setOrigin(m_pokemonText.getGlobalBounds().getSize().x / 2.f, m_pokemonText.getCharacterSize() / 1.4f);
	m_moneyText.setString("$" + std::to_string(info.money));
	m_moneyText.setOrigin(m_moneyText.getGlobalBounds().getSize().x / 2.f, m_moneyText.getCharacterSize() / 1.4f);
	m_nameText.setString(info.name);
	m_nameText.setOrigin(m_nameText.getGlobalBounds().getSize().x / 2.f, m_nameText.getCharacterSize() / 1.4f);
}

void SaveInfoUI::resetSave()
{
	SaveInfo test;
	test.id = m_info.id;
	loadInfo(test);
}

SaveInfo& SaveInfoUI::getSaveInfo()
{
	return m_info;
}

void SaveInfoUI::draw(sf::RenderWindow& window)
{
	window.draw(m_box);
	window.draw(m_moneyText);
	window.draw(m_pokemonText);
	window.draw(m_nameText);
}

bool SaveInfoUI::contains(sf::Vector2f mousePos)
{
	if (m_box.getGlobalBounds().contains(mousePos)) {
		GameUI::clickSound.play();
		return true;
	}
	return false;
}
