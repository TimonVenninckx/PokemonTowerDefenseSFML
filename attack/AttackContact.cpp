#include "AttackContact.h"
#include "../Pokemon.h"
#include "../GameMath.h"
#include "../Level.h"
#include "../sqlite/Database.h"



bool AttackContact::update(float delta)
{
	GameMath::InterpolateResult result = GameMath::interpolate(m_sprite.getPosition(), m_target->getPosition(), m_info->attackSpeed * delta);
	m_sprite.setPosition(result.position);


	if (onTarget) {
		m_currentFrametime -= delta;
		if (m_currentFrametime < 0.f) {
			m_currentFrame++;
			if (m_currentFrame == m_maxFrames) {
				return kill(m_target); // return true if kills because attack will be deleted by the level;
			}
			m_currentFrametime = m_maxFrameTime;
			m_sprite.setTextureRect(sf::IntRect{ m_frameSize.x * m_currentFrame,0,m_frameSize.x, m_frameSize.y });
			return true;
		}
	}
	else if (result.leftOverStepSize > 0) {
		onTarget = true;
		m_currentFrametime = 0.f;
		return true;
	}
	return true;
}
