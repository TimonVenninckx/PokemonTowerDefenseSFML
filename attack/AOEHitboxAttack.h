#ifndef AOEHITBOXATTACK_H
#define AOEHITBOXATTACK_H

#include "AOEAttack.h"
#include <vector>


class AOEHitboxAttack : public AOEAttack
{
protected:
	sf::Vector2i m_frameSize{};
	float m_maxFrameTime{};
	float m_currentFrametime{};

	int   m_currentFrame{};
	int   m_lastFrame{};

	std::vector<Pokemon*> m_hitEnemies;

public:

	AOEHitboxAttack(AttackStats* attack, PokemonStats& pokemonstats)
		: AOEAttack{ attack,pokemonstats }
	{}
	void startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets) override;
	bool update(float delta) override;
	bool removeTarget(Pokemon* pokemon)override;
	virtual void checkIfHit();
};


#endif
