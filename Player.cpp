#include "Player.h"
#include "PlayerPokemon.h"
#include <iostream>
#include "Random.h"
#include "sqlite/Database.h"


Player::Player() {

	
	//std::array<AttackStats*, 4> pokemonAttacks{ &Database::allAttackStats[33],&Database::allAttackStats[53],&Database::allAttackStats[85] ,nullptr };
	std::array<AttackStats*, 4> pokemonAttacks{ &Database::allAttackStats[33],&Database::allAttackStats[89],&Database::allAttackStats[58] ,&Database::allAttackStats[53] };

	std::array<AttackStats*, 4> iceAttacks{ &Database::allAttackStats[58],&Database::allAttackStats[423],&Database::allAttackStats[57],&Database::allAttackStats[486] };

	std::array<AttackStats*, 4> electricAttacks{ &Database::allAttackStats[423],&Database::allAttackStats[18],&Database::allAttackStats[86],&Database::allAttackStats[16] };
	
	{
		PlayerPokemon* pokemon = new PlayerPokemon{ &Database::baseStats[144], 100,SpriteLoader::getPokemonSprite(144),pokemonAttacks };
		activePokemon[0] = pokemon;
	}
	//2
	{
		PlayerPokemon* pokemon = new PlayerPokemon{&Database::baseStats[4], 5, SpriteLoader::getPokemonSprite(4), iceAttacks};
		activePokemon[2] = pokemon;
	}
	//3
	
	for (int i{ 0 }; i < 30; i++){
		//if (Random::get(1, 30) < 20)
		//	reservePokemon.emplace_back(nullptr);

		PlayerPokemon* pokemon = new PlayerPokemon{ &Database::baseStats[15 + i], 5 + i,SpriteLoader::getPokemonSprite(15 + i),electricAttacks };
		reservePokemon[i] = pokemon;
	}
	//4
	/*
	{
		PlayerPokemon pokemon{ &Database::baseStats[150], 5,SpriteLoader::getPokemonSprite(150),pokemonAttacks };
		activePokemon.emplace_back(pokemon);
	}
	*/
}

Player::~Player()
{
}

void Player::setProgression(int progression)
{
	m_info.progression = progression;
}

int Player::getProgression()
{
	return m_info.progression;
}

std::string Player::getName()
{
	return m_info.name;
}

int Player::getMoney()
{
	return m_info.money;
}

int Player::amountOfPokemon()
{
	int amount = 0;
	for (int i{ 0 }; i < activePokemon.size(); i++) {
		if (activePokemon[i])
			amount++;
	}
	for (int i{ 0 }; i < reservePokemon.size(); i++) {
		if (reservePokemon[i])
			amount++;
	}
	return amount;
}

void Player::addMoney(int amount)
{
	m_info.money += amount;
}

void Player::createSave(SaveInfo info)
{
	m_info = info;
	for (int i{ 1 }; i < activePokemon.size(); i++) { // starts at 1 because starter pokemon will be at that position;
		delete activePokemon[i];
		activePokemon[i] = nullptr;
	}

	for (int i{ 0 }; i < reservePokemon.size(); i++) {
		delete reservePokemon[i];
		reservePokemon[i] = nullptr;
	}
}

int Player::getCurrentSave()
{
	return m_info.id;
}

void Player::loadSave(int saveid)
{
	m_info.id = saveid;
	
	for (int i{ 0 }; i < activePokemon.size(); i++) {
		delete activePokemon[i];
		activePokemon[i] = nullptr;
	}

	for (int i{ 0 }; i < reservePokemon.size(); i++) {
		delete reservePokemon[i];
		reservePokemon[i] = nullptr;
	}
	//*/

	Database::loadSave(saveid,*this);
	m_info = Database::getSaveInformation(saveid);
}

void Player::save()
{
	/*{
		std::array<AttackStats*, 4> pokemonAttacks{ &Database::allAttackStats[33],&Database::allAttackStats[89],&Database::allAttackStats[58] ,&Database::allAttackStats[53] };
		PlayerPokemon* pokemon = new PlayerPokemon{ &Database::baseStats[144], 100,SpriteLoader::getPokemonSprite(144),pokemonAttacks };
		activePokemon[0] = pokemon;
	}*/
	Database::save(m_info.id, *this);
}


