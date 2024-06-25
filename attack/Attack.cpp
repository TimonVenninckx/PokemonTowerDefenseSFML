#include "Attack.h"
#include "../Pokemon.h"
#include "../GameMath.h"
#include "../Level.h"
#include <iostream>
#include "../Random.h"
#include "../SoundEffectManager.h"


bool Attack::kill(Pokemon* target)
{
	if (m_stats->accuracy < Random::get(1, 100)){
		m_levelRef->spawnDamageText(target->getPosition(), { 0.f,1.f }, "miss");
		return false;
	}
	SoundEffectManager::add(new sf::Sound);
	std::cout << "attack is current: " << m_stats->name << '\n';
	target->damage(m_owner, m_stats, m_pokemonAtkStat, m_pokemonLevel);


	if (target->getHealth() <= 0.f) {
		if (!m_levelRef) {
			std::cout << "Attackstanding.cpp Level ref not valid\n";
		}
		if (target->playerOwned())
			m_levelRef->playerPokemonDied(target);
		else
			m_levelRef->enemyPokemonDied(target);
		return true;
	}
	return false;
}

Attack::Attack(AttackStats* attack, PokemonStats& pokemonstats)
{
	m_stats = attack;
	m_pokemonLevel = pokemonstats.level;

	if (attack->category == AttackStats::AttackCategory::PHYSICAL) {
		m_pokemonAtkStat = pokemonstats.attack;
	}
	else if (attack->category == AttackStats::AttackCategory::SPECIAL) {
		m_pokemonAtkStat = pokemonstats.spAttack;
	}
}

bool Attack::removeTarget(Pokemon* pokemon)
{
	if (m_target == pokemon)
		return true;
	return false;
}

void Attack::startAttack(Pokemon* owner, Pokemon* target, float delta, Level* level)
{
	// not valid attack it is the base attack
}

void Attack::startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets)
{
	std::cout << "Attack.h Start AOE Attack not defined " << m_stats->name << '\n';
}

bool Attack::update(float delta)
{
	std::cout << "Not valid attack update func\n";
	return true;
}

void Attack::draw(sf::RenderTarget& window)
{
	window.draw(m_sprite);
}
