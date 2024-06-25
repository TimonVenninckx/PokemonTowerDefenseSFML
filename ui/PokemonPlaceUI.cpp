#include "PokemonPlaceUI.h"
#include "GameUI.h"
#include "../PlayerPokemon.h"
#include "RoundedRectangleShape.h"
#include <iostream>

PokemonPlaceUI::PokemonPlaceUI(sf::Vector2f position)
{

	m_border.setSize({ 200.f,200.f });
	m_border.setFillColor({ 30, 30, 30,255 });
	m_border.setPosition(position);
	m_border.setCornerPointCount(12);
	m_border.setCornersRadius(20);


	m_sprite.setPosition(position + sf::Vector2f{ 100.f, 90.f });
	m_sprite.setScale(2.f,2.f);
	
	

	m_expBarBack.setTexture(GameUI::border);
	m_expBar.setTexture(GameUI::border);
	m_healthBar.setTexture(GameUI::border);
	m_healthBarBack.setTexture(GameUI::border);

	// bars
	m_size = { 126.f, 14.f };
	m_backBarSize = { 130.f, 18.f };
	m_size =		{ m_size.x / GameUI::border.getSize().x, m_size.y / GameUI::border.getSize().y };
	m_backBarSize = { m_backBarSize.x / GameUI::border.getSize().x, m_backBarSize.y / GameUI::border.getSize().y };

	//m_healthBar.setScale(m_size);
	//m_expBar.setScale(m_size);
	m_healthBar.setColor(sf::Color(0, 255, 0, 255));
	m_expBar.setColor(sf::Color(0, 97, 219, 255));

	// backbars
	//m_healthBarBack.setScale(m_backBarSize);
	//m_expBarBack.setScale(m_backBarSize);
	m_healthBarBack.setColor(sf::Color(255, 255, 255, 255));
	m_expBarBack.setColor(sf::Color(255, 255, 255, 255));

	// positions  inside bar 2 px
	m_healthBar.setPosition(position + sf::Vector2f{ 57.f, 152.f });
	m_expBar.setPosition(position + sf::Vector2f{ 57.f, 177.f });
	m_healthBarBack.setPosition(position + sf::Vector2f{ 55.f, 150.f });
	m_expBarBack.setPosition(position + sf::Vector2f{ 55.f, 175.f });

	// hp and exp fonts
	m_hpText.setFont(GameUI::font);
	m_expText.setFont(GameUI::font);
	m_lvlText.setFont(GameUI::font);
	// strings
	m_hpText.setString("HP");
	m_expText.setString("EXP");
	m_lvlText.setString("LVL");
	// charsize
	m_hpText.setCharacterSize(22);
	m_expText.setCharacterSize(22);
	m_lvlText.setCharacterSize(30);

	m_lvlText.setPosition(position + sf::Vector2f{ 10.f,5.f });
	// Y position - 1/3 text height
	m_hpText.setPosition(position + sf::Vector2f{ 10.f,145.f });
	m_expText.setPosition(position + sf::Vector2f{ 10.f,170.f });


	m_nopokemon.setFont(GameUI::font);
	m_nopokemon.setCharacterSize(50);
	m_nopokemon.setString("NONE");
	m_nopokemon.setPosition(position + sf::Vector2f{ 30.f,75.f });

	
	
	m_levelUpSprite.setTexture(GameUI::levelUpArrow);
	m_levelUpSprite.setPosition(position + sf::Vector2f{ 150.f,10.f });


}

void PokemonPlaceUI::update(PokemonStats& pokemon)
{
	if (pokemon.health / pokemon.maxHealth < 0.3)
		m_healthBar.setColor(sf::Color::Red);
	m_healthBar.setScale({ m_size.x * (pokemon.health / pokemon.maxHealth), m_size.y });
	
	if(pokemon.health == 0)	
		m_border.setFillColor({ 30, 30, 30,255 });
	
	m_lvlText.setString("LVL " + std::to_string(pokemon.level));

	float currentLevelExp = 5.f * (pokemon.level * pokemon.level * pokemon.level) / 4.f;
	int nextlevel = pokemon.level + 1;
	//std::cout << "current exp: " << pokemon.experience << " current calc: " << pokemon.experience - currentLevelExp << '\n';
	//std::cout << "Currentlevel exp: " << currentLevelExp << " next level exp: " << nextlevel * nextlevel * nextlevel << " xp calculation: " << nextlevel * nextlevel * nextlevel - currentLevelExp << '\n';
	m_expBar.setScale({ m_size.x * ((pokemon.experience - currentLevelExp) /  ((5.f * (nextlevel * nextlevel * nextlevel) / 4.f) - currentLevelExp)),m_size.y });
	if (pokemon.experience == 0)
		m_expBar.setScale({ 0.f,0.f });

}

void PokemonPlaceUI::draw(sf::RenderTarget& window)
{


	window.draw(m_border);
	
	if (m_pokemon) {
		window.draw(m_sprite);
		window.draw(m_healthBarBack);
		window.draw(m_expBarBack);
		window.draw(m_healthBar);
		window.draw(m_expBar);

		window.draw(m_lvlText);
		window.draw(m_hpText);
		window.draw(m_expText);

		if (m_pokemon->canLevelUp()) {
			window.draw(m_levelUpSprite);
		}
	}
	else {
		window.draw(m_nopokemon);
	}
}

PlayerPokemon* PokemonPlaceUI::getPokemon()
{
	return m_pokemon;
}

void PokemonPlaceUI::setPokemon(PlayerPokemon* pokemon)
{
	m_pokemon = pokemon;
	sf::Texture* texture = pokemon->getTexture();
	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect{ 0,0,static_cast<int>(texture->getSize().x) / 4,static_cast<int>(texture->getSize().y) / 4});
	m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);

	m_lvlText.setString("LVL " + std::to_string(pokemon->getStats().level));
}

void PokemonPlaceUI::setActive(bool active) {
	if (active) {
		//m_border.setFillColor({ 0, 89, 255,255 });
		m_border.setFillColor({ 0, 140, 222,255 });
	}
	else {
		m_border.setFillColor({ 30, 30, 30,255 });
	}
}


bool PokemonPlaceUI::contains(sf::Vector2f position)
{
	return m_border.getGlobalBounds().contains(position);
}

void PokemonPlaceUI::setPosition(sf::Vector2f position) {

	m_border.setPosition(position);

	m_sprite.setPosition(position + sf::Vector2f{ 10.f,10.f });
	m_healthBar.setPosition(position + sf::Vector2f{ 30.f, 60.f });
	m_expBar.setPosition(position + sf::Vector2f{30.f, 80.f});

	m_hpText.setPosition(position + sf::Vector2f{ 10.f,60.f });
	m_expText.setPosition(position + sf::Vector2f{ 10.f,80.f });

	m_nopokemon.setPosition(position + sf::Vector2f{ 5.f,50.f });

}
