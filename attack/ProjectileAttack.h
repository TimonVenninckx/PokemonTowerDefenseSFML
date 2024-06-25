#ifndef PROJECTILEPOKEMONATTACK_H
#define PROJECTILEPOKEMONATTACK_H

#include <SFML/Graphics.hpp>
#include "Attack.h"


class ProjectileAttack : public Attack
{
protected:
	sf::Vector2i m_frameSize{};
	float m_maxFrameTime{};
	float m_currentFrametime{};

	int   m_currentFrame{};
	int   m_maxFrames{}; // amount of frames
public:
	ProjectileAttack(AttackStats* attack, PokemonStats& pokemonstats)
		: Attack{ attack,pokemonstats }
	{}
	void startAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level) override;
	bool update(float delta) override;

};

#endif