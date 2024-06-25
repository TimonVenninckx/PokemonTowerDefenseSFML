#ifndef ATTACKSTANDING_H
#define ATTACKSTANDING_H
#include "Attack.h"

class AttackStanding : public Attack
{
protected:
	float m_animationTime{};
	bool  m_attacked{ false };
public:
	AttackStanding(AttackStats* attack, PokemonStats& pokemonstats)
		: Attack{ attack,pokemonstats }
	{}

	void startAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level) override;
	bool update(float delta) override;
	bool removeTarget(Pokemon* pokemon)override;
};

#endif