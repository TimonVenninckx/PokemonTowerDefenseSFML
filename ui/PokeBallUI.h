#ifndef PokeBallUI_H
#define PokeBallUI_H


#include <SFML/Graphics.hpp>
#include "../LinkedList.h"
#include "../Pokemon.h"

class PokeBallUI
{
	LinkedList<Pokemon>* m_pokemonHead{};
	Pokemon* m_catchingPokemon{};

	sf::Texture m_texture{};
	sf::Sprite m_sprite{};
	bool m_active{ false };

public:
	
	PokeBallUI();

	void setActive(bool active);
	bool isActive();
	Pokemon* tryCatch();
	void setPokemon(LinkedList<Pokemon>* pokemon);
	void setPosition(sf::Vector2f mousePos, sf::Vector2f uiPos);
	bool contains(sf::Vector2f);
	void draw(sf::RenderWindow& window);
	void drawPokemon(sf::RenderWindow& window);

};


#endif
