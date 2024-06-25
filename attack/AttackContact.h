#ifndef PROJECTILEATTACK_H
#define PROJECTILEATTACK_H
#include "ProjectileAttack.h"
class AttackContact :    public ProjectileAttack
{
protected:

	bool onTarget{ false };

public:
	AttackContact(AttackStats* attack, PokemonStats& pokemonstats)
		: ProjectileAttack{ attack,pokemonstats }
	{}
	bool update(float delta)override;
};



#endif
