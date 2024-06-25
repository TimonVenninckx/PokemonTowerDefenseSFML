#ifndef LEARMOVEUI_H
#define LEARMOVEUI_H

#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.h"
#include "../PlayerPokemon.h"
#include "AttackUI.h"

class LearnMoveUI
{
	int m_selectedMove{-1};
	std::array<AttackUI, 5>   m_attacks;

	sf::Vector2f m_position;
	sf::Vector2f m_size;

	sf::RoundedRectangleShape m_box;
	sf::RoundedRectangleShape m_doneButton;
	sf::Text				  m_doneText;

	sf::Text				  m_newMoveText;

	sf::Text				  m_infoText;
	sf::Text				  m_typeText;
	sf::Text				  m_powerText;
	sf::Text				  m_accuracyText;
	sf::Text				  m_attackCategoryText;
	sf::Text				  m_attackCooldownText;

	bool	   m_learning{ false };

	PlayerPokemon* m_pokemon{};

	void updateInfoText(AttackStats* attack);
public:
	LearnMoveUI(sf::Vector2f position, sf::Vector2f size);
	bool isLearning();
	bool learnMove(PlayerPokemon* pokemon, AttackStats* attack);
	bool finished(sf::Vector2f mousePos);
	void draw(sf::RenderWindow& window);


};

#endif
