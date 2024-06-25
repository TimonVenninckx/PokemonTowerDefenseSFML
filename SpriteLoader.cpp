#include "SpriteLoader.h"
#include <iostream>

namespace SpriteLoader
{

    std::map<int, sf::Texture*> pokemonSprites;
    std::map<int, sf::Texture*> attackSprites;

    sf::Texture* getPokemonSprite(int id)
    {
        if (SpriteLoader::pokemonSprites.count(id)) {
            //std::cout << "found texture\n";
            return SpriteLoader::pokemonSprites[id];
        }

        sf::Texture* newTexture = new sf::Texture;

        if(id < 10)
            newTexture->loadFromFile("textures/pokemon/00" + std::to_string(id) +".png");
        else if(id < 100)
            newTexture->loadFromFile("textures/pokemon/0" + std::to_string(id) + ".png");
        else
            newTexture->loadFromFile("textures/pokemon/" + std::to_string(id) + ".png");

        SpriteLoader::pokemonSprites.emplace(id, newTexture);
        //std::cout << "Created new Texture" + std::to_string(id) + '\n';

        return newTexture;
    }

    sf::Texture* getAttackSprite(int id)
    {
        if (SpriteLoader::attackSprites.find(id) != SpriteLoader::attackSprites.end()) {
            //std::cout << "found attack texture\n ";
            return SpriteLoader::attackSprites[id];
        }

        sf::Texture* newTexture = new sf::Texture;
        
        newTexture->loadFromFile("textures/attacks/flametest.png");

        SpriteLoader::attackSprites.emplace(id, newTexture);
        //std::cout << "Created new attack Texture\n";

        return newTexture;
    }
}