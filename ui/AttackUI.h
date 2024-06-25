#ifndef ATTACKUI_H
#define ATTACKUI_H

#include <SFML/Graphics.hpp>
#include "../attack/AttackStats.h"

class AttackUI
{
private:
	sf::RectangleShape m_box;
	sf::Text m_attackName;
	AttackStats* m_attack{};

public:
	AttackUI(const sf::Vector2f& position, const sf::Vector2f& size);
	AttackUI(const sf::Vector2f& position,const sf::Vector2f& size,AttackStats* attack);
	

	void Activate(bool activate);
	void draw(sf::RenderWindow& window);

	AttackStats* getAttack();
	void setAttackStats(AttackStats* attack);
	bool contains(sf::Vector2f position);
};

#endif