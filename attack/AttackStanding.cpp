#include "AttackStanding.h"
#include "../Pokemon.h"
#include "../GameMath.h"
#include "../Level.h"
#include "../sqlite/Database.h"
#include <iostream>

void AttackStanding::startAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level)
{
	m_owner = owner;
	m_target = enemy;
	m_levelRef = level;
	AttackInfo* info = Database::getAttackInfo(m_stats->id);

	m_sprite.setTexture(info->texture);
	m_sprite.setPosition(owner->getPosition());
	m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);

	//m_sprite.setColor({ m_sprite.getColor().r,m_sprite.getColor().g,m_sprite.getColor().b,140 });

	m_animationTime = .1f;
	update(delta);
}

bool AttackStanding::update(float delta)
{

	m_animationTime -= delta;

	if (m_animationTime <= 0.f) {
		if (!m_attacked) {
			m_attacked = true;
			m_animationTime = 0.4f;
			kill(m_target);
		}
		else 
			return false;
	}
	return true;
}

bool AttackStanding::removeTarget(Pokemon* pokemon)
{
	if (m_target == pokemon)
		if (!m_attacked)
			return true; // cancel if pokemon and if we haven't attacked yet
	return false;
}