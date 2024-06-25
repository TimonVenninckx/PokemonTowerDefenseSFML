#ifndef ATTACKPOKEMONSPRITE_H
#define ATTACKPOKEMONSPRITE_H

#include "attack.h"


class AttackPokemonSprite : public Attack
{
	sf::Vector2i m_spriteSize{};
	int m_direction;

public:
	AttackPokemonSprite(AttackStats* attack, PokemonStats& pokemonstats);
	void startAttack(Pokemon* owner, Pokemon* target, float delta, Level* level)override;
	void calculateDirection(sf::Vector2f direction);
	bool update(float delta)override;
};



#endif
