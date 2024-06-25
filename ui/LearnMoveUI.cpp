#include "LearnMoveUI.h"
#include "GameUI.h"
#include <iostream>
#include <iomanip>
#include <sstream>



LearnMoveUI::LearnMoveUI(sf::Vector2f position, sf::Vector2f size)
	: m_attacks{
		{
			{position + sf::Vector2f{20.f,100.f}, {400.f,80.f}	},
			{position + sf::Vector2f{20.f,180.f}, {400.f,80.f}	},
			{position + sf::Vector2f{20.f,260.f}, {400.f,80.f}	},
			{position + sf::Vector2f{20.f,340.f}, {400.f,80.f}	},
			{position + sf::Vector2f{20.f,440.f}, {400.f,80.f}	}
		}
	}
{
	m_box.setPosition(position);
	m_box.setSize(size);
	m_box.setCornerPointCount(4);
	m_box.setCornersRadius(15.f);
	m_box.setFillColor(GameUI::backgroundColor);
	m_box.setOutlineThickness(5);
	m_box.setOutlineColor(GameUI::buttonColor);

	m_doneButton.setSize({ size.x / 4.f, size.y / 5.f });
	m_doneButton.setPosition(position + size - (m_doneButton.getSize() +  sf::Vector2f{ 20.f,20.f }));
	m_doneButton.setCornerPointCount(4);
	m_doneButton.setCornersRadius(15.f);
	m_doneButton.setFillColor(GameUI::buttonColor);

	m_doneText.setFont(GameUI::font);
	m_doneText.setCharacterSize(static_cast<int>(m_doneButton.getSize().y / 1.5f));
	m_doneText.setOrigin(m_doneText.getGlobalBounds().getSize() / 2.f);
	m_doneText.setPosition(m_doneButton.getPosition() + sf::Vector2f{m_doneButton.getSize().x / 6.f, m_doneButton.getSize().y / 8.f});
	m_doneText.setString("Done");

	// move info text


	m_newMoveText.setPosition({ position.x + size.x / 2.f,position.y + 40.f });
	m_newMoveText.setFont(GameUI::font);
	m_newMoveText.setCharacterSize(50);

	sf::Vector2f halfway = position + sf::Vector2f{size.x / 2.2f,100.f };

	m_typeText.setPosition({ halfway.x,halfway.y });
	m_typeText.setFont(GameUI::font);

	m_infoText.setPosition({ halfway.x,halfway.y + 50.f });
	m_infoText.setFont(GameUI::font);
	m_infoText.setCharacterSize(22);

	m_powerText.setPosition({ halfway.x,halfway.y + 150 });
	m_powerText.setFont(GameUI::font);
	m_attackCooldownText.setPosition({ halfway.x,halfway.y + 200.f });
	m_attackCooldownText.setFont(GameUI::font);


	m_attackCategoryText.setPosition({ halfway.x,halfway.y + 250.f });
	m_attackCategoryText.setFont(GameUI::font);
	m_accuracyText.setPosition({ halfway.x,halfway.y + 300.f });
	m_accuracyText.setFont(GameUI::font);
}

void LearnMoveUI::updateInfoText(AttackStats* attack)
{
	m_newMoveText.setString((m_attacks[4].getAttack() == nullptr? "Learned: " : "Learning: ") + attack->name);

	m_newMoveText.setOrigin(m_newMoveText.getGlobalBounds().getSize() / 2.f);
	m_newMoveText.setFillColor(PokemonStats::getTypeColor(attack->type));

	m_infoText.setString(attack->info);
	m_typeText.setString("Type: " + PokemonStats::getTypeString(attack->type));
	m_typeText.setFillColor(PokemonStats::getTypeColor(attack->type));

	std::stringstream stringstream;
	stringstream << std::fixed << std::setprecision(0) << "Power: " << attack->power;
	m_powerText.setString(stringstream.str());

	stringstream.str(std::string());
	stringstream << std::fixed << std::setprecision(2) << "Cooldown: " << attack->attackCooldown;
	m_attackCooldownText.setString(stringstream.str());

	stringstream.str(std::string());
	stringstream << std::fixed << std::setprecision(0) << "Accuracy: " << attack->accuracy;
	m_accuracyText.setString(stringstream.str());

	m_attackCategoryText.setString("Category: " + AttackStats::getCategoryString(attack->category));
}

bool LearnMoveUI::isLearning()
{
	return m_learning;
}

bool LearnMoveUI::learnMove(PlayerPokemon* pokemon, AttackStats* attack)
{
	m_pokemon = pokemon;
	m_selectedMove = -1;
	if (attack == nullptr)
		return false;

	std::array<AttackStats*, 4>* attacks = m_pokemon->getAttacks();
	for (int i{ 0 }; i < 4; ++i) {
		if ((*attacks)[i] == nullptr) { // check if attack is valid not nullptr
			if (m_selectedMove != -1)
				break;
			m_attacks[i].setAttackStats(attack);
			m_attacks[i].Activate(true);
			m_selectedMove = i;
			continue;
		}
		else if(attack->id == (*attacks)[i]->id) // chec if already learned this move
			return false; 
		else {
			m_attacks[i].setAttackStats((*attacks)[i]);
			m_attacks[i].Activate(false);
		}
	}

	if (m_selectedMove == -1) {
		m_attacks[4].setAttackStats(attack);
		m_attacks[4].Activate(true);
		m_selectedMove = 4;
	}
	m_learning = true;

	updateInfoText(attack);

	return true;
}

bool LearnMoveUI::finished(sf::Vector2f mousePos)
{
	if (m_doneButton.getGlobalBounds().contains(mousePos)) {
		GameUI::clickSound.play();
		m_learning = false;
		// update pokemon
		std::array<AttackStats*, 4> attacks;
		for (int i{ 0 }; i < 4; ++i) {
			attacks[i] = m_attacks[i].getAttack();
		}
		m_pokemon->updateAttacks(attacks);

		return true;
	}
	if (m_attacks[4].getAttack() == nullptr) {
		return false;
	}


	for (int i{ 0 }; i < m_attacks.size(); i++) {
		if (m_attacks[i].contains(mousePos)) {
			if (m_selectedMove == -1) { // if no move selected yet
				m_attacks[i].Activate(true);
				m_selectedMove = i;
				std::cout << "hit once more" << i << " \n";
			}
			else {
				GameUI::clickSound.play();
				std::cout << "Swapping \n";
				AttackStats* temp = m_attacks[i].getAttack();
				m_attacks[i].setAttackStats(m_attacks[m_selectedMove].getAttack());
				m_attacks[m_selectedMove].setAttackStats(temp);
				m_attacks[i].Activate(false);
				m_attacks[m_selectedMove].Activate(false);

				if (i == 4 || m_selectedMove == 4)
					updateInfoText(m_attacks[4].getAttack());
				m_selectedMove = -1;
				break;
			}
		}
	}

	return false;
}

void LearnMoveUI::draw(sf::RenderWindow& window)
{
	window.draw(m_box);
	window.draw(m_doneButton);
	window.draw(m_doneText);

	window.draw(m_newMoveText);

	window.draw(m_infoText);
	window.draw(m_typeText);
	window.draw(m_powerText);
	window.draw(m_accuracyText);
	window.draw(m_attackCategoryText);
	window.draw(m_attackCooldownText);

	for (auto attack : m_attacks)
		attack.draw(window);

}
