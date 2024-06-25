#ifndef POKEMONPICKMENU_H
#define POKEMONPICKMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "PokemonPickIcon.h"
#include "../Player.h"
#include <vector>
#include "RoundedRectangleShape.h"
#include "PokemonViewMenu.h"
#include "AttackUI.h"

class PokemonPickMenu
{
private:
	std::vector<PokemonPickIcon> m_reservePokemonIcons;
	std::array<PokemonPickIcon,6> m_activePokemonIcons;

	PokemonPickIcon*			m_draggingIcon;
	bool						m_dragging{ false };
	bool						m_dragginActivePokemon{ false }; // because there is a view coordinate difference for clicking on pokemon


	sf::RoundedRectangleShape	m_doneButton;
	sf::Text					m_doneButtonText;
	sf::RoundedRectangleShape	m_backButton;
	sf::Text					m_backButtonText;


	sf::RectangleShape			m_topBox;

	sf::RoundedRectangleShape	m_scrollBar;
	bool						m_scrolling{ false };
	float						m_scrollOffset;

	sf::View					m_scrollBarview;
	sf::View&					m_view;

	Player&						m_player;
	sf::RenderWindow&			m_window;
	PokemonViewMenu				m_viewMenu;

public:
	PokemonPickMenu(Player& player, sf::View& view, sf::RenderWindow& window);
	void reload();
	void recalculateScrollWindow();
	bool doneUpdate(sf::Event& event,sf::Vector2f mousePos, float delta);
	void draw(sf::RenderWindow& window);
	bool startLevel(sf::Vector2f mousePos);
};



#endif
