#ifndef AOEATTACK_H
#define AOEATTACK_H
#include "Attack.h"


class AOEAttack : public Attack
{

protected:
	LinkedList<Pokemon>* m_targetsHead{};
public:
	AOEAttack(AttackStats* attack, PokemonStats& pokemonstats)
		: Attack{ attack,pokemonstats }
	{}

	virtual void startAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level) override;
	virtual void startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets) override;
};

#endif