#ifndef AOEATTACKSTANDING_H
#define AOEATTACKSTANDING_H
#include "AOEHitboxAttack.h"

class AOEAttackStanding : public AOEHitboxAttack
{
protected:
	double m_range{};
public:

	AOEAttackStanding(AttackStats* attack, PokemonStats& pokemonstats)
		: AOEHitboxAttack{ attack,pokemonstats }
	{}
	void startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets) override;
	void checkIfHit()override;
};


#endif