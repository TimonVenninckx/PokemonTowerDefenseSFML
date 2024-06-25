#ifndef LevelPokemonUI_H
#define LevelPokemonUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "AttackUI.h"
#include "../PlayerPokemon.h"
#include "EvolutionUI.h"
#include "LearnMoveUI.h"
#include <array>
#include "../Player.h"

class LevelPokemonUI
{
private:
	Player& m_player;
	std::array<AttackUI, 4>   m_attacks;
	AttackStats* m_currentAttack{};
	sf::RectangleShape		  m_box;
	sf::Text				  m_infoText;
	sf::Text				  m_typeText;
	sf::Text				  m_powerText;
	sf::Text				  m_accuracyText;
	sf::Text				  m_attackCategoryText;
	sf::Text				  m_attackCooldownText;
	sf::Text				  m_levelUpText;
	sf::Text				  m_levelUpCostText;
	sf::Text				  m_moneyText;

	sf::RoundedRectangleShape m_levelUpButton;
	sf::RoundedRectangleShape m_doneButton;
	sf::Text			      m_doneButtonText;
	PlayerPokemon* m_pokemon{ nullptr };

	EvolutionUI m_evolutionUI;
	LearnMoveUI m_learnMoveUI;

	sf::Music m_song;

	void updateActiveAttack(AttackStats* attack);
	bool levelUpStuff();
public:
	LevelPokemonUI(Player& player);
	void updateMoney();
	void setPokemon(PlayerPokemon* pokemon);
	void draw(sf::RenderWindow& window);
	bool exitUI(sf::Vector2f mousePos);
};


#endif