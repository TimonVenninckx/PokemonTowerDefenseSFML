#include "AOEAttack.h"
#include "../Pokemon.h"
#include "../GameMath.h"
#include "../Level.h"
#include "../sqlite/Database.h"
#include <iostream>

void AOEAttack::startAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level)
{
	std::cout << "AOEATTACK.cpp Wrong start attack called!!!\n";
}

void AOEAttack::startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets)
{
	m_owner = owner;
	m_target = enemy;
	m_targetsHead = targets;
	m_levelRef = level;

	m_info = Database::getAttackInfo(m_stats->id);

	m_sprite.setTexture(m_info->texture);
	m_sprite.setPosition(owner->getPosition());
	m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);

	//m_sprite.setColor({ m_sprite.getColor().r,m_sprite.getColor().g,m_sprite.getColor().b,140 });

	//update(delta);
}



