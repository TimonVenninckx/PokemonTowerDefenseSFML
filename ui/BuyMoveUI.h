#ifndef BUYMOVEUI_H
#define BUYMOVEUI_H

#include "SFML/Graphics.hpp"
#include "RoundedRectangleShape.h"
#include "AttackUI.h"
#include "../PlayerPokemon.h"
#include "../Player.h"

class BuyMoveUI
{
	sf::RoundedRectangleShape m_box;
	sf::RoundedRectangleShape m_buyButton;
	sf::Text				  m_buyButtonText;
	sf::RoundedRectangleShape m_backButton;
	sf::Text				  m_backButtonText;
	std::vector<AttackUI>     m_attacks;

	Player&					  m_player;
	bool					  m_boughtAttack{ false };
	bool					  m_relearnMoves{ false };
	AttackStats*			  m_currentAttack{ nullptr };


	sf::Text				  m_infoText;
	sf::Text				  m_typeText;
	sf::Text				  m_powerText;
	sf::Text				  m_accuracyText;
	sf::Text				  m_attackCategoryText;
	sf::Text				  m_attackCooldownText;


public:
	BuyMoveUI(Player& player);
	void loadMoves(PlayerPokemon* pokemon, bool relearnMoves);
	void draw(sf::RenderWindow& window);
	bool doneUpdate(sf::Event& event, sf::Vector2f mousePos);
	void updateActiveAttack(AttackStats* attack);


	AttackStats* learnedAttack();

};





#endif
