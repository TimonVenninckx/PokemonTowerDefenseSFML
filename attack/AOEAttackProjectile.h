#pragma once
#include "AOEAttack.h"


class AOEAttackProjectile :
    public AOEAttack
{
	sf::Vector2i m_frameSize{};
	float m_maxFrameTime{};
	float m_currentFrametime{};

	int   m_currentFrame{};
	int   m_lastFrame{};
	bool m_attacked{ false };
	std::map<Pokemon*, sf::Sprite> m_sprites;
public:

	AOEAttackProjectile(AttackStats* attack, PokemonStats& pokemonstats)
		: AOEAttack{ attack,pokemonstats }
	{}
	void startAOEAttack(Pokemon* owner, Pokemon* enemy, float delta, Level* level, LinkedList<Pokemon>* targets) override;
	bool update(float delta) override;
	bool removeTarget(Pokemon* pokemon)override;
	void draw(sf::RenderTarget& window)override;

};

