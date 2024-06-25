#ifndef HEALTHBAR_H
#define HEALTHBAR_H
#include <SFML/Graphics.hpp>
#include "../StatusEffect.h"

class HealthBar
{
private:
	sf::Vector2f m_size;
	sf::RectangleShape m_shape{};
	sf::Text m_healthText;
	sf::Text m_lvlText;
	sf::Sprite m_statusSprite;

	bool m_canCatch{true};
public:
	HealthBar();

	void canCatch(bool cancatch);
	void update(float health, float  maxhealth);
	void updateLevel(int level);
	void draw(sf::RenderTarget& window);

	void setPosition(sf::Vector2f position);

	void changeStatusEffect(StatusEffect::EffectType effect);
};





#endif

