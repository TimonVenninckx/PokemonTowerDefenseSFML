#include "LevelPokemonUI.h"
#include "GameUI.h"
#include "../sqlite/Database.h"
#include <iomanip>
#include <sstream>

LevelPokemonUI::LevelPokemonUI(Player& player)
	: m_player{ player }
	, m_attacks{
		{
			{{0.f,760.f}, {400.f,80.f}},
			{{0.f,840.f}, {400.f,80.f}},
			{{0.f,920.f}, {400.f,80.f}},
			{{0.f,1000.f}, {400.f,80.f}}
		}
	}
	, m_evolutionUI{ {660.f,200.f},{600.f,400.f} }
	, m_learnMoveUI{ {460.f,100.f},{1000.f,550.f} }
{

	m_box.setPosition(-1000.f, 680);
	//m_box.setSize({ 1920.f, 400.f });
	m_box.setSize({ 4000.f, 1000.f });
	m_box.setFillColor({20, 20, 20, 200});

	// done button
	m_doneButton.setCornersRadius(20);
	m_doneButton.setCornerPointCount(4);
	m_doneButton.setPosition(1660.f, 940.f);
	m_doneButton.setSize({ 250.f, 130.f });
	m_doneButton.setFillColor(GameUI::buttonColor);

	m_doneButtonText.setPosition({ 1680.f,960.f });
	m_doneButtonText.setFont(GameUI::font);
	m_doneButtonText.setCharacterSize(75);
	m_doneButtonText.setString("Done");

	// level up button
	m_levelUpButton.setCornersRadius(20);
	m_levelUpButton.setCornerPointCount(4);
	m_levelUpButton.setPosition(1560.f, 760.f);
	m_levelUpButton.setSize({ 350.f, 170.f });
	m_levelUpButton.setFillColor(sf::Color::Red);

	m_levelUpText.setPosition({ 1580.f,775.f });
	m_levelUpText.setFont(GameUI::font);
	m_levelUpText.setCharacterSize(65);
	m_levelUpText.setString("Level Up!");

	m_levelUpCostText.setPosition({ 1580.f,860.f });
	m_levelUpCostText.setFont(GameUI::font);
	m_levelUpCostText.setCharacterSize(40);
	m_levelUpCostText.setString("Cost: $42069");
	
	// money text

	m_moneyText.setPosition({ 1560.f,700.f });
	m_moneyText.setFont(GameUI::font);
	m_moneyText.setCharacterSize(40);
	m_moneyText.setString("Balance: 100000");

	// text
	m_infoText.setPosition({ 410.f,760.f });
	m_infoText.setFont(GameUI::font);
	m_infoText.setCharacterSize(35);

	m_typeText.setPosition({ 410.f,1020.f });
	m_typeText.setFont(GameUI::font);
	m_typeText.setCharacterSize(35);
	m_powerText.setPosition({ 410.f,980.f });
	m_powerText.setFont(GameUI::font);
	m_powerText.setCharacterSize(35);
	m_attackCooldownText.setPosition({ 410.f,940.f });
	m_attackCooldownText.setFont(GameUI::font);
	m_attackCooldownText.setCharacterSize(35);


	m_attackCategoryText.setPosition({ 1010.f,980.f });
	m_attackCategoryText.setFont(GameUI::font);
	m_attackCategoryText.setCharacterSize(35);
	m_accuracyText.setPosition({ 1010.f,1020.f });
	m_accuracyText.setFont(GameUI::font);
	m_accuracyText.setCharacterSize(35);


	m_song.openFromFile("sound/levelup.mp3");
}

void LevelPokemonUI::updateActiveAttack(AttackStats* attack) {
	m_currentAttack = attack;
	m_infoText.setString(m_currentAttack->info);
	m_typeText.setString("Type: " + PokemonStats::getTypeString(m_currentAttack->type));
	m_typeText.setFillColor(PokemonStats::getTypeColor(m_currentAttack->type));
	std::stringstream stringstream;
	if (attack->power > 0.f)
		stringstream << std::fixed << std::setprecision(0) << "Power: " << m_currentAttack->power;
	m_powerText.setString(stringstream.str());

	stringstream.str(std::string()); 
	stringstream << std::fixed << std::setprecision(2) << "Attack Cooldown: " << m_currentAttack->attackCooldown;
	m_attackCooldownText.setString(stringstream.str());

	stringstream.str(std::string());
	stringstream << std::fixed << std::setprecision(0) << "Accuracy: " << m_currentAttack->accuracy;
	m_accuracyText.setString(stringstream.str());

	m_attackCategoryText.setString("Category: " + AttackStats::getCategoryString(m_currentAttack->category));
}


void LevelPokemonUI::updateMoney() {
	m_player.getMoney();

	if (m_player.getMoney() >= 1000000)
		m_moneyText.setCharacterSize(35);
	m_moneyText.setString("Balance: $"+ std::to_string(m_player.getMoney()));
}


void LevelPokemonUI::setPokemon(PlayerPokemon* pokemon)
{
	m_pokemon = pokemon;
	updateActiveAttack(pokemon->getCurrentAttack());
	std::array<AttackStats*, 4>* attacks = pokemon->getAttacks();
	for (int i{ 0 }; i < 4; ++i) {
		m_attacks[i].setAttackStats((*attacks)[i]);
		if ((*attacks)[i] == m_currentAttack) {
			m_attacks[i].Activate(true);
		}
		else // set all other attacks to false;
			m_attacks[i].Activate(false);
	}
	m_levelUpCostText.setString("Cost: $" + std::to_string(m_pokemon->getLevelUpCost()));
}




bool LevelPokemonUI::levelUpStuff(){

	int moneyNeeded = m_pokemon->getLevelUpCost();
	if (m_player.getMoney() < moneyNeeded) {
		return false;
	}
	if (m_pokemon->levelUp()) {
		m_song.setVolume(GameUI::sfxVolume);
		m_song.play();
		m_levelUpCostText.setString("Cost: $" + std::to_string(m_pokemon->getLevelUpCost()));
		m_player.addMoney(-moneyNeeded);
		
		if (m_evolutionUI.startEvolution(m_pokemon))
			return false;

		AttackStats* attack = Database::getPokemonAttack(m_pokemon->getStats().id, m_pokemon->getStats().level);

		if (attack) { // if attack is valid add it
			m_learnMoveUI.learnMove(m_pokemon, attack);
			return false;
		}
		return true;
	}
	return false;
}

bool LevelPokemonUI::exitUI(sf::Vector2f mousePos)
{
	if (m_evolutionUI.isEvolving()) {
		if (m_evolutionUI.cancelEvolution(mousePos)) {
			AttackStats* attack = Database::getPokemonAttack(m_pokemon->getStats().id, m_pokemon->getStats().level);
			if (attack) { // if attack is valid try to add it
				m_learnMoveUI.learnMove(m_pokemon, attack);
			}
		}
		return false;
	}
	if (m_learnMoveUI.isLearning()) {
		return m_learnMoveUI.finished(mousePos);
		//return false;
	}

	if (m_doneButton.getGlobalBounds().contains(mousePos)) {
		GameUI::clickSound.play();
		return true;
	}
	if (m_levelUpButton.getGlobalBounds().contains(mousePos)) {
		return levelUpStuff(); // click sound in function
	}

	for (int i{ 0 }; i < m_attacks.size(); ++i) {
		if (m_attacks[i].contains(mousePos)) {
			// activate this attack on pokemon;
			if (!m_attacks[i].getAttack())
				return false;
			if (m_attacks[i].getAttack() == m_currentAttack)
				return false;
			// if we clicked on a valid new attack change attack
			GameUI::clickSound.play();
			updateActiveAttack(m_attacks[i].getAttack());

			m_pokemon->setCurrentAttack(m_currentAttack);
			m_attacks[i].Activate(true); // ui

			for (int j{ 0 }; j < m_attacks.size(); ++j) {
				if (!(i == j)) {
					m_attacks[j].Activate(false);
				}
			}
			return false;
		}
	}
	if (m_box.getGlobalBounds().contains(mousePos))
		return false;

	return true;
}

void LevelPokemonUI::draw(sf::RenderWindow& window)
{

	window.draw(m_box);
	for (auto attackinfo : m_attacks)
		attackinfo.draw(window);

	window.draw(m_infoText);
	window.draw(m_typeText);
	window.draw(m_powerText);
	window.draw(m_accuracyText);
	window.draw(m_attackCategoryText);
	window.draw(m_attackCooldownText);

	if (m_pokemon->canLevelUp() && m_pokemon->getLevelUpCost() <= m_player.getMoney()) {
		m_levelUpButton.setFillColor(sf::Color::Red);
	}
	else {
		m_levelUpButton.setFillColor(sf::Color(30, 30, 30, 220));
	}
	window.draw(m_levelUpButton);
	window.draw(m_levelUpText);
	window.draw(m_levelUpCostText);
	window.draw(m_moneyText);
	window.draw(m_doneButton);
	window.draw(m_doneButtonText);

	if (m_evolutionUI.isEvolving()) {
		m_evolutionUI.draw(window);
	}
	if (m_learnMoveUI.isLearning()) {
		m_learnMoveUI.draw(window);
	}

}
