#ifndef POKEMONSTATS_H
#define POKEMONSTATS_H

#include <SFML/Graphics.hpp>
#include <string>


struct PokemonStats {
	enum Types {
		NONE,			//0
		BUG, 			//1
		DARK, 			//2
		DRAGON,			//3
		ELECTRIC, 		//4
		FAIRY, 			//5
		FIGHTING,		//6
		FIRE, 			//7
		FLYING, 		//8
		GHOST,			//9
		GRASS, 			//10
		GROUND, 		//11
		ICE,			//12
		NORMAL, 		//13
		POISON, 		//14
		PSYCHIC,		//15
		ROCK, 			//16
		STEEL, 			//17
		WATER,			//18
		MAX_TYPES		//19
	};
	static float effectiveness(Types attacking, Types defending);


	static sf::Color getTypeColor(Types);
	static std::string getTypeString(Types type);


	int id{ 0 };
	std::string name{};
	Types type1{ NONE };
	Types type2{ NONE };
	int level{ 1 };
	float maxHealth{0.f};
	float health{ 10.f };
	float attack{ 10.f };
	float spAttack{ 10.f };
	float defense{ 10.f };
	float spDefence{ 10.f };
	float speed{ 10.f };
	float experience{ 0.f };

};


#endif