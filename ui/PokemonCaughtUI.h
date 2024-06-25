#ifndef POKEMONCAUGHTUI_H
#define POKEMONCAUGHTUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "RoundedRectangleShape.h"
class PlayerPokemon;


class PokemonCaughtUI
{
	sf::RoundedRectangleShape m_box;
	sf::Text	m_nameText;
	sf::Text	m_clickText;
	sf::Sprite	m_sprite;

	sf::Music	m_song;

	bool	m_active;

public:
	PokemonCaughtUI();
	void load(PlayerPokemon* pokemon);
	void update (sf::Event& event);
	bool isActive();
	void draw(sf::RenderWindow& window);


};



#endif
