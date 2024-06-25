#ifndef SPRITELOADER_H
#define SPRITELOADER_H

#include <SFML/Graphics.hpp>

namespace SpriteLoader
{

	extern std::map<int, sf::Texture*> pokemonSprites;
	extern std::map<int, sf::Texture*> attackSprites;

	sf::Texture* getPokemonSprite(int id);
	sf::Texture* getAttackSprite(int id);

};

#endif