#ifndef ATTACKSTATS_H
#define ATTACKSTATS_H

#include <string>
#include "../PokemonStats.h"
#include "../StatusEffect.h"

struct AttackStats
{
	// Physical Special or status
	enum class AttackCategory
	{
		PHYSICAL,
		SPECIAL,
		STATUS,
		MAX_ATTACKTYPE
	};
	enum class AttackClass {
		TACKLE_CLASS,
		PROJECTILE_CLASS,
		STANDING_CLASS,
		AOESTANDING_CLASS,
		AOEPROJECTILE_CLASS,
		CONTACT_CLASS,
		AOEHITBOX_CLASS,
		AOEHITBOXMOVING_CLASS,
		NONE_CLASS,
	};
	enum class AdditionalEffects {
		NONE,
		WHIRLWIND,
		FLINCH,
		MAX_EFFECTS
	};
	friend bool operator== (const AttackStats& a1, const AttackStats& a2);
	static std::string getCategoryString(AttackCategory attack);

	int id{};
	std::string name{};
	std::string info{};
	PokemonStats::Types type{ PokemonStats::NORMAL };
	float power{ 40.f };
	float accuracy{ 100.f };
	float attackCooldown{ 1.f };
	// Physical Special or status
	AttackCategory category{ AttackCategory::PHYSICAL };
	AttackClass attackClass{ AttackClass::NONE_CLASS };
	bool contact{ false };
	StatusEffect::EffectType statusEffect{ StatusEffect::None };
	int statusEffectAccuracy{ 100 };
	AdditionalEffects  additionalEffect{ AdditionalEffects::NONE };

};


/*
inline AttackStats flameThrowerTestAttack{ 
	1,
	"FlameThrower",
	"The foe is scorched with an intense blast of fire.\nThe target may also be left with a burn.",
	PokemonStats::FIRE,
	60.f, 
	100.f,
	.6f,
	AttackStats::AttackCategory::SPECIAL,
	AttackStats::AttackClass::PROJECTILE_CLASS,
	false
};

inline AttackStats thunderBoltTestAttack{
	2,
	"Thunderbolt",
	"A strong electric blast is loosed at the foe.\nIt may also leave the foe paralyzed.",
	PokemonStats::ELECTRIC,
	95.f,
	100.f,
	.4f,
	AttackStats::AttackCategory::SPECIAL,
	AttackStats::AttackClass::PROJECTILE_CLASS,
	false
};

inline AttackStats TackleTestAttack{
	3,
	"Tackle",
	"A full-body charge attack.",
	PokemonStats::NORMAL,
	40.f,
	100.f,
	.5f,
	AttackStats::AttackCategory::PHYSICAL,
	AttackStats::AttackClass::TACKLE_CLASS,
	true
};
*/



#endif