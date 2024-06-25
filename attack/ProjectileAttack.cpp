#include "ProjectileAttack.h"
#include "../Pokemon.h"
#include "../GameMath.h"
#include "../Level.h"
#include "../sqlite/Database.h"
#include <iostream>



void ProjectileAttack::startAttack(Pokemon* owner, Pokemon* target, float delta, Level* level)
{
	m_owner = owner;
	m_target = target;
	m_levelRef = level;

	m_info = Database::getAttackInfo(m_stats->id);

	m_maxFrameTime = m_info->frameTime;
	m_frameSize = { static_cast<int>(m_info->texture.getSize().x) / m_info->frames,static_cast<int>(m_info->texture.getSize().y) };
	m_maxFrames = m_info->frames;

	m_sprite.setTexture(m_info->texture);
	m_sprite.setTextureRect(sf::IntRect{ 0,0,m_frameSize.x, m_frameSize.y });
	m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);
	m_sprite.setPosition(owner->getPosition());

	update(delta);
}

bool ProjectileAttack::update(float delta)
{
	GameMath::InterpolateResult result = GameMath::interpolate(m_sprite.getPosition(), m_target->getPosition(), m_info->attackSpeed * delta);
	m_sprite.setPosition(result.position);

	if (result.leftOverStepSize > 0.f)
		return kill(m_target);

	m_currentFrametime -= delta;
	if (m_currentFrametime < 0.f) {
		m_currentFrame++;
		if (m_currentFrame == m_maxFrames)
			m_currentFrame = 0;

		m_currentFrametime = m_maxFrameTime;
		m_sprite.setTextureRect(sf::IntRect{ m_frameSize.x * m_currentFrame,0,m_frameSize.x, m_frameSize.y });
		return true;
	}
	return true;
}


