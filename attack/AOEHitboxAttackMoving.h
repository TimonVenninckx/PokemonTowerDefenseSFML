#pragma once
#include "AOEHitboxAttack.h"
class AOEHitboxAttackMoving : public AOEHitboxAttack
{

	sf::Vector2f m_direction{};
	float m_distanceTraveled{};
	float m_maxDistance{};
public:

	AOEHitboxAttackMoving(AttackStats* attack, PokemonStats& pokemonstats)
		: AOEHitboxAttack{ attack,pokemonstats }
	{}
	void startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets) override;
	bool update(float delta) override;
};

