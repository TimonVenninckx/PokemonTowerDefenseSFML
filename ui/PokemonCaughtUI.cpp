#include "PokemonCaughtUI.h"
#include "../PlayerPokemon.h"
#include "GameUI.h"

PokemonCaughtUI::PokemonCaughtUI()
{

	m_box.setSize({500.f, 300.f});
	m_box.setCornerPointCount(5);
	m_box.setCornersRadius(15.f);
	m_box.setFillColor(GameUI::backgroundColor);
	m_box.setOrigin(m_box.getGlobalBounds().getSize() / 2.f);
	m_box.setPosition(960.f, 500.f);

	m_sprite.setPosition(960.f, 500.f);
	m_sprite.setScale(3.f,3.f);


	m_nameText.setFont(GameUI::font);
	m_nameText.setPosition(960.f, 400.f);
	m_nameText.setCharacterSize(40);

	m_clickText.setFont(GameUI::font);
	m_clickText.setPosition(960.f, 600.f);
	m_clickText.setCharacterSize(40);
	m_clickText.setString("Click to continue");
	m_clickText.setOrigin(m_clickText.getGlobalBounds().getSize() / 2.f);

	m_song.openFromFile("sound/caughtpokemon.mp3");
}

void PokemonCaughtUI::load(PlayerPokemon* pokemon)
{
	m_active = true;
	m_nameText.setString("Caught: " + pokemon->getName());
	m_nameText.setOrigin(m_nameText.getGlobalBounds().getSize() / 2.f);
	m_nameText.setFillColor(PokemonStats::getTypeColor(pokemon->getStats().type1));

	m_sprite.setTexture(*pokemon->getTexture());
	m_sprite.setTextureRect({ 0,0, static_cast<int>(pokemon->getTexture()->getSize().x / 4.f), static_cast<int>(pokemon->getTexture()->getSize().y / 4.f) });
	m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
	
	GameUI::music.pause();
	m_song.setVolume(GameUI::sfxVolume);
	m_song.play();

}

void PokemonCaughtUI::update(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		m_active = false;
		m_song.stop();
		GameUI::music.play();
	}
}

bool PokemonCaughtUI::isActive()
{
	return m_active;
}

void PokemonCaughtUI::draw(sf::RenderWindow& window)
{
	window.draw(m_box);
	window.draw(m_sprite);
	window.draw(m_clickText);
	window.draw(m_nameText);
}
