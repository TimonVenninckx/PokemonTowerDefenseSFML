#ifndef POKEMONVIEWMENU_H
#define POKEMONVIEWMENU_H

#include <SFML/Graphics.hpp>
#include "../PlayerPokemon.h"
#include "AttackUI.h"
#include "RoundedRectangleShape.h"
#include "LearnMoveUI.h"
#include "BuyMoveUI.h"
#include "../Player.h"
#include "TwoOptionMenu.h"

class PokemonViewMenu
{
protected:
	Player& m_player;
	sf::Sprite m_sprite;
	sf::RoundedRectangleShape	m_spriteBackground;
	sf::RoundedRectangleShape	m_attacksBackground;	
	sf::RoundedRectangleShape	m_buttonsBackground;
	sf::RoundedRectangleShape	m_backButton;
	sf::Text					m_backButtonText;
	sf::RoundedRectangleShape	m_relearnButton;
	sf::Text					m_relearnButtonText;
	sf::RoundedRectangleShape	m_hmButton;
	sf::Text					m_hmButtonText;


	sf::RoundedRectangleShape	m_releaseButton;
	sf::Text					m_releaseButtonText;
	TwoOptionMenu*				m_releaseMenu;


	sf::Text				  m_infoText;
	sf::Text				  m_typeText;
	sf::Text				  m_powerText;
	sf::Text				  m_accuracyText;
	sf::Text				  m_attackCategoryText;
	sf::Text				  m_attackCooldownText;
	sf::Text				  m_moneyText;

	sf::Text				  m_pokemonType1Text;
	sf::Text				  m_pokemonType2Text;


	bool m_active{ false };
	bool m_buyingMove{ false };
	AttackStats* m_currentAttack{};
	std::array<AttackUI, 4>   m_attacks;
	
	LearnMoveUI m_learnMoveUI;
	BuyMoveUI   m_buyMoveUI;


	bool m_releasedPokemon{ false };
	PlayerPokemon* m_pokemon{};
	

public:
	PokemonViewMenu(Player& player);
	void updateActiveAttack(AttackStats* attack);
	void draw(sf::RenderWindow& window);
	bool doneUpdate(sf::Event& event, sf::Vector2f mousePos);
	void setPokemon(PlayerPokemon* pokemon);
	PlayerPokemon* getPokemon();
	bool isActive();
	bool released();
};





#endif
