#include "AttackStats.h"


bool operator== (const AttackStats& a1, const AttackStats& a2)
{
	if( a1.name != a2.name ||
		a1.type != a2.type ||
		a1.power != a2.power ||
		a1.accuracy != a2.accuracy ||
		a1.category != a2.category ||
		a1.attackClass != a2.attackClass ||
		a1.contact != a2.contact)
		return false;
	
	return true;
}

std::string AttackStats::getCategoryString(AttackCategory attack)
{
	switch (attack) {
	case AttackCategory::PHYSICAL:	return "Physical";
	case AttackCategory::SPECIAL:	return "Special";
	case AttackCategory::STATUS:	return "Status";
	default:						return "none";
	}
}
