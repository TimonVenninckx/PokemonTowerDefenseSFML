#include "AttackUI.h"
#include "GameUI.h"


AttackUI::AttackUI(const sf::Vector2f& position, const sf::Vector2f& size)
{
	m_box.setPosition(position);
	m_box.setSize(size);
	m_box.setFillColor({ 20,20,20,255 });
	m_box.setOutlineThickness(-2);
	m_box.setOutlineColor({0, 0, 0, 255});

	m_attackName.setPosition(position + sf::Vector2f{ size.x / 20.f, size.y / 4.f });
	m_attackName.setFont(GameUI::font);
	m_attackName.setCharacterSize(static_cast<unsigned int>(size.y / 2.5f));
}


AttackUI::AttackUI(const sf::Vector2f& position,const sf::Vector2f& size,AttackStats* attack)
	: AttackUI{ position,size }
{
	setAttackStats(attack);
}




void AttackUI::Activate(bool activate)
{
	if (activate)
		m_box.setFillColor({ 50,50,50,255 });
	else
		m_box.setFillColor({ 20,20,20,255 });
}

void AttackUI::draw(sf::RenderWindow& window)
{
	window.draw(m_box);
	window.draw(m_attackName);
}

AttackStats* AttackUI::getAttack()
{
	return m_attack;
}

void AttackUI::setAttackStats(AttackStats* attack)
{
	m_attack = attack;
	if (attack) {
		m_attackName.setString(attack->name);
		m_attackName.setFillColor(PokemonStats::getTypeColor(attack->type));
	}
	else {
		m_attackName.setString("");
	}
}

bool AttackUI::contains(sf::Vector2f position)
{
	if (!m_attack)
		return false;
	if (m_attack->attackClass == AttackStats::AttackClass::NONE_CLASS)
		return false;

	return m_box.getGlobalBounds().contains(position);
}
