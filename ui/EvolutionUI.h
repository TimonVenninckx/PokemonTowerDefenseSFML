#ifndef EVOLUTIONUI_H
#define EVOLUTIONUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "RoundedRectangleShape.h"
#include "../PlayerPokemon.h"

class EvolutionUI
{
	sf::Vector2f m_position;
	sf::Vector2f m_size;

	sf::RoundedRectangleShape m_box;
	sf::RoundedRectangleShape m_cancelButton;
	sf::Text				  m_cancelText;
	sf::Text				  m_evolutionText;
	sf::Sprite m_currentPokemon;
	sf::Sprite m_evolvedPokemon;
	bool	   m_evolving{ false };
	bool	   m_doneEvolving{ false };
	int		   m_evolutionId{};
	float	   m_evolutionTime{};

	sf::Clock  m_clock;
	sf::Music  m_song;

	PlayerPokemon* m_pokemon{};
public:
	EvolutionUI(sf::Vector2f position, sf::Vector2f size);
	bool isEvolving();
	bool startEvolution(PlayerPokemon* pokemon);
	bool cancelEvolution(sf::Vector2f mousePos);
	void draw(sf::RenderWindow& window);
};



#endif