#ifndef POKEMON_ATTACK_H
#define POKEMON_ATTACK_H
#include <SFML/Graphics.hpp>
#include "AttackStats.h"
#include "AttackInfo.h"
#include "../ui/GameUI.h"
#include "../LinkedList.h"
class Pokemon;
class Level;


class Attack
{
protected:
	sf::Music m_soundEffect;

	AttackInfo* m_info{};
	AttackStats* m_stats{};
	float m_pokemonAtkStat{};
	int m_pokemonLevel{};

	sf::Sprite m_sprite{};
	Pokemon* m_owner{};
	Pokemon* m_target{};

	// for calling kill enemy function
	Level* m_levelRef{};
	virtual bool kill(Pokemon* target);
public:
	Attack(AttackStats* attack, PokemonStats& pokemonstats);
	virtual bool removeTarget(Pokemon* pokemon);
	virtual void startAttack(Pokemon* owner, Pokemon* target, float delta, Level* level);
	virtual void startAOEAttack(Pokemon* owner, Pokemon* target, float delta, Level* level, LinkedList<Pokemon>* targets);
	virtual bool update(float delta);
	virtual void draw(sf::RenderTarget& window);
};



#endif