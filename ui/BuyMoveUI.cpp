#include "BuyMoveUI.h"
#include "GameUI.h"
#include "../sqlite/Database.h"
#include <iomanip>


BuyMoveUI::BuyMoveUI(Player& player)
	: m_player{ player }
{
	m_box.setCornerPointCount(5);
	m_box.setCornersRadius(25.f);
	m_box.setPosition(500.f,50.f);
	m_box.setSize({ 1370.f ,980.f });
	m_box.setFillColor(GameUI::backgroundColor);
	m_box.setOutlineThickness(5.f);
	m_box.setOutlineColor(GameUI::buttonColor);

	m_backButton.setCornerPointCount(5);
	m_backButton.setCornersRadius(25.f);
	m_backButton.setSize({ 340.f, 140.f });
	m_backButton.setOrigin(m_backButton.getGlobalBounds().getSize() / 2.f);
	m_backButton.setPosition(m_box.getPosition() + sf::Vector2f{ 1180.f, 90.f });
	m_backButton.setFillColor(GameUI::buttonColor);

	m_backButtonText.setFont(GameUI::font);
	m_backButtonText.setCharacterSize(100);
	m_backButtonText.setString("BACK");
	m_backButtonText.setOrigin(m_backButtonText.getGlobalBounds().getSize().x / 2.f, m_backButtonText.getGlobalBounds().getSize().y / 1.4f);
	m_backButtonText.setPosition(m_box.getPosition() + sf::Vector2f{ 1180.f, 90.f });


	m_buyButton.setCornerPointCount(5);
	m_buyButton.setCornersRadius(25.f);
	m_buyButton.setSize({ 340.f, 140.f });
	m_buyButton.setOrigin(m_buyButton.getGlobalBounds().getSize() / 2.f);
	m_buyButton.setPosition(m_box.getPosition() + sf::Vector2f{ 1180.f, 890.f });
	m_buyButton.setFillColor(GameUI::buttonColor);

	m_buyButtonText.setFont(GameUI::font);
	m_buyButtonText.setCharacterSize(70);
	m_buyButtonText.setPosition(m_box.getPosition() + sf::Vector2f{ 1180.f, 890.f });




	m_infoText.setPosition(m_box.getPosition() + sf::Vector2f{ 450.f, 200.f });
	m_infoText.setFont(GameUI::font);
	m_infoText.setCharacterSize(35);

	m_typeText.setPosition(m_box.getPosition() + sf::Vector2f{ 450.f, 150.f });
	m_typeText.setFont(GameUI::font);
	m_typeText.setCharacterSize(35);
	m_powerText.setPosition(m_box.getPosition() + sf::Vector2f{ 450.f, 400.f });
	m_powerText.setFont(GameUI::font);
	m_powerText.setCharacterSize(35);
	m_attackCooldownText.setPosition(m_box.getPosition() + sf::Vector2f{ 450.f, 450.f });
	m_attackCooldownText.setFont(GameUI::font);
	m_attackCooldownText.setCharacterSize(35);

	m_attackCategoryText.setPosition(m_box.getPosition() + sf::Vector2f{ 450.f, 500.f });
	m_attackCategoryText.setFont(GameUI::font);
	m_attackCategoryText.setCharacterSize(35);
	m_accuracyText.setPosition(m_box.getPosition() + sf::Vector2f{ 450.f, 550.f });
	m_accuracyText.setFont(GameUI::font);
	m_accuracyText.setCharacterSize(35);

}

void BuyMoveUI::loadMoves(PlayerPokemon* pokemon, bool relearnMoves)
{
	m_relearnMoves = relearnMoves; // for price calc
	m_boughtAttack = false;
	if (relearnMoves) {
		m_buyButtonText.setString("5000");
		m_buyButtonText.setOrigin(m_buyButtonText.getGlobalBounds().getSize().x / 2.f, m_buyButtonText.getGlobalBounds().getSize().y / 1.4f);
		
		std::cout << Database::getRelearnAttacks(pokemon->getStats().id, pokemon->getStats().level).size() << " attacks can be relearned\n";
		std::vector<AttackStats*> attacks{ Database::getRelearnAttacks(pokemon->getStats().id, pokemon->getStats().level) };
		m_attacks.clear();
		for (int i{ 0 }; i < attacks.size(); i++) {
			bool alreadyLearned = false;
			for (int j{ 0 }; j < 4; j++) {
				if (attacks[i] == (*pokemon->getAttacks())[j]) {
					alreadyLearned = true;
					break;
				}
			}
			if (alreadyLearned)
				continue;
			AttackUI attackui({ m_box.getPosition() + sf::Vector2f{25.f, 25.f + (80 * i)},{400.f,80.f},attacks[i] });
			m_attacks.emplace_back(attackui);
		}
	}
	else {
		m_buyButtonText.setString("10000");
		m_buyButtonText.setOrigin(m_buyButtonText.getGlobalBounds().getSize().x / 2.f, m_buyButtonText.getGlobalBounds().getSize().y / 1.4f);
		
		std::cout << Database::getRelearnAttacks(pokemon->getStats().id, pokemon->getStats().level).size() << " attacks can be relearned\n";
		std::vector<AttackStats*> attacks{ Database::getTMAttacks(pokemon->getStats().id) };
		m_attacks.clear();
		int alreadyLearnedMoves{ 0 }; // for i offset if we already learned a move;
		for (int i{ 0 }; i < attacks.size(); i++) {
			bool alreadyLearned = false;
			for (int j{ 0 }; j < 4; j++) {
				if (attacks[i] == (*pokemon->getAttacks())[j]) {
					alreadyLearned = true;
					break;
				}
			}
			if (alreadyLearned) {
				alreadyLearnedMoves++;
				continue;
			}
			AttackUI attackui({ m_box.getPosition() + sf::Vector2f{25.f, 25.f + (80 * i)},{400.f,80.f},attacks[i - alreadyLearnedMoves] });
			m_attacks.emplace_back(attackui);
		}
	}
	if (m_attacks.size() > 0) {
		m_attacks[0].Activate(true);
		updateActiveAttack(m_attacks[0].getAttack());
	}
}

void BuyMoveUI::draw(sf::RenderWindow& window)
{
	window.draw(m_box);
	window.draw(m_backButton);
	window.draw(m_backButtonText);
	window.draw(m_buyButton);
	window.draw(m_buyButtonText);

	for (AttackUI attack : m_attacks) {
		attack.draw(window);
	}
	if (m_attacks.size() == 0)
		return;

	window.draw(m_infoText);
	window.draw(m_typeText);
	window.draw(m_powerText);
	window.draw(m_accuracyText);
	window.draw(m_attackCategoryText);
	window.draw(m_attackCooldownText);
}

bool BuyMoveUI::doneUpdate(sf::Event& event, sf::Vector2f mousePos)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (m_backButton.getGlobalBounds().contains(mousePos)) {
			GameUI::clickSound.play();
			return true;
		}
		if (m_buyButton.getGlobalBounds().contains(mousePos)) {
			if (m_currentAttack) {
				GameUI::clickSound.play();
				m_boughtAttack = true;
			}
			return true;
		}
		
		for (int i{ 0 }; i < m_attacks.size(); ++i) {
			if (m_attacks[i].contains(mousePos)) {
				// activate this attack on pokemon;
				if (!m_attacks[i].getAttack())
					return false;
				if (m_attacks[i].getAttack() == m_currentAttack)
					return false;
				// if we clicked on a valid new attack change attack
				updateActiveAttack(m_attacks[i].getAttack());
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

	return false;
}

void BuyMoveUI::updateActiveAttack(AttackStats* attack)
{
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

AttackStats* BuyMoveUI::learnedAttack()
{
	if (m_boughtAttack)
		return m_currentAttack;
	return nullptr;
}
