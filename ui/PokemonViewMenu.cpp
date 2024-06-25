#include "PokemonViewMenu.h"
#include "GameUI.h"
#include <iomanip>
#include <sstream>

PokemonViewMenu::PokemonViewMenu(Player& player)
	: m_player{ player }
	, m_attacks{
		{
			{{75.f,660.f}, {400.f,80.f}},
			{{75.f,740.f}, {400.f,80.f}},
			{{75.f,820.f}, {400.f,80.f}},
			{{75.f,900.f}, {400.f,80.f}}
		}
	}
	, m_learnMoveUI{ {460.f,100.f},{1000.f,550.f} }
	, m_buyMoveUI{ m_player }

{
	
	m_attacksBackground.setCornerPointCount(5);
	m_attacksBackground.setCornersRadius(25);
	m_attacksBackground.setSize({ 1820.f, 370.f });
	m_attacksBackground.setPosition(50.f, 635.f);
	m_attacksBackground.setFillColor(GameUI::backgroundColor);

	m_buttonsBackground.setCornerPointCount(5);
	m_buttonsBackground.setCornersRadius(25);
	m_buttonsBackground.setSize({ 1365.f, 440.f });
	m_buttonsBackground.setPosition(505.f, 180.f);
	m_buttonsBackground.setFillColor(GameUI::backgroundColor);


	m_spriteBackground.setCornerPointCount(5);
	m_spriteBackground.setCornersRadius(25);
	m_spriteBackground.setSize({ 440.f, 440.f });
	m_spriteBackground.setPosition(270.f, 400.f);
	m_spriteBackground.setOrigin(m_spriteBackground.getGlobalBounds().getSize() / 2.f);
	m_spriteBackground.setFillColor(GameUI::backgroundColor);

	m_sprite.setPosition(270.f, 400.f);
	m_sprite.setScale(4.f, 4.f);


	m_backButton.setCornerPointCount(5);
	m_backButton.setCornersRadius(25);
	m_backButton.setSize({ 340.f, 140.f });
	m_backButton.setOrigin(m_backButton.getGlobalBounds().getSize() / 2.f);
	m_backButton.setPosition(220.f, 90.f);
	m_backButton.setFillColor(GameUI::buttonColor);

	m_backButtonText.setFont(GameUI::font);
	m_backButtonText.setCharacterSize(100);
	m_backButtonText.setString("BACK");
	m_backButtonText.setOrigin(m_backButtonText.getGlobalBounds().getSize().x / 2.f, m_backButtonText.getGlobalBounds().getSize().y / 1.4f);
	m_backButtonText.setPosition(220.f, 90.f);


	m_hmButton.setCornerPointCount(5);
	m_hmButton.setCornersRadius(25);
	m_hmButton.setSize({ 450.f, 190.f });
	m_hmButton.setOrigin(m_hmButton.getGlobalBounds().getSize() / 2.f);
	m_hmButton.setPosition(m_buttonsBackground.getPosition() + sf::Vector2f{ 1115.f,120.f });
	m_hmButton.setFillColor(GameUI::buttonColor);

	m_hmButtonText.setFont(GameUI::font);
	m_hmButtonText.setCharacterSize(100);
	m_hmButtonText.setString("HM/TM");
	m_hmButtonText.setOrigin(m_hmButtonText.getGlobalBounds().getSize().x / 2.f, m_hmButtonText.getGlobalBounds().getSize().y / 1.4f);
	m_hmButtonText.setPosition(m_hmButton.getPosition());


	m_relearnButton.setCornerPointCount(5);
	m_relearnButton.setCornersRadius(25);
	m_relearnButton.setSize({ 450.f, 190.f });
	m_relearnButton.setOrigin(m_relearnButton.getGlobalBounds().getSize() / 2.f);
	m_relearnButton.setPosition(m_buttonsBackground.getPosition() + sf::Vector2f{ 1115.f,320.f });
	m_relearnButton.setFillColor(GameUI::buttonColor);

	m_relearnButtonText.setFont(GameUI::font);
	m_relearnButtonText.setCharacterSize(100);
	m_relearnButtonText.setString("Relearn");
	m_relearnButtonText.setOrigin(m_relearnButtonText.getGlobalBounds().getSize().x / 2.f, m_relearnButtonText.getGlobalBounds().getSize().y / 1.4f);
	m_relearnButtonText.setPosition(m_relearnButton.getPosition());


	m_releaseButton.setCornerPointCount(5);
	m_releaseButton.setCornersRadius(25);
	m_releaseButton.setSize({ 450.f, 190.f });
	m_releaseButton.setOrigin(m_releaseButton.getGlobalBounds().getSize() / 2.f);
	m_releaseButton.setPosition(m_buttonsBackground.getPosition() + sf::Vector2f{ 640.f,320.f });
	m_releaseButton.setFillColor(GameUI::buttonColor);

	m_releaseButtonText.setFont(GameUI::font);
	m_releaseButtonText.setCharacterSize(100);
	m_releaseButtonText.setString("Release");
	m_releaseButtonText.setOrigin(m_releaseButtonText.getGlobalBounds().getSize().x / 2.f, m_releaseButtonText.getGlobalBounds().getSize().y / 1.4f);
	m_releaseButtonText.setPosition(m_releaseButton.getPosition());


	m_pokemonType1Text.setFont(GameUI::font);
	m_pokemonType1Text.setCharacterSize(40);

	m_pokemonType2Text.setFont(GameUI::font);
	m_pokemonType2Text.setCharacterSize(40);

	m_moneyText.setPosition(m_buttonsBackground.getPosition() +  sf::Vector2f{ 25.f,25.f });
	m_moneyText.setFont(GameUI::font);
	m_moneyText.setCharacterSize(50);
	m_moneyText.setString("Balance: 100000");

	// text
	m_infoText.setPosition({ 485.f,660.f });
	m_infoText.setFont(GameUI::font);
	m_infoText.setCharacterSize(35);

	m_typeText.setPosition({ 485.f,920.f });
	m_typeText.setFont(GameUI::font);
	m_typeText.setCharacterSize(35);
	m_powerText.setPosition({ 485.f,880.f });
	m_powerText.setFont(GameUI::font);
	m_powerText.setCharacterSize(35);
	m_attackCooldownText.setPosition({ 485.f,840.f });
	m_attackCooldownText.setFont(GameUI::font);
	m_attackCooldownText.setCharacterSize(35);


	m_attackCategoryText.setPosition({ 1085.f,880.f });
	m_attackCategoryText.setFont(GameUI::font);
	m_attackCategoryText.setCharacterSize(35);
	m_accuracyText.setPosition({ 1085.f,920.f });
	m_accuracyText.setFont(GameUI::font);
	m_accuracyText.setCharacterSize(35);


}

PlayerPokemon* PokemonViewMenu::getPokemon()
{
	return m_pokemon;
}

bool PokemonViewMenu::released()
{
	return m_releasedPokemon;
}



void PokemonViewMenu::updateActiveAttack(AttackStats* attack) {
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



bool PokemonViewMenu::isActive()
{
	return m_active;
}

bool PokemonViewMenu::doneUpdate(sf::Event& event, sf::Vector2f mousePos)
{
	if (m_releaseMenu) {
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if (int option = m_releaseMenu->pickedOption(mousePos)) {
				delete m_releaseMenu;
				m_releaseMenu = nullptr;
				m_releasedPokemon = true;
				if (option == 1) {
					m_active = false;
					return true;
				}
			}
		}
		return false;
	}

	if (m_buyingMove) {
		if (m_buyMoveUI.doneUpdate(event, mousePos)) {
			m_buyingMove = false;
			m_learnMoveUI.learnMove(m_pokemon, m_buyMoveUI.learnedAttack());
		}
		return false;
	}
	if (m_learnMoveUI.isLearning()) {
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if (m_learnMoveUI.finished(mousePos)) {
				updateActiveAttack(m_pokemon->getCurrentAttack());
				std::array<AttackStats*, 4>* attacks = m_pokemon->getAttacks();
				for (int i{ 0 }; i < 4; ++i) {
					m_attacks[i].setAttackStats((*attacks)[i]);
					if ((*attacks)[i] == m_currentAttack) {
						m_attacks[i].Activate(true);
					}
					else // set all other attacks to false;
						m_attacks[i].Activate(false);
				}
			}
		}
		return false;
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (m_backButton.getGlobalBounds().contains(mousePos)) {
			GameUI::clickSound.play();
			m_active = false;
			return true;
		}
		else if (m_relearnButton.getGlobalBounds().contains(mousePos)) {
			GameUI::clickSound.play();
			m_buyingMove = true;
			m_buyMoveUI.loadMoves(m_pokemon, true);
			return false;
		}
		else if (m_hmButton.getGlobalBounds().contains(mousePos)) {
			GameUI::clickSound.play();
			m_buyingMove = true;
			m_buyMoveUI.loadMoves(m_pokemon, false);
			return false;
		}
		else if (m_releaseButton.getGlobalBounds().contains(mousePos)) {
			m_releaseMenu = new TwoOptionMenu;
			m_releaseMenu->setButton1Text("Release");
			m_releaseMenu->setButton2Text("No");
			m_releaseMenu->setInfoText("Are you sure you want to release\n" + m_pokemon->getName() + "?");
			return false;
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

	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { // release pokemon

	}
	return false;
}

void PokemonViewMenu::setPokemon(PlayerPokemon* pokemon)
{
	if (pokemon == nullptr)
		return;
	m_pokemon = pokemon;
	m_releasedPokemon = false;
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
	m_active = true;

	m_sprite.setTexture(*m_pokemon->getTexture());
	m_sprite.setTextureRect({ {0,0}, sf::Vector2i{m_pokemon->getTexture()->getSize()} / 4 });
	m_sprite.setOrigin(sf::Vector2f(m_sprite.getTextureRect().getSize()) / 2.f);


	m_pokemonType1Text.setString(PokemonStats::getTypeString(pokemon->getStats().type1));
	m_pokemonType2Text.setString(PokemonStats::getTypeString(pokemon->getStats().type2));
	m_pokemonType1Text.setFillColor(PokemonStats::getTypeColor(pokemon->getStats().type1));
	m_pokemonType2Text.setFillColor(PokemonStats::getTypeColor(pokemon->getStats().type2));
	m_pokemonType1Text.setOrigin(m_pokemonType1Text.getGlobalBounds().getSize() / 2.f);
	m_pokemonType2Text.setOrigin(m_pokemonType2Text.getGlobalBounds().getSize() / 2.f);

	if (pokemon->getStats().type2 != PokemonStats::Types::NONE) {
		m_pokemonType1Text.setPosition( 175,550 );
		m_pokemonType2Text.setPosition( 375,550 );
	}
	else
		m_pokemonType1Text.setPosition(275, 550);


	m_moneyText.setString("Balance: $" + std::to_string(m_player.getMoney()));
}

void PokemonViewMenu::draw(sf::RenderWindow& window)
{
	window.draw(m_spriteBackground);
	window.draw(m_attacksBackground);
	window.draw(m_buttonsBackground);
	window.draw(m_sprite);
	window.draw(m_pokemonType1Text);
	window.draw(m_pokemonType2Text);

	window.draw(m_hmButton);
	window.draw(m_hmButtonText);
	window.draw(m_relearnButton);
	window.draw(m_relearnButtonText);
	window.draw(m_releaseButton);
	window.draw(m_releaseButtonText);
	for (auto attackinfo : m_attacks)
		attackinfo.draw(window);

	window.draw(m_infoText);
	window.draw(m_typeText);
	window.draw(m_powerText);
	window.draw(m_accuracyText);
	window.draw(m_attackCategoryText);
	window.draw(m_attackCooldownText);
	window.draw(m_moneyText);

	if (!m_learnMoveUI.isLearning() && !m_buyingMove) {
		window.draw(m_backButton);
		window.draw(m_backButtonText);
	}


	if (m_learnMoveUI.isLearning()) {
		m_learnMoveUI.draw(window);
	}
	if (m_buyingMove)
		m_buyMoveUI.draw(window);

	if (m_releaseMenu)
		m_releaseMenu->draw(window);
}


