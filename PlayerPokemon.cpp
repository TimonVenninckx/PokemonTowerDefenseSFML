#include <iostream>
#include "PlayerPokemon.h"
#include "GameMath.h"
#include "ui/PokemonPlacement.h"
#include "Level.h"
//#include "PokemonPlaceUI.h"

PlayerPokemon::PlayerPokemon(PokemonStats* baseStats,int level, sf::Texture* texture, std::array<AttackStats*, 4>& attacks)
	: Pokemon{ baseStats,level,texture,attacks }
{
	m_canAttack = true;

	m_healthBar.updateLevel(level);

	m_rangeCircle.setPosition(m_sprite.getPosition());
	m_rangeCircle.setOrigin(m_range, m_range);
	m_rangeCircle.setRadius(m_range);
	m_rangeCircle.setFillColor({ 10,10,10,150 });
	
	//m_rangeCircle.setOutlineColor(sf::Color::Red);
	//m_rangeCircle.setOutlineThickness(2);

	m_moving = false;
}

PlayerPokemon::PlayerPokemon(Pokemon* pokemon)
	: Pokemon{pokemon->getBaseStats(),pokemon->getStats().level,pokemon->getTexture(),*pokemon->getAttacks()}
{

	m_canAttack = true;

	m_rangeCircle.setPosition(m_sprite.getPosition());
	m_rangeCircle.setOrigin(m_range, m_range);
	m_rangeCircle.setRadius(m_range);
	m_rangeCircle.setFillColor({ 10,10,10,150 });

	//m_rangeCircle.setOutlineColor(sf::Color::Red);
	//m_rangeCircle.setOutlineThickness(2);

	m_moving = false;
}

void PlayerPokemon::reset() {
	stats.health = stats.maxHealth;
	m_placeholder = nullptr;
	m_ui = nullptr;
	m_pickupItem = nullptr;
}

void PlayerPokemon::update(float delta)
{
	Pokemon::update(delta);
}

void PlayerPokemon::draw(sf::RenderTarget& window)
{
	if (stats.health <= 0)
		return;
	Pokemon::draw(window);
}

void PlayerPokemon::drawRange(sf::RenderTarget& window)
{
	window.draw(m_rangeCircle);
}


void PlayerPokemon::setPosition(sf::Vector2f position) {
	Pokemon::setPosition(position);
	m_rangeCircle.setPosition(position);
}


bool PlayerPokemon::contains(sf::Vector2f position) {
	if (m_sprite.getGlobalBounds().contains(position))
		return true;
	return false;
}



bool PlayerPokemon::damage(Pokemon* pokemon, AttackStats* attack, float pokemonAtkStat, int pokemonLevel)
{
	bool damaged = Pokemon::damage(pokemon, attack,pokemonAtkStat,pokemonLevel);
	if(m_ui)
		m_ui->update(stats);
	return damaged;
}

float PlayerPokemon::getHealth()
{
	return stats.health;
}




PokemonPlacement* PlayerPokemon::getPlacement() {
	return m_placeholder;
}


void PlayerPokemon::setPlacement(PokemonPlacement* pokemonplacement)
{
	m_placeholder = pokemonplacement;
	if (pokemonplacement) {
		pokemonplacement->setPokemon(this);
		setPosition(pokemonplacement->getPosition());
	}
}

void PlayerPokemon::setGameUI(PokemonPlaceUI& ui)
{
	ui.setPokemon(this);
	m_ui = &ui;
	m_ui->update(stats);
}

void PlayerPokemon::setActive(bool active)
{
	m_active = active;
	m_attackCooldown = m_currentAttack->attackCooldown;
	if (m_ui)
		m_ui->setActive(active);
	if (!active) {
		deleteAllAttacks();
	}
}

bool PlayerPokemon::isActive()
{
	return m_active;
}

bool PlayerPokemon::playerOwned()
{
	return true;
}

void PlayerPokemon::giveExp(float experience)
{
	Pokemon::giveExp(experience);
	if (m_ui) {
		m_ui->update(stats);
	}
}

bool PlayerPokemon::levelUp()
{
	bool outcome = Pokemon::levelUp();
	if (m_ui && outcome) {
		m_ui->update(stats);
	}
	return outcome;
}

int PlayerPokemon::getLevelUpCost()
{
	return stats.level * 5; //* stats.level;
}

void PlayerPokemon::evolve(PokemonStats* baseStats, sf::Texture* texture)
{
	Pokemon::evolve(baseStats, texture);
	if (m_ui) {
		m_ui->update(stats);
		m_ui->setPokemon(this); // update sprite with setpokemon
	}
}

