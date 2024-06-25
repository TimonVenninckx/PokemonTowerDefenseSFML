#include "PickupItem.h"
#include "Pokemon.h"
#include <iostream>
#include "GameMath.h"


PickupItem::PickupItem(sf::Texture* texture, sf::Vector2f position, LinkedList<Pokemon>* pokemonHead)
{
	m_pokemonHead = pokemonHead;
	m_sprite.setTexture(*texture);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);
	m_sprite.setPosition(position);
}

void PickupItem::draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void PickupItem::update() 
{
	if (m_owner) {
		m_sprite.setPosition(m_owner->getPosition());
	}
	else {
		for (LinkedList<Pokemon>* i{ m_pokemonHead->next }; i; i = i->next) {
			if (GameMath::distance(m_sprite.getPosition(),i->value->getPosition()) < 40.f) {
				//std::cout << "try pickup item\n";
				if (i->value->canPickupItem()) {
					setOwner(i->value);
					return;
				}
			}
		}
	}
}


void PickupItem::setOwner(Pokemon* owner)
{
	if (m_owner) {
		std::cout << "already has owner";
		return;
	}
	m_owner = owner;
	m_owner->setPickupItem(this);
	m_positionOffset.y = m_owner->getTexture()->getSize().y / 4.f / 4.f;  // by 4 because we have four animations in a texture
	std::cout << "item picked up offset = " << m_positionOffset.y << '\n';
	m_sprite.setPosition(m_owner->getPosition() + m_positionOffset);
}

void PickupItem::drop()
{
	//if (m_owner)
		//m_sprite.setPosition(m_sprite.getPosition() - m_positionOffset);
	m_owner = nullptr;
}

bool PickupItem::hasOwner()
{
	return m_owner != nullptr;
} 
