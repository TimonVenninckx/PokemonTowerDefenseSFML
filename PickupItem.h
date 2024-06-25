#ifndef PICKUPITEM_H
#define PICKUPITEM_H

#include <SFML/Graphics.hpp>
#include "LinkedList.h"

class Pokemon;

class PickupItem
{
	sf::Sprite m_sprite;
	sf::Vector2f m_positionOffset;
	Pokemon* m_owner{ nullptr };
	LinkedList<Pokemon>* m_pokemonHead{ nullptr };

public:
	PickupItem(sf::Texture* texture, sf::Vector2f position, LinkedList<Pokemon>* m_pokemonHead);
	void draw(sf::RenderWindow& window);
	void update();
	void setOwner(Pokemon* owner);
	void drop();
	bool hasOwner();
};




#endif
