#ifndef ATTACKINFO_H
#define ATTACKINFO_H
#include "AttackStats.h"
#include <SFML/Graphics.hpp>

struct AttackInfo
{
	int id{};
	AttackStats::AttackClass attackClass{ AttackStats::AttackClass::NONE_CLASS };
	sf::Texture texture;
	int frames{1};
	float frameTime{};
	float attackSpeed{};
};

#endif