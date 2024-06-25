#ifndef POKEMONPICKICON_H
#define POKEMONPICKICON_H

#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.h"
#include "../PlayerPokemon.h"


class PokemonPickIcon
{
private:
	sf::RoundedRectangleShape m_box;
	sf::Text m_text;
	sf::Sprite m_sprite;
	PlayerPokemon* m_pokemon;
public:
	PokemonPickIcon(PlayerPokemon* pokemon,sf::Vector2f position);
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);
	void updateDrag(sf::Vector2f position);
	void stopDrag();
	void drawBox(sf::RenderWindow& window);
	void drawSprite(sf::RenderWindow& window);
	bool contains(sf::Vector2f mousePos);
	void setPokemon(PlayerPokemon* pokemon);
	PlayerPokemon* getPokemon() const;
	bool operator==(const PokemonPickIcon& icon) {
		return m_pokemon == icon.getPokemon();
	}
};








#endif
