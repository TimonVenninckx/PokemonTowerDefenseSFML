#ifndef POKEMONPLACEMENT_H
#define POKEMONPLACEMENT_H

class PlayerPokemon;
#include <SFML/Graphics.hpp>

class PokemonPlacement
{
	PlayerPokemon* m_pokemon{ nullptr };
	sf::Sprite sprite;

public:
	PokemonPlacement(sf::Texture* texture)
	{
		sprite.setTexture(*texture);
		sprite.setOrigin(sprite.getLocalBounds().getSize() / 2.f);
	}
	PlayerPokemon* getPokemon();
	void setPokemon(PlayerPokemon* pokemon);

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);
	void draw(sf::RenderWindow& window);

	bool contains(sf::Vector2f position);

};



#endif

