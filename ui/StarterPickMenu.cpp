#include "StarterPickMenu.h"
#include "../sqlite/Database.h"
#include "../SpriteLoader.h"

StarterPickMenu::StarterPickMenu()
	: m_starterIcons{ {
		{nullptr,{760.f,500.f} },
		{nullptr,{960.f,500.f} },
		{nullptr,{1160.f,500.f} },
	} }
{
	m_text.setFont(GameUI::font);
	m_text.setString("Pick Your Starter");
	m_text.setCharacterSize(60);
	m_text.setPosition(960.f, 300.f);
	m_text.setOrigin(m_text.getGlobalBounds().getSize() / 2.f);

	std::array<AttackStats*, 4> attacks{ &Database::allAttackStats[33], nullptr, nullptr, nullptr };
	PlayerPokemon* pokemon = new PlayerPokemon(&Database::baseStats[1], 5, SpriteLoader::getPokemonSprite(1),attacks);
	m_starterIcons[0].setPokemon(pokemon);
	pokemon = new PlayerPokemon(&Database::baseStats[4], 5, SpriteLoader::getPokemonSprite(4), attacks);
	m_starterIcons[1].setPokemon(pokemon);
	pokemon = new PlayerPokemon(&Database::baseStats[7], 5, SpriteLoader::getPokemonSprite(7), attacks);
	m_starterIcons[2].setPokemon(pokemon);



	m_backButton.setCornerPointCount(5);
	m_backButton.setCornersRadius(15.f);
	m_backButton.setSize({ 250.f, 120.f });
	m_backButton.setOrigin(m_backButton.getGlobalBounds().getSize() / 2.f);
	m_backButton.setPosition(135.f, 70.f);
	m_backButton.setFillColor(GameUI::buttonColor);

	m_backText.setFont(GameUI::font);
	m_backText.setString("BACK");
	m_backText.setCharacterSize(70);
	m_backText.setOrigin(m_backText.getGlobalBounds().getSize().x / 2.f, m_backText.getGlobalBounds().getSize().y / 1.4f);
	m_backText.setPosition(135.f, 70.f);
}

void StarterPickMenu::draw(sf::RenderWindow& window)
{
	window.draw(m_text);
	for (int i{ 0 }; i < m_starterIcons.size(); i++) {
		m_starterIcons[i].drawBox(window);
		m_starterIcons[i].drawSprite(window);
	}
	window.draw(m_backButton);
	window.draw(m_backText);
}

PlayerPokemon* StarterPickMenu::chosePokemon(sf::Vector2f mousePos)
{
	for (int i{ 0 }; i < m_starterIcons.size(); i++) {
		if (m_starterIcons[i].contains(mousePos)) {
			m_pickedPokemon = m_starterIcons[i].getPokemon();
			GameUI::clickSound.play();
			return m_pickedPokemon;
		}
	}
	return nullptr;
}

bool StarterPickMenu::clickedBackButton(sf::Vector2f mousePos)
{
	if (m_backButton.getGlobalBounds().contains(mousePos)) {
		GameUI::clickSound.play();
		return true;
	}
	return false;
}

StarterPickMenu::~StarterPickMenu()
{
	for (int i{ 0 }; i < m_starters.size(); i++) {
		if (m_starters[i] != m_pickedPokemon) {
			delete m_starters[i];
		}
	}
}
