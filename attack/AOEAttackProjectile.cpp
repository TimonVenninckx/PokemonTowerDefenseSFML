#include "AOEAttackProjectile.h"
#include "../Pokemon.h"
#include "../GameMath.h"
#include "../Level.h"
#include "../sqlite/Database.h"
#include <iostream>

void AOEAttackProjectile::startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets)
{
	AOEAttack::startAOEAttack(owner, enemy, delta, level, targets);

	float m_range = owner->getRange();
	m_frameSize = sf::Vector2i(m_info->texture.getSize().x / m_info->frames, m_info->texture.getSize().y);

	m_lastFrame = m_info->frames - 1;
	m_maxFrameTime = m_info->frameTime;
	m_currentFrametime = m_maxFrameTime;


	sf::Sprite sprite;
	sprite.setTexture(m_info->texture);
	sprite.setTextureRect({ 0,0,m_frameSize.x,m_frameSize.y });
	sprite.setOrigin(0.f, m_frameSize.y / 2.f);
	sprite.setPosition(owner->getPosition());
	


	LinkedList<Pokemon>* target = m_targetsHead->next;
	while (target) {
		if (GameMath::distance(owner->getPosition(), target->value->getPosition()) < m_range) {

			// scaling slowly up
			float scale{ static_cast<float>(GameMath::distance(target->value->getPosition(),sprite.getPosition()) / sprite.getLocalBounds().getSize().x) };

			sprite.setScale(scale, scale);

			double angle = atan2(sprite.getPosition().x - target->value->getPosition().x, sprite.getPosition().y - target->value->getPosition().y);
			angle = angle * 180 / 3.14159265358979323846 + 90; // + 90 because sprite is drawn to the right
			// invert angle because of rotational system
			// sfml uses y+ on down;
			//https://cplusplus.com/forum/general/113085/
			sprite.setRotation(static_cast<float>(-angle));

			m_sprites.emplace(target->value, sprite);
		}

		target = target->next;
	}
}

bool AOEAttackProjectile::update(float delta)
{
	m_currentFrametime -= delta;
	if (m_currentFrametime < 0.f) {
		if (m_currentFrame != m_lastFrame) {

			m_currentFrame++;
			m_currentFrametime = m_maxFrameTime;


			std::map<Pokemon*, sf::Sprite>::iterator it;
			for (it = m_sprites.begin(); it != m_sprites.end(); ++it) {
				it->second.setTextureRect({ m_frameSize.x * m_currentFrame,0,m_frameSize.x,m_frameSize.y });
			}
			return true;
		}

		std::map<Pokemon*, sf::Sprite>::iterator it;
		for (it = m_sprites.begin(); it != m_sprites.end();) {

			std::map<Pokemon*, sf::Sprite>::iterator old = it;
			++it;
			kill(old->first);
		}
		return false;
	}
	return true;
}

bool AOEAttackProjectile::removeTarget(Pokemon* pokemon)
{

	if (m_sprites.count(pokemon)) {
		m_sprites.erase(m_sprites.find(pokemon));
	}
	return false;
}

void AOEAttackProjectile::draw(sf::RenderTarget& window)
{
	std::map<Pokemon*, sf::Sprite>::iterator it;
	for (it = m_sprites.begin(); it != m_sprites.end(); ++it) {
		window.draw(it->second);
	}
}
