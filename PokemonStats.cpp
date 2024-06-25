#include "PokemonStats.h"


sf::Color PokemonStats::getTypeColor(Types type)
{
	switch (type) {
	case BUG:			return sf::Color(166, 185, 26 , 255);
	case DRAGON:		return sf::Color(111, 53 , 252, 255);
	case ELECTRIC:		return sf::Color(247, 208, 44 , 255);
	case FAIRY:			return sf::Color(214, 133, 173, 255);
	case DARK:			return sf::Color(112, 87 , 70 , 255);
	case FIGHTING:		return sf::Color(194, 46 , 40 , 255);
	case FIRE:			return sf::Color(238, 129, 48 , 255);
	case FLYING:		return sf::Color(169, 143, 243, 255);
	case GHOST:			return sf::Color(115, 87 , 151, 255);
	case GRASS:			return sf::Color(122, 199, 76 , 255);
	case GROUND:		return sf::Color(226, 191, 101, 255);
	case ICE:			return sf::Color(150, 217, 214, 255);
	case NORMAL:		return sf::Color(255, 255, 255, 255); //sf::Color(168, 167, 122, 255);
	case POISON:		return sf::Color(163, 62 , 161, 255);
	case PSYCHIC:		return sf::Color(249, 85 , 135, 255);
	case ROCK:			return sf::Color(182, 161, 54 , 255);
	case STEEL:			return sf::Color(183, 183, 206, 255);
	case WATER:			return sf::Color(99 , 144, 240, 255);
	default:			return sf::Color::White;
	}
}

std::string PokemonStats::getTypeString(Types type) {
	switch (type) {
	case BUG:			return std::string{ "Bug" };
	case DRAGON:		return std::string{ "Dragon" };
	case ELECTRIC:		return std::string{ "Electric" };
	case FAIRY:			return std::string{ "Fairy" };
	case DARK:			return std::string{ "Dark" };
	case FIGHTING:		return std::string{ "Fighting" };
	case FIRE:			return std::string{ "Fire" };
	case FLYING:		return std::string{ "Flying" };
	case GHOST:			return std::string{ "Ghost" };
	case GRASS:			return std::string{ "Grass" };
	case GROUND:		return std::string{ "Ground" };
	case ICE:			return std::string{ "Ice" };
	case NORMAL:		return std::string{ "Normal" };
	case POISON:		return std::string{ "Poison" };
	case PSYCHIC:		return std::string{ "Psychic" };
	case ROCK:			return std::string{ "Rock" };
	case STEEL:			return std::string{ "Steel" };
	case WATER:			return std::string{ "Water" };
	default:			return std::string{ "" };
	}
}




float PokemonStats::effectiveness(Types attacking, Types defending)
{
	switch (attacking) {
	case BUG:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return 2.f;
		case FIGHTING:		return .5f;
		case FIRE:			return .5f;
		case FLYING:		return .5f;
		case GHOST:			return .5f;
		case GRASS:			return 2.f;
		case GROUND:		return 1.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return .5f;
		case PSYCHIC:		return 2.f;
		case ROCK:			return 1.f;
		case STEEL:			return .5f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case DRAGON:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 2.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 0.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return 1.f;
		case FLYING:		return 1.f;
		case GHOST:			return 1.f;
		case GRASS:			return 1.f;
		case GROUND:		return 1.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return 1.f;
		case STEEL:			return .5f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case ELECTRIC:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return .5f;
		case ELECTRIC:		return .5f;
		case FAIRY:			return 1.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return 1.f;
		case FLYING:		return 2.f;
		case GHOST:			return 1.f;
		case GRASS:			return .5f;
		case GROUND:		return 0.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return 1.f;
		case STEEL:			return 1.f;
		case WATER:			return 2.f;
		default:			return 1.f;
		}
	case FAIRY:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 2.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return 2.f;
		case FIGHTING:		return 2.f;
		case FIRE:			return .5f;
		case FLYING:		return 1.f;
		case GHOST:			return 1.f;
		case GRASS:			return 1.f;
		case GROUND:		return 1.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return .5f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return 1.f;
		case STEEL:			return .5f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case DARK:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return .5f;
		case DARK:			return .5f;
		case FIGHTING:		return .5f;
		case FIRE:			return 1.f;
		case FLYING:		return 1.f;
		case GHOST:			return 2.f;
		case GRASS:			return 1.f;
		case GROUND:		return 1.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 2.f;
		case ROCK:			return 1.f;
		case STEEL:			return 1.f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case FIGHTING:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return .5f;
		case DARK:			return 2.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return 1.f;
		case FLYING:		return .5f;
		case GHOST:			return 0.f;
		case GRASS:			return 1.f;
		case GROUND:		return 1.f;
		case ICE:			return 2.f;
		case NORMAL:		return 2.f;
		case POISON:		return .5f;
		case PSYCHIC:		return .5f;
		case ROCK:			return 2.f;
		case STEEL:			return 1.f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case FIRE:
		switch (defending) {
		case BUG:			return 2.f;
		case DRAGON:		return .5f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return .5f;
		case FLYING:		return 1.f;
		case GHOST:			return 1.f;
		case GRASS:			return 2.f;
		case GROUND:		return 1.f;
		case ICE:			return 2.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return .5f;
		case STEEL:			return 2.f;
		case WATER:			return .5f;
		default:			return 1.f;
		}
	case FLYING:
		switch (defending) {
		case BUG:			return 2.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return .5f;
		case FAIRY:			return 1.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 2.f;
		case FIRE:			return 1.f;
		case FLYING:		return 1.f;
		case GHOST:			return 1.f;
		case GRASS:			return 2.f;
		case GROUND:		return 1.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return .5f;
		case STEEL:			return .5f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case GHOST:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return .5f;
		case FIGHTING:		return 1.f;
		case FIRE:			return 1.f;
		case FLYING:		return 1.f;
		case GHOST:			return 2.f;
		case GRASS:			return 1.f;
		case GROUND:		return 1.f;
		case ICE:			return 1.f;
		case NORMAL:		return 0.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 2.f;
		case ROCK:			return 1.f;
		case STEEL:			return 1.f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case GRASS:
		switch (defending) {
		case BUG:			return .5f;
		case DRAGON:		return .5f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return .5f;
		case FLYING:		return .5f;
		case GHOST:			return 1.f;
		case GRASS:			return .5f;
		case GROUND:		return 2.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return .5f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return 2.f;
		case STEEL:			return .5f;
		case WATER:			return 2.f;
		default:			return 1.f;
		}
	case GROUND:
		switch (defending) {
		case BUG:			return .5f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return 2.f;
		case FAIRY:			return 1.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return 2.f;
		case FLYING:		return 0.f;
		case GHOST:			return 1.f;
		case GRASS:			return 1.f;
		case GROUND:		return 1.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return 2.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return 2.f;
		case STEEL:			return 2.f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case ICE:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 2.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return .5f;
		case FLYING:		return 2.f;
		case GHOST:			return 1.f;
		case GRASS:			return 2.f;
		case GROUND:		return 2.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return 1.f;
		case STEEL:			return .5f;
		case WATER:			return .5f;
		default:			return 1.f;
		}
	case NORMAL: // HERE WE ARE CURRENTLY
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return 1.f;
		case FLYING:		return 1.f;
		case GHOST:			return 0.f;
		case GRASS:			return 1.f;
		case GROUND:		return 1.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return .5f;
		case STEEL:			return .5f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case POISON:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 2.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return 1.f;
		case FLYING:		return 1.f;
		case GHOST:			return .5f;
		case GRASS:			return 2.f;
		case GROUND:		return .5f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return .5f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return .5f;
		case STEEL:			return 1.f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case PSYCHIC:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return 0.f;
		case FIGHTING:		return 2.f;
		case FIRE:			return 1.f;
		case FLYING:		return 1.f;
		case GHOST:			return 1.f;
		case GRASS:			return 1.f;
		case GROUND:		return 1.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return 2.f;
		case PSYCHIC:		return .5f;
		case ROCK:			return 1.f;
		case STEEL:			return .5f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case ROCK:
		switch (defending) {
		case BUG:			return 2.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return 1.f;
		case FIGHTING:		return .5f;
		case FIRE:			return 2.f;
		case FLYING:		return 2.f;
		case GHOST:			return 1.f;
		case GRASS:			return 1.f;
		case GROUND:		return .5f;
		case ICE:			return 2.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return 1.f;
		case STEEL:			return .5f;
		case WATER:			return 1.f;
		default:			return 1.f;
		}
	case STEEL:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return 1.f;
		case ELECTRIC:		return .5f;
		case FAIRY:			return 2.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return .5f;
		case FLYING:		return 1.f;
		case GHOST:			return 1.f;
		case GRASS:			return 1.f;
		case GROUND:		return 1.f;
		case ICE:			return 2.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return 2.f;
		case STEEL:			return .5f;
		case WATER:			return .5f;
		default:			return 1.f;
		}
	case WATER:
		switch (defending) {
		case BUG:			return 1.f;
		case DRAGON:		return .5f;
		case ELECTRIC:		return 1.f;
		case FAIRY:			return 1.f;
		case DARK:			return 1.f;
		case FIGHTING:		return 1.f;
		case FIRE:			return 2.f;
		case FLYING:		return 1.f;
		case GHOST:			return 1.f;
		case GRASS:			return .5f;
		case GROUND:		return 2.f;
		case ICE:			return 1.f;
		case NORMAL:		return 1.f;
		case POISON:		return 1.f;
		case PSYCHIC:		return 1.f;
		case ROCK:			return 2.f;
		case STEEL:			return 1.f;
		case WATER:			return .5f;
		default:			return 1.f;
		}
	}
	return 1.f;
}