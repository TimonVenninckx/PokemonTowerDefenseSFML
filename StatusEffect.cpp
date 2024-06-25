#include "StatusEffect.h"
#include "Pokemon.h"
#include "ui/GameUI.h"

StatusEffect::StatusEffect(Pokemon& owner)
	: m_owner{ owner }
{
	m_sprite.setTexture(GameUI::statusTexture);
	m_spriteSize.x = GameUI::statusTexture.getSize().x / 3;
	m_spriteSize.y = GameUI::statusTexture.getSize().y / EffectType::Max_Effect;

	m_sprite.setTextureRect({ { 0,m_spriteSize.y * static_cast<int>(m_type) }, m_spriteSize });
	m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);
	
	
	
	m_movespeedMultiplier = 1.f;
	m_maxDuration = 8.f;
	m_duration = m_maxDuration;
	m_maxAnimationDuration = .3f;
	m_animationDuration = 0.f;
	m_animationIndex = 0;
}

StatusEffect::EffectType StatusEffect::getEffect()
{
	return m_type;
}

void StatusEffect::changeEffect(EffectType type)
{
	m_type = type;
	m_sprite.setTextureRect({0, 0, 0, 0});
	switch (type) {
	case EffectType::Paralysis:
	case EffectType::Freeze:
		m_movespeedMultiplier = .5f;
		break;
	default:
		m_movespeedMultiplier = 1.f;
		break;
	}
	m_duration = 1.f;
	//m_sprite.setTextureRect({ { 0,m_spriteSize.y * static_cast<int>(m_type) }, m_spriteSize });
}

void StatusEffect::update(float delta,sf::Vector2f position)
{
	m_duration -= delta;
	m_sprite.setPosition(position);

	if (m_duration > 0.f)
		return;
	m_animationDuration -= delta;
	if (m_animationDuration > 0.f)
		return;
	// otherwise update animation
	m_animationIndex++;

	switch (m_type) {
	case StatusEffect::Burn:
		if (m_animationIndex >= 3) {
			m_animationIndex = -1; // we will increment it to 0 again;
			m_duration = m_maxDuration;
			m_sprite.setTextureRect({ 0, 0, 0, 0 });
			m_owner.statusDamage(m_owner.getStats().maxHealth / 8.f);
			return;
		}
		else {
			m_sprite.setTextureRect({ { m_spriteSize.x * m_animationIndex,m_spriteSize.y * static_cast<int>(m_type)}, m_spriteSize });
			m_animationDuration = m_maxAnimationDuration;
		}
		break;
	case StatusEffect::Freeze:
	case StatusEffect::Paralysis:
		if (m_animationIndex >= 3) {
			m_movespeedMultiplier = .5f;
			m_animationIndex = -1; // we will increment it to 0 again;
			m_duration = m_maxDuration;
			m_sprite.setTextureRect({0, 0, 0, 0});
		}
		else {
			m_sprite.setTextureRect({ { m_spriteSize.x * m_animationIndex,m_spriteSize.y * static_cast<int>(m_type)}, m_spriteSize });
			m_animationDuration = m_maxAnimationDuration;
			m_movespeedMultiplier = 0.f;
		}
		break;
	case StatusEffect::Poison:
		if (m_animationIndex >= 3) {
			m_animationIndex = -1; // we will increment it to 0 again;
			m_duration = m_maxDuration;
			m_sprite.setTextureRect({ 0, 0, 0, 0 });
			m_owner.statusDamage(m_owner.getStats().maxHealth / 8.f);
			return;
		}
		else {
			m_sprite.setTextureRect({ { m_spriteSize.x * m_animationIndex,m_spriteSize.y * static_cast<int>(m_type)}, m_spriteSize });
			m_animationDuration = m_maxAnimationDuration;
		}
		break;
	default:
		break;
	}

}

void StatusEffect::draw(sf::RenderTarget& window)
{
	window.draw(m_sprite);
}

float StatusEffect::getMoveSpeedMultiplier()
{
	return m_movespeedMultiplier;
}
