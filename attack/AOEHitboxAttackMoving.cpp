#include "AOEHitboxAttackMoving.h"
#include "../GameMath.h"
#include "iostream"
#include "../Pokemon.h"

void AOEHitboxAttackMoving::startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets)
{
	AOEHitboxAttack::startAOEAttack(owner, enemy, delta, level, targets);
	float rotation =  -m_sprite.getRotation();
	m_direction = sf::Vector2f{ cos(rotation),sin(rotation)};

	m_maxDistance = 2000.f;

	float m_range = owner->getRange();

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
		m_direction = -GameMath::normalize(m_sprite.getPosition() - m_target->getPosition());
	}
}

bool AOEHitboxAttackMoving::update(float delta)
{
	sf::Vector2f newPosition = m_sprite.getPosition() + (m_direction * m_info->attackSpeed * delta);
	m_distanceTraveled += static_cast<float>(GameMath::distance(m_sprite.getPosition(), newPosition));
	//std::cout << "this is the distance traveled so far " << GameMath::distance(m_sprite.getPosition(), newPosition) << '\n';
	m_sprite.setPosition(newPosition);
	checkIfHit();
	if (m_distanceTraveled > m_maxDistance)
		return false;
	// doesn't get executed
	m_currentFrametime -= delta;
	if (m_currentFrametime < 0.f) {
		if (m_currentFrame == m_lastFrame)
			m_currentFrame = 0;
		else
			m_currentFrame++;
		m_currentFrametime = m_maxFrameTime;
		m_sprite.setTextureRect({ m_frameSize.x * m_currentFrame,0,m_frameSize.x,m_frameSize.y });
		//std::cout << "currenframe: " << m_currentFrame << " maxframe:  " << m_lastFrame << '\n';
	}
	return true;
}
