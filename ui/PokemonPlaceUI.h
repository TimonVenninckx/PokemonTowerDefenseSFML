#ifndef POKEMONPLACEUI_H
#define POKEMONPLACEUI_H

#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.h"
#include "../PokemonStats.h"
class PlayerPokemon;


class PokemonPlaceUI
{
private:
	sf::RoundedRectangleShape m_border;
	sf::Sprite m_healthBar;
	sf::Sprite m_healthBarBack;
	sf::Sprite m_expBar;
	sf::Sprite m_expBarBack;
	sf::Text m_hpText;
	sf::Text m_expText;
	sf::Text m_lvlText;
	sf::Text m_nopokemon;

	sf::Vector2f m_size;
	sf::Vector2f m_backBarSize;

	sf::Sprite m_levelUpSprite;
	sf::Sprite m_sprite;
	PlayerPokemon *m_pokemon{ nullptr };

public:
	PokemonPlaceUI(sf::Vector2f position);

	void update(PokemonStats& pokemon);
	void draw(sf::RenderTarget &view);

	void setPosition(sf::Vector2f position);

	PlayerPokemon* getPokemon();
	void setPokemon(PlayerPokemon* pokemon);
	void setActive(bool active);

	bool contains(sf::Vector2f position);
};


#endif
