#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "SpriteLoader.h"
#include "PlayerPokemon.h"

struct SaveInfo {
	int id{};
	std::string name;
	int money{};
	int progression{};
	int pokemonAmount{};
};

class Player
{
protected:
	SaveInfo m_info;
public:
	Player();
	~Player();
	void setProgression(int progression);
	int getProgression();
	std::string getName();
	int getMoney();
	int amountOfPokemon();
	void addMoney(int amount);
	std::array<PlayerPokemon*,6> activePokemon;
	std::array<PlayerPokemon*,150> reservePokemon;

	void createSave(SaveInfo info);
	int getCurrentSave();
	void loadSave(int saveid);
	void save();
};





#endif

