#include "AttackPokemonSprite.h"
#include "../Pokemon.h"
#include "../GameMath.h"
#include "../Level.h"
#include <iostream>

AttackPokemonSprite::AttackPokemonSprite(AttackStats* attack, PokemonStats& pokemonstats)
	: Attack(attack, pokemonstats)
{
}

void AttackPokemonSprite::startAttack(Pokemon* owner, Pokemon* target, float delta, Level* level)
{
	m_owner = owner;
	m_target = target;
	m_levelRef = level;

	m_info = Database::getAttackInfo(m_stats->id);

	sf::Texture* texture = owner->getTexture();
	m_spriteSize = {static_cast<int>(texture->getSize().x) / 4, static_cast<int>(texture->getSize().y) / 4};
	m_sprite.setTexture(*texture);
	m_sprite.setTextureRect(sf::IntRect{ 0,0,m_spriteSize.x,m_spriteSize.y});
	m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
	m_sprite.setPosition(owner->getPosition());


	calculateDirection(target->getPosition() - m_sprite.getPosition());

	update(delta);
}

void AttackPokemonSprite::calculateDirection(sf::Vector2f direction)
{
	int lastDirection = m_direction;
	if (abs(direction.x) >= abs(direction.y)) {
		if (direction.x >= 0)
			m_direction = 2;
		else
			m_direction = 1;
	}
	else {
		if (direction.y >= 0)
			m_direction = 0;
		else
			m_direction = 3;
	}
	if(lastDirection != m_direction)
		m_sprite.setTextureRect(sf::IntRect{ 0, m_spriteSize.y * m_direction ,m_spriteSize.x,m_spriteSize.y });
}

bool AttackPokemonSprite::update(float delta)
{
	if (!m_owner) {
		std::cout << "PokemonAttack.cpp EROROR NO OWNING POKEMON IN  ATTACK\n";
		return false;
	}
	if (!m_target) {
		std::cout << "PokemonAttack.cpp  " << m_owner->getName() << " enemy not valid\n";
		return false;
	}

	GameMath::InterpolateResult result = GameMath::interpolate(m_sprite.getPosition(), m_target->getPosition(), m_info->attackSpeed * delta);

	//calculateDirection(result.position - m_sprite.getPosition());
	m_sprite.setPosition(result.position);

	if (result.leftOverStepSize > 0) {
		return kill(m_target);
	}
	return true;
}
