#ifndef STATUSEFFECT_H
#define STATUSEFFECT_H

#include <SFML/Graphics.hpp>

class Pokemon;

class StatusEffect
{
public:
	enum EffectType {
		None,
		Burn,
		Freeze,
		Paralysis,
		Poison,
		Sleep,
		Max_Effect
	};


	StatusEffect(Pokemon& owner);
    EffectType getEffect();
	void changeEffect(EffectType type);
	void update(float delta, sf::Vector2f position);
	void draw(sf::RenderTarget& window);
	float getMoveSpeedMultiplier();
private:

	Pokemon& m_owner;
	sf::Vector2i m_spriteSize;
	sf::Sprite m_sprite;
	float m_movespeedMultiplier;
	EffectType m_type{ Max_Effect };
	int		   m_animationIndex;
	float	   m_duration{};
	float	   m_maxDuration{};
	float	   m_maxAnimationDuration{};
	float	   m_animationDuration{};
};


#endif
