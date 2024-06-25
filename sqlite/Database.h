#ifndef DATABASE_H
#define DATABASE_H


#include <map>
#include <array>
#include <vector>
#include "sqlite3.h"
#include "../PokemonStats.h"
#include "../attack/AttackStats.h"
#include "../attack/AttackInfo.h"
#include "../Player.h"
#include <iostream>
#include <sstream>
#include "../ui/TextBoxManager.h"
#include "../Character.h"

struct LevelInfo;

struct Wave {
	struct Prefab {
		int id{};
		int level{};

		int amount{};
		float initialSpawnTimer{};
		float timeBetweenSpawn{};
		std::array<AttackStats*, 4> attacks{};
		bool canCatch{ true };
	};
	std::vector<Prefab> prefabs{};
	int number{};
};

struct PickupStats {
	sf::Texture* texture;
	sf::Vector2f location;
};

namespace Database
{
	extern sqlite3* db;
	extern std::map<int, PokemonStats> baseStats;
	extern std::map<int, AttackStats> allAttackStats;
	extern std::map<int, AttackInfo> allAttackInfo;
	extern std::map<int, sf::Texture> pickupTextures;

	AttackInfo* getAttackInfo(int id);
	AttackStats* getPokemonAttack(int id, int level);
	LevelInfo getLevelInformation(int id);
	void getLevelMessages(std::queue<MessageInfo>& messageQueue, int level);
	Wave getWave(int level,int waveNumber);
	sf::Texture* getPickupTexture(int id);
	std::vector<PickupStats>getPickups(int level);

	std::vector<AttackStats*>getRelearnAttacks(int id, int level);
	std::vector<AttackStats*>getTMAttacks(int id);

	SaveInfo getSaveInformation(int saveId);
	void loadSave(int saveId, Player& player);
	void save(int saveId, Player& player);
	void deleteSave(int saveId);

	int canEvolve(int pokemonId, int level);
	void init();

};


#endif