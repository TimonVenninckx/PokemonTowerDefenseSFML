#include "AOEAttackStanding.h"
#include "../Pokemon.h"
#include "../GameMath.h"
#include "../Level.h"
#include "../sqlite/Database.h"
#include <iostream>


void AOEAttackStanding::startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets)
{
	AOEAttack::startAOEAttack(owner, enemy, delta, level, targets);

	m_range = owner->getRange();
	m_frameSize = sf::Vector2i(m_info->texture.getSize().x / m_info->frames, m_info->texture.getSize().y);

	m_lastFrame = m_info->frames - 1;
	m_maxFrameTime = m_info->frameTime;
	m_currentFrametime = m_maxFrameTime;

	m_sprite.setTexture(m_info->texture);
	m_sprite.setTextureRect({ 0,0,m_frameSize.x,m_frameSize.y });
	m_sprite.setPosition(owner->getPosition());
	m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);

}

void AOEAttackStanding::checkIfHit()
{
	std::cout << "are we trying to hit anything\n ";
	LinkedList<Pokemon>* it = m_targetsHead->next;
	LinkedList<Pokemon>* target;
	while (it) {
		target = it;
		it = it->next;
		if (std::find(m_hitEnemies.begin(), m_hitEnemies.end(), target->value) == m_hitEnemies.end()) {
			if (GameMath::distance(m_sprite.getPosition(),target->value->getPosition()) <= m_range) {
				kill(target->value);
				m_hitEnemies.push_back(target->value);
			}
		}
	}
}
