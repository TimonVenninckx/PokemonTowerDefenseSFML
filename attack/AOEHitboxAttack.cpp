#include "AOEHitboxAttack.h"
#include "../Pokemon.h"
#include "../GameMath.h"
#include "../Level.h"
#include "../sqlite/Database.h"
#include <iostream>

void AOEHitboxAttack::startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets)
{
	AOEAttack::startAOEAttack(owner, enemy, delta, level, targets);

	float m_range = owner->getRange();
	m_frameSize = sf::Vector2i(m_info->texture.getSize().x / m_info->frames, m_info->texture.getSize().y);

	m_lastFrame = m_info->frames - 1;
	m_maxFrameTime = m_info->frameTime;
	m_currentFrametime = m_maxFrameTime;


	m_sprite.setTexture(m_info->texture);
	m_sprite.setTextureRect({ 0,0,m_frameSize.x,m_frameSize.y });
	m_sprite.setPosition(owner->getPosition());
	m_sprite.setOrigin(0.f,m_sprite.getGlobalBounds().getSize().y / 2.f);

	LinkedList<Pokemon>* target = m_targetsHead->next;
	while (target) {
		if (GameMath::distance(owner->getPosition(), target->value->getPosition()) < m_range) {
			if (m_target == nullptr) {
				m_target = target->value;
			}
			else if (GameMath::distance(owner->getPosition(), target->value->getPosition()) < GameMath::distance(owner->getPosition(), m_target->getPosition()))
				m_target = target->value;
		}
		target = target->next;
	}
	if (m_target) {
		double angle = atan2(m_sprite.getPosition().x - m_target->getPosition().x, m_sprite.getPosition().y - m_target->getPosition().y);
		angle = angle * 180 / 3.14159265358979323846 + 90; // + 90 because sprite is drawn to the right
		m_sprite.setRotation(static_cast<float>(-angle));
		checkIfHit();
	}
}

bool AOEHitboxAttack::update(float delta)
{
	m_currentFrametime -= delta;
	if (m_currentFrametime < 0.f) {
		if (m_currentFrame != m_lastFrame) {

			m_currentFrame++;
			m_currentFrametime = m_maxFrameTime;

			m_sprite.setTextureRect({ m_frameSize.x * m_currentFrame,0,m_frameSize.x,m_frameSize.y });
			checkIfHit();
			return true;
		}
		return false;
	}
	return true;
}

bool AOEHitboxAttack::removeTarget(Pokemon* pokemon)
{
	// we could add them to the already hit list but that shouldn't be necessary
	// because we have he will only be on the active enemies linkedlist if we are the one attacking.
	// but we will just skip him anyways by using an iterator and a target
    return false;
}

void AOEHitboxAttack::checkIfHit()
{
	LinkedList<Pokemon>* it = m_targetsHead->next;
	LinkedList<Pokemon>* target;
	while (it) {
		target = it;
		it = it->next;
		if (std::find(m_hitEnemies.begin(), m_hitEnemies.end(), target->value) == m_hitEnemies.end()) {
			if (m_sprite.getGlobalBounds().contains(target->value->getPosition())) {
				kill(target->value);
				m_hitEnemies.push_back(target->value);
			}
		}
	}

}
