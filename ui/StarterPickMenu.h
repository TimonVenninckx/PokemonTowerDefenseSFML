#ifndef STARTERPICKMENU_H
#define STARTERPICKMENU_H

#include "GameUI.h"
#include "PokemonPickIcon.h"


class StarterPickMenu
{
	sf::Text m_text;
	std::array<PokemonPickIcon, 3> m_starterIcons;
	std::array<PlayerPokemon*, 3> m_starters{};
	PlayerPokemon* m_pickedPokemon{};


	sf::RoundedRectangleShape m_backButton;
	sf::Text m_backText;
public:
	StarterPickMenu();
	void draw(sf::RenderWindow& window);
	PlayerPokemon* chosePokemon(sf::Vector2f mousePos);
	bool clickedBackButton(sf::Vector2f mousePos);
	~StarterPickMenu();
};



#endif
