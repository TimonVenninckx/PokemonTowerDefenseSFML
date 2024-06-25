#include "Database.h"
#include "../SpriteLoader.h"
#include "../Level.h"
#include "../StatusEffect.h"

std::map<int, PokemonStats> Database::baseStats;	
std::map<int, AttackStats> Database::allAttackStats;
std::map<int, AttackInfo> Database::allAttackInfo;
std::map<int, sf::Texture> Database::pickupTextures;

sqlite3* Database::db;


static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}


AttackInfo* Database::getAttackInfo(int id)
{
	if (id == 0)
		return nullptr;

	if (allAttackInfo.count(id)) {
		//std::cout << "Attack: " << id << " already exists\n";
		return &allAttackInfo[id];
	}

	std::ostringstream sql;
	sqlite3_stmt* stmt;

	sql << "SELECT * FROM ATTACKINFO WHERE " \
		"ID = " << id << ';';

	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if (rc != SQLITE_OK) {
		std::cout << "database.cpp ERROR GETTING ATTACK INFO\n";
		return nullptr;
	}

	if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		//std::cout << "Create Attack info id: "<< id <<'\n';
		AttackInfo info;
		info.id = id;
		info.attackClass = static_cast<AttackStats::AttackClass>(sqlite3_column_int(stmt, 1));
		info.frames = sqlite3_column_int(stmt, 3);
		info.frameTime = static_cast<float>(sqlite3_column_double(stmt, 4));
		info.attackSpeed = static_cast<float>(sqlite3_column_double(stmt, 5));

		if (((char*)sqlite3_column_text(stmt, 2))[0] != '\0') {
			std::string location{ "textures/attacks/" };
			location += (char*)sqlite3_column_text(stmt, 2);
			info.texture.loadFromFile(location);
		}
		allAttackInfo.emplace(id, info);
		sqlite3_finalize(stmt);
		return &allAttackInfo[id];
	}
	sqlite3_finalize(stmt);
	return nullptr;
}

AttackStats* Database::getPokemonAttack(int id, int level)
{
	std::ostringstream sql;

	sql << "SELECT ATTACKID FROM ATTACKSLEARNED WHERE " \
		"POKEMONID = " << id << " AND LEVEL = " << level << ';';
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if (rc != SQLITE_OK) {
		std::cout << "database.cpp ERROR GETTING POKEMON ATTACK\n";
		return nullptr;
	}

	if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		std::cout << "FOUND MOVE\n";
		sqlite3_finalize(stmt);
		return &allAttackStats[sqlite3_column_int(stmt, 0)];
	}
	sqlite3_finalize(stmt);
	return nullptr;
}

LevelInfo Database::getLevelInformation(int id)
{
	LevelInfo level;
	level.id = id;


	std::ostringstream sql;

	sql << "SELECT * FROM LEVELS WHERE " \
		"ID = " << id << ';';

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if (rc != SQLITE_OK) {
		std::cout << "database.cpp ERROR GETTING LEVEL INFORMATION\n";
		return level;
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		std::cout << "FOUND LEVEL\n";
		level.name		   =	  (char*)sqlite3_column_text(stmt, 1);
		level.fileLocation =	  (char*)sqlite3_column_text(stmt, 2);
		level.thumbnailLocation = (char*)sqlite3_column_text(stmt, 3);
		level.waves				=		 sqlite3_column_int (stmt, 4);
		level.winText		    = (char*)sqlite3_column_text(stmt, 5);
		level.loseText		    = (char*)sqlite3_column_text(stmt, 6);
		level.deadEnd			= sqlite3_column_int(stmt, 7);
		level.songPath			= (char*)sqlite3_column_text(stmt, 8);
		level.winImage			= (char*)sqlite3_column_text(stmt, 9);
		level.loseImage			= (char*)sqlite3_column_text(stmt, 10);
	}
	sqlite3_finalize(stmt);

	sql = std::ostringstream(std::string(""));
	sql << "SELECT * FROM LEVELPOKEMONPLACEMENTS WHERE " \
		" LEVELID = " << id << ';';

	rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		level.placementLocations.push_back({ static_cast<float>(sqlite3_column_double(stmt,1)),static_cast<float>(sqlite3_column_double(stmt,2)) });
	}
	sqlite3_finalize(stmt);

	// level path
	sql = std::ostringstream(std::string(""));
	sql << "SELECT * FROM LEVELPATH WHERE " \
		" LEVELID = " << id << ';';

	rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		level.path.push_back({ static_cast<float>(sqlite3_column_double(stmt,1)),static_cast<float>(sqlite3_column_double(stmt,2)) });
	}

	sqlite3_finalize(stmt);
	
	// level characters

	sql = std::ostringstream(std::string(""));
	sql << "SELECT * FROM LEVELCHARACTERS WHERE " \
		" LEVELID = " << id << ';';

	rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		level.characters.push_back({ (char*)sqlite3_column_text(stmt,1),sf::Vector2f{static_cast<float>(sqlite3_column_double(stmt,2)),static_cast<float>(sqlite3_column_double(stmt,3))}});
	}

	sqlite3_finalize(stmt);



	return level;
}

void Database::getLevelMessages(std::queue<MessageInfo>& messageQueue, int level)
{

	std::ostringstream sql;

	sql << "SELECT * FROM LEVELMESSAGES WHERE " \
		"LEVELID = " << level << ';';

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		MessageInfo message;
		message.text = (char*)sqlite3_column_text(stmt, 1);
		message.characterLocation = (char*)sqlite3_column_text(stmt, 2);
		messageQueue.push(message);
	}
	sqlite3_finalize(stmt);
}

Wave Database::getWave(int level, int waveNumber)
{
	Wave wave;
	wave.number = waveNumber;
	 
	std::ostringstream sql;

	sql << "SELECT * FROM WAVEPOKEMON WHERE "
		"LEVELID = " << level << " AND WAVE = " << waveNumber << ';';

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		Wave::Prefab prefab;
		prefab.id    = sqlite3_column_int(stmt, 2); // set prefab id so we can read pokemon
		prefab.level = sqlite3_column_int(stmt, 3);
		prefab.amount = sqlite3_column_int(stmt, 4);
		prefab.initialSpawnTimer = static_cast<float>(sqlite3_column_double(stmt, 5));
		prefab.timeBetweenSpawn = static_cast<float>(sqlite3_column_double(stmt, 6));
		prefab.canCatch			= sqlite3_column_int(stmt, 7);
		wave.prefabs.emplace_back(prefab);
	}
	sqlite3_finalize(stmt);


	if (wave.prefabs.empty()) {
		wave.number = 0;
		return wave;
	}

	for (int i{ 0 }; i < wave.prefabs.size(); i++) {
		sql = std::ostringstream(std::string(""));
		sql << "SELECT * FROM POKEMONPREFAB WHERE "
			"ID = " << wave.prefabs[i].id << ';';
		std::cout << sql.str() << '\n';
		rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);
		if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
			wave.prefabs[i].id = sqlite3_column_int(stmt, 1);
			wave.prefabs[i].attacks[0] = &allAttackStats[sqlite3_column_int(stmt, 2)];
			wave.prefabs[i].attacks[1] = &allAttackStats[sqlite3_column_int(stmt, 3)];
			wave.prefabs[i].attacks[2] = &allAttackStats[sqlite3_column_int(stmt, 4)];
			wave.prefabs[i].attacks[3] = &allAttackStats[sqlite3_column_int(stmt, 5)];
		}
	}
	
	sqlite3_finalize(stmt);
	return wave;
}

sf::Texture* Database::getPickupTexture(int id)
{
	if (pickupTextures.count(id))
		return &pickupTextures[id];
	sf::Texture texture;

	std::ostringstream sql;
	sql << "SELECT * FROM PICKUPPREFABS WHERE "\
		"ID = " << id << ';';
	
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		std::ostringstream location;
		location << "textures/" << (char*)sqlite3_column_text(stmt, 1);
		texture.loadFromFile(location.str());
		pickupTextures.insert(std::pair<int, sf::Texture>(id, texture));
		return &pickupTextures[id];
	}
	std::cout << "DATABBASE.CPP we couldn't find the pickup texture";
	sqlite3_finalize(stmt);
	return &pickupTextures[0];
}

std::vector<PickupStats> Database::getPickups(int level)
{
	std::vector<PickupStats> positions;

	std::ostringstream sql;
	sql << "SELECT * FROM LEVELPICKUPS WHERE " \
		"LEVELID = " << level << ';';
	
	sqlite3_stmt* stmt;

	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if (rc != SQLITE_OK) {
		std::cout << "Database connection eror pickuppositions\n" << stmt << '\n';
		return positions;
	}
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		PickupStats stats;
		stats.location.x = static_cast<float>(sqlite3_column_double(stmt, 1));
		stats.location.y = static_cast<float>(sqlite3_column_double(stmt, 2));
		stats.texture = getPickupTexture(sqlite3_column_int(stmt, 3));
		positions.emplace_back(stats);
	}

	sqlite3_finalize(stmt);
	return positions;
}

std::vector<AttackStats*> Database::getRelearnAttacks(int id, int level)
{
	std::vector<AttackStats*> allAttacks;

	std::ostringstream sql;
	sql << "SELECT * FROM ATTACKSLEARNED WHERE " \
		"POKEMONID = " << id << " AND LEVEL < " << level << ';';

	sqlite3_stmt* stmt;

	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if (rc != SQLITE_OK) {
		std::cout << "Database connection eror pickuppositions\n" << stmt << '\n';
		return allAttacks;
	}
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		AttackStats* attack = &allAttackStats[sqlite3_column_int(stmt, 1)];
		if(attack)
			allAttacks.emplace_back(attack);
	}
	sqlite3_finalize(stmt);
	return allAttacks;
}

std::vector<AttackStats*> Database::getTMAttacks(int id)
{
	std::vector<AttackStats*> allAttacks;

	std::ostringstream sql;
	sql << "SELECT * FROM ATTACKHM WHERE " \
		"POKEMONID = " << id << ';';

	sqlite3_stmt* stmt;

	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if (rc != SQLITE_OK) {
		std::cout << "Database connection eror pickuppositions\n" << stmt << '\n';
		return allAttacks;
	}
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		AttackStats* attack = &allAttackStats[sqlite3_column_int(stmt, 0)];
		if (attack)
			allAttacks.emplace_back(attack);
	}
	sqlite3_finalize(stmt);
	return allAttacks;
}

SaveInfo Database::getSaveInformation(int saveId)
{
	std::ostringstream sql;

	sql << "SELECT * FROM SAVEINFO WHERE " \
		"ID = " << saveId << ';';


	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	SaveInfo info{saveId,"Empty",0,0};
	if (rc != SQLITE_OK) {
		// handle the error
		std::cout << "Database connection error\n" << stmt << '\n';
		return info;
	}

	
	if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		info.name = (char*)sqlite3_column_text(stmt, 1);
		info.money = sqlite3_column_int(stmt, 2);
		info.progression = sqlite3_column_int(stmt, 3);
		info.pokemonAmount = sqlite3_column_int(stmt, 4);
	}
	sqlite3_finalize(stmt);
	return info;
}

void Database::loadSave(int saveId, Player& player)
{
	std::ostringstream sql;

	sql << "SELECT * FROM SAVEPOKEMON WHERE " \
		"SAVEID = " << saveId << ';';


	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if (rc != SQLITE_OK) {
		// handle the error
		std::cout << "Database connection error\n" << stmt << '\n';
		return;
	}


	while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int inventoryPos = sqlite3_column_int(stmt, 1);
		int pokemonId = sqlite3_column_int(stmt, 2);
		int level = sqlite3_column_int(stmt, 3);
		std::array<AttackStats*, 4> attacks;

		for (int i{ 0 }; i < 4; i++) {
			int attackid = sqlite3_column_int(stmt, 4 + i);
			if (allAttackStats.count(attackid))
				attacks[i] = &allAttackStats[attackid];
			else 
				attacks[i] = nullptr;
		}

		PlayerPokemon* pokemon = new PlayerPokemon(&baseStats[pokemonId], level, SpriteLoader::getPokemonSprite(pokemonId), attacks);
		if (inventoryPos < 6)
			player.activePokemon[inventoryPos] = pokemon;
		else {
			inventoryPos -= 6;
			if (player.reservePokemon.size() > inventoryPos)
				player.reservePokemon[inventoryPos] = pokemon;
		}
	}
	sqlite3_finalize(stmt);
}

void Database::save(int saveId, Player& player)
{
	std::ostringstream sql;
	char* errMsg = 0;

	sql << "DELETE FROM SAVEPOKEMON WHERE " \
		"SAVEID = " << saveId << ';';

	//int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);
	int rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		// handle the error
		std::cout << "Database connection error SAVING\n" << errMsg << '\n';
		return;
	}


	sql = std::ostringstream(std::string(""));

	// active pokemon
	for (int i{ 0 }; i < player.activePokemon.size(); i++) {
		if (player.activePokemon[i] == nullptr)
			continue;

		std::array<int,4> attacks;
		for (int j{ 0 }; j < 4; j++) {
			attacks[j] = ((*player.activePokemon[i]->getAttacks())[j] ? (*player.activePokemon[i]->getAttacks())[j]->id : 0 );
		}

		std::cout << "saved pokemon " << i << '\n';
		sql << "INSERT INTO SAVEPOKEMON(SAVEID,INVENTORYPOS,POKEMONID,LEVEL,MOVE1,MOVE2,MOVE3,MOVE4) "
			<< "VALUES (" << saveId << ',' << i << ','
			<< player.activePokemon[i]->getStats().id << ',' << player.activePokemon[i]->getStats().level << ','
			<< attacks[0] << ',' << attacks[1] << ',' << attacks[2] << ',' << attacks[3] << "); ";
		std::cout << sql.str() << '\n';

		rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);
		sql = std::ostringstream(std::string(""));

		if (rc != SQLITE_OK) {
			// handle the error
			std::cout << "Database connection error saving pokemon\n" << errMsg << '\n';
			return;
		}
	}
	//inactive pokemon
	for (int i{ 0 }; i < player.reservePokemon.size(); i++) {
		if (player.reservePokemon[i] == nullptr)
			continue;

		std::array<int, 4> attacks;
		for (int j{ 0 }; j < 4; j++) {
			attacks[j] = ((*player.reservePokemon[i]->getAttacks())[j] ? (*player.reservePokemon[i]->getAttacks())[j]->id : 0);
		}

		std::cout << "saved pokemon " << i << '\n';
		sql << "INSERT INTO SAVEPOKEMON(SAVEID,INVENTORYPOS,POKEMONID,LEVEL,MOVE1,MOVE2,MOVE3,MOVE4) "
			<< "VALUES (" << saveId << ',' << i + 6 << ','
			<< player.reservePokemon[i]->getStats().id << ',' << player.reservePokemon[i]->getStats().level << ','
			<< attacks[0] << ',' << attacks[1] << ',' << attacks[2] << ',' << attacks[3] << "); ";
		std::cout << sql.str() << '\n';

		rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);
		sql = std::ostringstream(std::string(""));

		if (rc != SQLITE_OK) {
			// handle the error
			std::cout << "Database connection error saving pokemon\n" << errMsg << '\n';
			return;
		}
	}
	// update stats

	sql = std::ostringstream(std::string(""));

	sql << "INSERT OR REPLACE INTO SAVEINFO "
		<< "VALUES (" << player.getCurrentSave() << ",'" << player.getName() << "'," << player.getMoney() << ',' << player.getProgression() << ',' << player.amountOfPokemon() << ");";

	rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		// handle the error
		std::cout << sql.str() << '\n';
		std::cout << "Database connection error saving save stats\n" << errMsg << '\n';
		return;
	}
}

void Database::deleteSave(int saveId)
{
	std::ostringstream sql;
	char* errMsg = 0;

	sql << "DELETE FROM SAVEPOKEMON WHERE " \
		"SAVEID = " << saveId << ';';

	//int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);
	int rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		// handle the error
		std::cout << "Database connection error deleting save\n" << errMsg << '\n';
		return;
	}

	sql = std::ostringstream(std::string(""));

	sql << "DELETE FROM SAVEINFO WHERE " \
		"ID = " << saveId << ';';

	//int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);
	rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		// handle the error
		std::cout << "Database connection error deleting save\n" << errMsg << '\n';
		return;
	}


}




int Database::canEvolve(int pokemonId, int level)
{
	std::ostringstream sql;
	
	sql << "SELECT EVOLUTIONID,LEVEL FROM EVOLUTION WHERE " \
		"ID = " << pokemonId << ';';

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if (rc != SQLITE_OK) {
		// handle the error
		std::cout << "Database connection error\n" << stmt << '\n';
		return 0;
	}

	int evolutionId{};
	int levelNeeded{};
	if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		std::cout << "Found evolution\n";
		evolutionId = sqlite3_column_int(stmt, 0);
		levelNeeded = sqlite3_column_int(stmt, 1);
	}
	else {
		std::cout << "No evolution found\n";
		return 0;
	}
	if (level < levelNeeded) {
		std::cout << "You need to be level: " << levelNeeded << " to evolve into id: " << evolutionId << '\n';
		return 0;
	}
	sqlite3_finalize(stmt);
	return evolutionId;
}



void Database::init()
{

	char* errMsg = 0;
	int rc;
	std::ostringstream sql;

	rc = sqlite3_open("sqlite/Pokemon.db", &db);
	if (rc != SQLITE_OK)
		std::cout << "file didnt open\n";
	else
		std::cout << "file opened\n";



	sql << "SELECT * FROM BASEPOKEMON";


	sqlite3_stmt* stmt;
	rc = sqlite3_prepare_v2(db, sql.str().c_str(),static_cast<int>(sql.str().size()), &stmt, nullptr);
	if (rc != SQLITE_OK) {
		std::cout << "Init error";
		// handle the error
	}

	// Loop through the results, a row at a time.
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

		PokemonStats stats;
		stats.id =			sqlite3_column_int(stmt, 0);
		stats.name =		(const char*)sqlite3_column_text(stmt, 1);
		stats.type1 =		static_cast<PokemonStats::Types>(sqlite3_column_int(stmt, 2));
		stats.type2 =		static_cast<PokemonStats::Types>(sqlite3_column_int(stmt, 3));
		stats.maxHealth =	static_cast<float>(sqlite3_column_double(stmt, 4));
		stats.attack =		static_cast<float>(sqlite3_column_double(stmt, 5));
		stats.spAttack =	static_cast<float>(sqlite3_column_double(stmt, 6));
		stats.defense =		static_cast<float>(sqlite3_column_double(stmt, 7));
		stats.spDefence =	static_cast<float>(sqlite3_column_double(stmt, 8));
		stats.speed =		static_cast<float>(sqlite3_column_double(stmt, 9));

		stats.health = stats.maxHealth;
		baseStats.emplace(stats.id,stats);
	}
	// Free the statement when done.
	sqlite3_finalize(stmt);

	sql = std::ostringstream(std::string(""));

	sql << "SELECT * FROM ATTACKSTATS";

	rc = sqlite3_prepare_v2(db, sql.str().c_str(), static_cast<int>(sql.str().size()), &stmt, nullptr);

	if (rc != SQLITE_OK)
		std::cout << "init error v2";

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

		AttackStats stats;
		stats.id =				sqlite3_column_int(stmt, 0);
		stats.name =			(const char*)sqlite3_column_text(stmt, 1);
		stats.type =			static_cast<PokemonStats::Types>(sqlite3_column_int(stmt, 2));
		stats.category =		static_cast<AttackStats::AttackCategory>(sqlite3_column_int(stmt, 3));
		stats.power =			static_cast<float>(sqlite3_column_double(stmt, 4));
		stats.accuracy =		static_cast<float>(sqlite3_column_double(stmt, 5));
		stats.attackCooldown =	static_cast<float>(sqlite3_column_double(stmt, 6));
		stats.info =			(const char*)sqlite3_column_text(stmt, 7);
		stats.contact =			sqlite3_column_int(stmt, 8);
		stats.statusEffect =	static_cast<StatusEffect::EffectType>(sqlite3_column_int(stmt, 9));
		stats.statusEffectAccuracy = sqlite3_column_int(stmt, 10);
		stats.additionalEffect = static_cast<AttackStats::AdditionalEffects>(sqlite3_column_int(stmt, 11));

		stats.attackClass = getAttackInfo(stats.id)->attackClass;
		allAttackStats.emplace(stats.id, stats);
	}
	sqlite3_finalize(stmt);

	for (auto stat : allAttackStats) {
		std::cout << stat.second.id << " name: " << stat.second.name << '\n';
	}
	
}






/*sql << "CREATE TABLE BASEPOKEMON(" \
	"ID INT PRIMARY KEY		NOT NULL," \
	"NAME			TEXT	NOT NULL," \
	"TYPE1			INT 	NOT NULL," \
	"TYPE2			INT 	NOT NULL," \
	"HP				REAL 	NOT NULL," \
	"ATK			REAL 	NOT NULL," \
	"SPATK			REAL 	NOT NULL," \
	"DEF			REAL 	NOT NULL," \
	"SPDEF			REAL 	NOT NULL," \
	"SPEED			REAL 	NOT NULL);";

sql << "CREATE TABLE EVOLUTION("   \
	"ID INT PRIMARY KEY NOT NULL," \
	"EVOLUTIONID	INT NOT NULL," \
	"LEVEL			INT NOT NULL);";



sql << "CREATE TABLE ATTACKSTATS(" \
	"ID INT PRIMARY KEY		NOT NULL," \
	"NAME TEXT				NOT NULL," \
	"TYPE INT				NOT NULL," \
	"CATEGORY INT			NOT NULL," \
	"POWER REAL				NOT NULL," \
	"ACCURACY REAL			NOT NULL," \
	"COOLDOWN REAL			NOT NULL," \
	"DESCRIPTION TEXT		NOT NULL," \
	"CONTACT INT			NOT NULL);";


sql << "CREATE TABLE ATTACKSLEARNED(" \
	"POKEMONID INT NOT NULL,"
	"ATTACKID  INT NOT NULL,"
	"LEVEL	   INT NOT NULL);";

sql = std::ostringstream(std::string(""));

sql << "CREATE TABLE ATTACKINFO(" \
	"ID		   INT		NOT NULL," \
	"CLASS	   INT		NOT NULL," \
	"TEXTURE   TEXT		NOT NULL);";


rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);


rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);
sql = std::ostringstream(std::string(""));

sql << "INSERT INTO BASEPOKEMON(ID,NAME,TYPE1,TYPE2,LEVEL,HP,ATK,SPATK,DEF,SPDEF,SPEED) " \
	<< "VALUES (" << stats.id <<  ", '"  <<  stats.name + "'," \
	<< stats.type1 << ", " << stats.type2 << ", " << stats.level << ", " << stats.maxHealth << ", " \
	<< stats.attack << ", " << stats.spAttack << ", " << stats.defense << ", " \
	<< stats.spDefence << ", " << stats.speed << ");";


rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);
sql = std::ostringstream(std::string(""));*/

/* IMPORTING

float invalid;
for (int i{ 0 }; i < 100; ++i) {
	PokemonStats stats;
	std::cin >> stats.id >> stats.name >> stats.name >> stats.health >> stats.attack >> stats.defense >> stats.spAttack >> stats.spDefence >> stats.speed >> invalid >> invalid;
	sql << "INSERT INTO BASEPOKEMON(ID,NAME,TYPE1,TYPE2,HP,ATK,SPATK,DEF,SPDEF,SPEED) "
	<< "VALUES (" << stats.id <<  ", '"  <<  stats.name + "',"
	<< stats.type1 << ", " << stats.type2 << ", " << stats.health << ", "
	<< stats.attack << ", " << stats.spAttack << ", " << stats.defense << ", "
	<< stats.spDefence << ", " << stats.speed << "); ";
	rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &errMsg);
	sql = std::ostringstream(std::string(""));
}

*/




/*
0001 	Bulbasaur 	Bulbasaur 	45 	49 	49 	65 	65 	45 	318 	53
0002 	Ivysaur 	Ivysaur 	60 	62 	63 	80 	80 	60 	405 	67.5
0003 	Venusaur 	Venusaur 	80 	82 	83 	100 	100 	80 	525 	87.5
0004 	Charmander 	Charmander 	39 	52 	43 	60 	50 	65 	309 	51.5
0005 	Charmeleon 	Charmeleon 	58 	64 	58 	80 	65 	80 	405 	67.5
0006 	Charizard 	Charizard 	78 	84 	78 	109 	85 	100 	534 	89
0007 	Squirtle 	Squirtle 	44 	48 	65 	50 	64 	43 	314 	52.33
0008 	Wartortle 	Wartortle 	59 	63 	80 	65 	80 	58 	405 	67.5
0009 	Blastoise 	Blastoise 	79 	83 	100 	85 	105 	78 	530 	88.33
0010 	Caterpie 	Caterpie 	45 	30 	35 	20 	20 	45 	195 	32.5
0011 	Metapod 	Metapod 	50 	20 	55 	25 	25 	30 	205 	34.17
0012 	Butterfree 	Butterfree 	60 	45 	50 	80 	80 	70 	385 	64.17
0013 	Weedle 	Weedle 	40 	35 	30 	20 	20 	50 	195 	32.5
0014 	Kakuna 	Kakuna 	45 	25 	50 	25 	25 	35 	205 	34.17
0015 	Beedrill 	Beedrill 	65 	80 	40 	45 	80 	75 	385 	64.17
0016 	Pidgey 	Pidgey 	40 	45 	40 	35 	35 	56 	251 	41.83
0017 	Pidgeotto 	Pidgeotto 	63 	60 	55 	50 	50 	71 	349 	58.17
0018 	Pidgeot 	Pidgeot 	83 	80 	75 	70 	70 	91 	469 	78.17
0019 	Rattata 	Rattata 	30 	56 	35 	25 	35 	72 	253 	42.17
0020 	Raticate 	Raticate 	55 	81 	60 	50 	70 	97 	413 	68.83
0021 	Spearow 	Spearow 	40 	60 	30 	31 	31 	70 	262 	43.67
0022 	Fearow 	Fearow 	65 	90 	65 	61 	61 	100 	442 	73.67
0023 	Ekans 	Ekans 	35 	60 	44 	40 	54 	55 	288 	48
0024 	Arbok 	Arbok 	60 	85 	69 	65 	79 	80 	438 	73
0025 	Pikachu 	Pikachu 	35 	55 	30 	50 	40 	90 	300 	50
0026 	Raichu 	Raichu 	60 	90 	55 	90 	80 	100 	475 	79.17
0027 	Sandshrew 	Sandshrew 	50 	75 	85 	20 	30 	40 	300 	50
0028 	Sandslash 	Sandslash 	75 	100 	110 	45 	55 	65 	450 	75
0029 	Nidoran♀ 	Nidoran♀ 	55 	47 	52 	40 	40 	41 	275 	45.83
0030 	Nidorina 	Nidorina 	70 	62 	67 	55 	55 	56 	365 	60.83
0031 	Nidoqueen 	Nidoqueen 	90 	82 	87 	75 	85 	76 	495 	82.5
0032 	Nidoran♂ 	Nidoran♂ 	46 	57 	40 	40 	40 	50 	273 	45.5
0033 	Nidorino 	Nidorino 	61 	72 	57 	55 	55 	65 	365 	60.83
0034 	Nidoking 	Nidoking 	81 	92 	77 	85 	75 	85 	495 	82.5
0035 	Clefairy 	Clefairy 	70 	45 	48 	60 	65 	35 	323 	53.83
0036 	Clefable 	Clefable 	95 	70 	73 	85 	90 	60 	473 	78.83
0037 	Vulpix 	Vulpix 	38 	41 	40 	50 	65 	65 	299 	49.83
0038 	Ninetales 	Ninetales 	73 	76 	75 	81 	100 	100 	505 	84.17
0039 	Jigglypuff 	Jigglypuff 	115 	45 	20 	45 	25 	20 	270 	45
0040 	Wigglytuff 	Wigglytuff 	140 	70 	45 	75 	50 	45 	425 	70.83
0041 	Zubat 	Zubat 	40 	45 	35 	30 	40 	55 	245 	40.83
0042 	Golbat 	Golbat 	75 	80 	70 	65 	75 	90 	455 	75.83
0043 	Oddish 	Oddish 	45 	50 	55 	75 	65 	30 	320 	53.33
0044 	Gloom 	Gloom 	60 	65 	70 	85 	75 	40 	395 	65.83
0045 	Vileplume 	Vileplume 	75 	80 	85 	100 	90 	50 	480 	80
0046 	Paras 	Paras 	35 	70 	55 	45 	55 	25 	285 	47.5
0047 	Parasect 	Parasect 	60 	95 	80 	60 	80 	30 	405 	67.5
0048 	Venonat 	Venonat 	60 	55 	50 	40 	55 	45 	305 	50.83
0049 	Venomoth 	Venomoth 	70 	65 	60 	90 	75 	90 	450 	75
0050 	Diglett 	Diglett 	10 	55 	25 	35 	45 	95 	265 	44.17
0051 	Dugtrio 	Dugtrio 	35 	80 	50 	50 	70 	120 	405 	67.5
0052 	Meowth 	Meowth 	40 	45 	35 	40 	40 	90 	290 	48.33
0053 	Persian 	Persian 	65 	70 	60 	65 	65 	115 	440 	73.33
0054 	Psyduck 	Psyduck 	50 	52 	48 	65 	50 	55 	320 	53.33
0055 	Golduck 	Golduck 	80 	82 	78 	95 	80 	85 	500 	83.33
0056 	Mankey 	Mankey 	40 	80 	35 	35 	45 	70 	305 	50.83
0057 	Primeape 	Primeape 	65 	105 	60 	60 	70 	95 	455 	75.83
0058 	Growlithe 	Growlithe 	55 	70 	45 	70 	50 	60 	350 	58.33
0059 	Arcanine 	Arcanine 	90 	110 	80 	100 	80 	95 	555 	92.5
0060 	Poliwag 	Poliwag 	40 	50 	40 	40 	40 	90 	300 	50
0061 	Poliwhirl 	Poliwhirl 	65 	65 	65 	50 	50 	90 	385 	64.17
0062 	Poliwrath 	Poliwrath 	90 	85 	95 	70 	90 	70 	500 	83.33
0063 	Abra 	Abra 	25 	20 	15 	105 	55 	90 	310 	51.67
0064 	Kadabra 	Kadabra 	40 	35 	30 	120 	70 	105 	400 	66.67
0065 	Alakazam 	Alakazam 	55 	50 	45 	135 	85 	120 	490 	81.67
0066 	Machop 	Machop 	70 	80 	50 	35 	35 	35 	305 	50.83
0067 	Machoke 	Machoke 	80 	100 	70 	50 	60 	45 	405 	67.5
0068 	Machamp 	Machamp 	90 	130 	80 	65 	85 	55 	505 	84.17
0069 	Bellsprout 	Bellsprout 	50 	75 	35 	70 	30 	40 	300 	50
0070 	Weepinbell 	Weepinbell 	65 	90 	50 	85 	45 	55 	390 	65
0071 	Victreebel 	Victreebel 	80 	105 	65 	100 	60 	70 	480 	80
0072 	Tentacool 	Tentacool 	40 	40 	35 	50 	100 	70 	335 	55.83
0073 	Tentacruel 	Tentacruel 	80 	70 	65 	80 	120 	100 	515 	85.83
0074 	Geodude 	Geodude 	40 	80 	100 	30 	30 	20 	300 	50
0075 	Graveler 	Graveler 	55 	95 	115 	45 	45 	35 	390 	65
0076 	Golem 	Golem 	80 	110 	130 	55 	65 	45 	485 	80.83
0077 	Ponyta 	Ponyta 	50 	85 	55 	65 	65 	90 	410 	68.33
0078 	Rapidash 	Rapidash 	65 	100 	70 	80 	80 	105 	500 	83.33
0079 	Slowpoke 	Slowpoke 	90 	65 	65 	40 	40 	15 	315 	52.5
0080 	Slowbro 	Slowbro 	95 	75 	110 	100 	80 	30 	490 	81.67
0081 	Magnemite 	Magnemite 	25 	35 	70 	95 	55 	45 	325 	54.17
0082 	Magneton 	Magneton 	50 	60 	95 	120 	70 	70 	465 	77.5
0083 	Farfetch'd 	Farfetch'd 	52 	65 	55 	58 	62 	60 	352 	58.67
0084 	Doduo 	Doduo 	35 	85 	45 	35 	35 	75 	310 	51.67
0085 	Dodrio 	Dodrio 	60 	110 	70 	60 	60 	100 	460 	76.67
0086 	Seel 	Seel 	65 	45 	55 	45 	70 	45 	325 	54.17
0087 	Dewgong 	Dewgong 	90 	70 	80 	70 	95 	70 	475 	79.17
0088 	Grimer 	Grimer 	80 	80 	50 	40 	50 	25 	325 	54.17
0089 	Muk 	Muk 	105 	105 	75 	65 	100 	50 	500 	83.33
0090 	Shellder 	Shellder 	30 	65 	100 	45 	25 	40 	305 	50.83
0091 	Cloyster 	Cloyster 	50 	95 	180 	85 	45 	70 	525 	87.5
0092 	Gastly 	Gastly 	30 	35 	30 	100 	35 	80 	310 	51.67
0093 	Haunter 	Haunter 	45 	50 	45 	115 	55 	95 	405 	67.5
0094 	Gengar 	Gengar 	60 	65 	60 	130 	75 	110 	500 	83.33
0095 	Onix 	Onix 	35 	45 	160 	30 	45 	70 	385 	64.17
0096 	Drowzee 	Drowzee 	60 	48 	45 	43 	90 	42 	328 	54.67
0097 	Hypno 	Hypno 	85 	73 	70 	73 	115 	67 	483 	80.5
0098 	Krabby 	Krabby 	30 	105 	90 	25 	25 	50 	325 	54.17
0099 	Kingler 	Kingler 	55 	130 	115 	50 	50 	75 	475 	79.17
0100 	Voltorb 	Voltorb 	40 	30 	50 	55 	55 	100 	330 	55
0101 	Electrode 	Electrode 	60 	50 	70 	80 	80 	140 	480 	80
0102 	Exeggcute 	Exeggcute 	60 	40 	80 	60 	45 	40 	325 	54.17
0103 	Exeggutor 	Exeggutor 	95 	95 	85 	125 	65 	55 	520 	86.67
0104 	Cubone 	Cubone 	50 	50 	95 	40 	50 	35 	320 	53.33
0105 	Marowak 	Marowak 	60 	80 	110 	50 	80 	45 	425 	70.83
0106 	Hitmonlee 	Hitmonlee 	50 	120 	53 	35 	110 	87 	455 	75.83
0107 	Hitmonchan 	Hitmonchan 	50 	105 	79 	35 	110 	76 	455 	75.83
0108 	Lickitung 	Lickitung 	90 	55 	75 	60 	75 	30 	385 	64.17
0109 	Koffing 	Koffing 	40 	65 	95 	60 	45 	35 	340 	56.67
0110 	Weezing 	Weezing 	65 	90 	120 	85 	70 	60 	490 	81.67
0111 	Rhyhorn 	Rhyhorn 	80 	85 	95 	30 	30 	25 	345 	57.5
0112 	Rhydon 	Rhydon 	105 	130 	120 	45 	45 	40 	485 	80.83
0113 	Chansey 	Chansey 	250 	5 	5 	35 	105 	50 	450 	75
0114 	Tangela 	Tangela 	65 	55 	115 	100 	40 	60 	435 	72.5
0115 	Kangaskhan 	Kangaskhan 	105 	95 	80 	40 	80 	90 	490 	81.67
0116 	Horsea 	Horsea 	30 	40 	70 	70 	25 	60 	295 	49.17
0117 	Seadra 	Seadra 	55 	65 	95 	95 	45 	85 	440 	73.33
0118 	Goldeen 	Goldeen 	45 	67 	60 	35 	50 	63 	320 	53.33
0119 	Seaking 	Seaking 	80 	92 	65 	65 	80 	68 	450 	75
0120 	Staryu 	Staryu 	30 	45 	55 	70 	55 	85 	340 	56.67
0121 	Starmie 	Starmie 	60 	75 	85 	100 	85 	115 	520 	86.67
0122 	Mr. Mime 	Mr. Mime 	40 	45 	65 	100 	120 	90 	460 	76.67
0123 	Scyther 	Scyther 	70 	110 	80 	55 	80 	105 	500 	83.33
0124 	Jynx 	Jynx 	65 	50 	35 	115 	95 	95 	455 	75.83
0125 	Electabuzz 	Electabuzz 	65 	83 	57 	95 	85 	105 	490 	81.67
0126 	Magmar 	Magmar 	65 	95 	57 	100 	85 	93 	495 	82.5
0127 	Pinsir 	Pinsir 	65 	125 	100 	55 	70 	85 	500 	83.33
0128 	Tauros 	Tauros 	75 	100 	95 	40 	70 	110 	490 	81.67
0129 	Magikarp 	Magikarp 	20 	10 	55 	15 	20 	80 	200 	33.33
0130 	Gyarados 	Gyarados 	95 	125 	79 	60 	100 	81 	540 	90
0131 	Lapras 	Lapras 	130 	85 	80 	85 	95 	60 	535 	89.17
0132 	Ditto 	Ditto 	48 	48 	48 	48 	48 	48 	288 	48
0133 	Eevee 	Eevee 	55 	55 	50 	45 	65 	55 	325 	54.17
0134 	Vaporeon 	Vaporeon 	130 	65 	60 	110 	95 	65 	525 	87.5
0135 	Jolteon 	Jolteon 	65 	65 	60 	110 	95 	130 	525 	87.5
0136 	Flareon 	Flareon 	65 	130 	60 	95 	110 	65 	525 	87.5
0137 	Porygon 	Porygon 	65 	60 	70 	85 	75 	40 	395 	65.83
0138 	Omanyte 	Omanyte 	35 	40 	100 	90 	55 	35 	355 	59.17
0139 	Omastar 	Omastar 	70 	60 	125 	115 	70 	55 	495 	82.5
0140 	Kabuto 	Kabuto 	30 	80 	90 	55 	45 	55 	355 	59.17
0141 	Kabutops 	Kabutops 	60 	115 	105 	65 	70 	80 	495 	82.5
0142 	Aerodactyl 	Aerodactyl 	80 	105 	65 	60 	75 	130 	515 	85.83
0143 	Snorlax 	Snorlax 	160 	110 	65 	65 	110 	30 	540 	90
0144 	Articuno 	Articuno 	90 	85 	100 	95 	125 	85 	580 	96.67
0145 	Zapdos 	Zapdos 	90 	90 	85 	125 	90 	100 	580 	96.67
0146 	Moltres 	Moltres 	90 	100 	90 	125 	85 	90 	580 	96.67
0147 	Dratini 	Dratini 	41 	64 	45 	50 	50 	50 	300 	50
0148 	Dragonair 	Dragonair 	61 	84 	65 	70 	70 	70 	420 	70
0149 	Dragonite 	Dragonite 	91 	134 	95 	100 	100 	80 	600 	100
0150 	Mewtwo 	Mewtwo 	106 	110 	90 	154 	90 	130 	680 	113.33
0151 	Mew 	Mew 	100 	100 	100 	100 	100 	100 	600 	100
0152 	Chikorita 	Chikorita 	45 	49 	65 	49 	65 	45 	318 	53
0153 	Bayleef 	Bayleef 	60 	62 	80 	63 	80 	60 	405 	67.5
0154 	Meganium 	Meganium 	80 	82 	100 	83 	100 	80 	525 	87.5
0155 	Cyndaquil 	Cyndaquil 	39 	52 	43 	60 	50 	65 	309 	51.5
0156 	Quilava 	Quilava 	58 	64 	58 	80 	65 	80 	405 	67.5
0157 	Typhlosion 	Typhlosion 	78 	84 	78 	109 	85 	100 	534 	89
0158 	Totodile 	Totodile 	50 	65 	64 	44 	48 	43 	314 	52.33
0159 	Croconaw 	Croconaw 	65 	80 	80 	59 	63 	58 	405 	67.5
0160 	Feraligatr 	Feraligatr 	85 	105 	100 	79 	83 	78 	530 	88.33
0161 	Sentret 	Sentret 	35 	46 	34 	35 	45 	20 	215 	35.83
0162 	Furret 	Furret 	85 	76 	64 	45 	55 	90 	415 	69.17
0163 	Hoothoot 	Hoothoot 	60 	30 	30 	36 	56 	50 	262 	43.67
0164 	Noctowl 	Noctowl 	100 	50 	50 	76 	96 	70 	442 	73.67
0165 	Ledyba 	Ledyba 	40 	20 	30 	40 	80 	55 	265 	44.17
0166 	Ledian 	Ledian 	55 	35 	50 	55 	110 	85 	390 	65
0167 	Spinarak 	Spinarak 	40 	60 	40 	40 	40 	30 	250 	41.67
0168 	Ariados 	Ariados 	70 	90 	70 	60 	60 	40 	390 	65
0169 	Crobat 	Crobat 	85 	90 	80 	70 	80 	130 	535 	89.17
0170 	Chinchou 	Chinchou 	75 	38 	38 	56 	56 	67 	330 	55
0171 	Lanturn 	Lanturn 	125 	58 	58 	76 	76 	67 	460 	76.67
0172 	Pichu 	Pichu 	20 	40 	15 	35 	35 	60 	205 	34.17
0173 	Cleffa 	Cleffa 	50 	25 	28 	45 	55 	15 	218 	36.33
0174 	Igglybuff 	Igglybuff 	90 	30 	15 	40 	20 	15 	210 	35
0175 	Togepi 	Togepi 	35 	20 	65 	40 	65 	20 	245 	40.83
0176 	Togetic 	Togetic 	55 	40 	85 	80 	105 	40 	405 	67.5
0177 	Natu 	Natu 	40 	50 	45 	70 	45 	70 	320 	53.33
0178 	Xatu 	Xatu 	65 	75 	70 	95 	70 	95 	470 	78.33
0179 	Mareep 	Mareep 	55 	40 	40 	65 	45 	35 	280 	46.67
0180 	Flaaffy 	Flaaffy 	70 	55 	55 	80 	60 	45 	365 	60.83
0181 	Ampharos 	Ampharos 	90 	75 	75 	115 	90 	55 	500 	83.33
0182 	Bellossom 	Bellossom 	75 	80 	85 	90 	100 	50 	480 	80
0183 	Marill 	Marill 	70 	20 	50 	20 	50 	40 	250 	41.67
0184 	Azumarill 	Azumarill 	100 	50 	80 	50 	80 	50 	410 	68.33
0185 	Sudowoodo 	Sudowoodo 	70 	100 	115 	30 	65 	30 	410 	68.33
0186 	Politoed 	Politoed 	90 	75 	75 	90 	100 	70 	500 	83.33
0187 	Hoppip 	Hoppip 	35 	35 	40 	35 	55 	50 	250 	41.67
0188 	Skiploom 	Skiploom 	55 	45 	50 	45 	65 	80 	340 	56.67
0189 	Jumpluff 	Jumpluff 	75 	55 	70 	55 	85 	110 	450 	75
0190 	Aipom 	Aipom 	55 	70 	55 	40 	55 	85 	360 	60
0191 	Sunkern 	Sunkern 	30 	30 	30 	30 	30 	30 	180 	30
0192 	Sunflora 	Sunflora 	75 	75 	55 	105 	85 	30 	425 	70.83
0193 	Yanma 	Yanma 	65 	65 	45 	75 	45 	95 	390 	65
0194 	Wooper 	Wooper 	55 	45 	45 	25 	25 	15 	210 	35
0195 	Quagsire 	Quagsire 	95 	85 	85 	65 	65 	35 	430 	71.67
0196 	Espeon 	Espeon 	65 	65 	60 	130 	95 	110 	525 	87.5
0197 	Umbreon 	Umbreon 	95 	65 	110 	60 	130 	65 	525 	87.5
0198 	Murkrow 	Murkrow 	60 	85 	42 	85 	42 	91 	405 	67.5
0199 	Slowking 	Slowking 	95 	75 	80 	100 	110 	30 	490 	81.67
0200 	Misdreavus 	Misdreavus 	60 	60 	60 	85 	85 	85 	435 	72.5
0201 	Unown 	Unown 	48 	72 	48 	72 	48 	48 	336 	56
0202 	Wobbuffet 	Wobbuffet 	190 	33 	58 	33 	58 	33 	405 	67.5
0203 	Girafarig 	Girafarig 	70 	80 	65 	90 	65 	85 	455 	75.83
0204 	Pineco 	Pineco 	50 	65 	90 	35 	35 	15 	290 	48.33
0205 	Forretress 	Forretress 	75 	90 	140 	60 	60 	40 	465 	77.5
0206 	Dunsparce 	Dunsparce 	100 	70 	70 	65 	65 	45 	415 	69.17
0207 	Gligar 	Gligar 	65 	75 	105 	35 	65 	85 	430 	71.67
0208 	Steelix 	Steelix 	75 	85 	200 	55 	65 	30 	510 	85
0209 	Snubbull 	Snubbull 	60 	80 	50 	40 	40 	30 	300 	50
0210 	Granbull 	Granbull 	90 	120 	75 	60 	60 	45 	450 	75
0211 	Qwilfish 	Qwilfish 	65 	95 	75 	55 	55 	85 	430 	71.67
0212 	Scizor 	Scizor 	70 	130 	100 	55 	80 	65 	500 	83.33
0213 	Shuckle 	Shuckle 	20 	10 	230 	10 	230 	5 	505 	84.17
0214 	Heracross 	Heracross 	80 	125 	75 	40 	95 	85 	500 	83.33
0215 	Sneasel 	Sneasel 	55 	95 	55 	35 	75 	115 	430 	71.67
0216 	Teddiursa 	Teddiursa 	60 	80 	50 	50 	50 	40 	330 	55
0217 	Ursaring 	Ursaring 	90 	130 	75 	75 	75 	55 	500 	83.33
0218 	Slugma 	Slugma 	40 	40 	40 	70 	40 	20 	250 	41.67
0219 	Magcargo 	Magcargo 	50 	50 	120 	80 	80 	30 	410 	68.33
0220 	Swinub 	Swinub 	50 	50 	40 	30 	30 	50 	250 	41.67
0221 	Piloswine 	Piloswine 	100 	100 	80 	60 	60 	50 	450 	75
0222 	Corsola 	Corsola 	55 	55 	85 	65 	85 	35 	380 	63.33
0223 	Remoraid 	Remoraid 	35 	65 	35 	65 	35 	65 	300 	50
0224 	Octillery 	Octillery 	75 	105 	75 	105 	75 	45 	480 	80
0225 	Delibird 	Delibird 	45 	55 	45 	65 	45 	75 	330 	55
0226 	Mantine 	Mantine 	65 	40 	70 	80 	140 	70 	465 	77.5
0227 	Skarmory 	Skarmory 	65 	80 	140 	40 	70 	70 	465 	77.5
0228 	Houndour 	Houndour 	45 	60 	30 	80 	50 	65 	330 	55
0229 	Houndoom 	Houndoom 	75 	90 	50 	110 	80 	95 	500 	83.33
0230 	Kingdra 	Kingdra 	75 	95 	95 	95 	95 	85 	540 	90
0231 	Phanpy 	Phanpy 	90 	60 	60 	40 	40 	40 	330 	55
0232 	Donphan 	Donphan 	90 	120 	120 	60 	60 	50 	500 	83.33
0233 	Porygon2 	Porygon2 	85 	80 	90 	105 	95 	60 	515 	85.83
0234 	Stantler 	Stantler 	73 	95 	62 	85 	65 	85 	465 	77.5
0235 	Smeargle 	Smeargle 	55 	20 	35 	20 	45 	75 	250 	41.67
0236 	Tyrogue 	Tyrogue 	35 	35 	35 	35 	35 	35 	210 	35
0237 	Hitmontop 	Hitmontop 	50 	95 	95 	35 	110 	70 	455 	75.83
0238 	Smoochum 	Smoochum 	45 	30 	15 	85 	65 	65 	305 	50.83
0239 	Elekid 	Elekid 	45 	63 	37 	65 	55 	95 	360 	60
0240 	Magby 	Magby 	45 	75 	37 	70 	55 	83 	365 	60.83
0241 	Miltank 	Miltank 	95 	80 	105 	40 	70 	100 	490 	81.67
0242 	Blissey 	Blissey 	255 	10 	10 	75 	135 	55 	540 	90
0243 	Raikou 	Raikou 	90 	85 	75 	115 	100 	115 	580 	96.67
0244 	Entei 	Entei 	115 	115 	85 	90 	75 	100 	580 	96.67
0245 	Suicune 	Suicune 	100 	75 	115 	90 	115 	85 	580 	96.67
0246 	Larvitar 	Larvitar 	50 	64 	50 	45 	50 	41 	300 	50
0247 	Pupitar 	Pupitar 	70 	84 	70 	65 	70 	51 	410 	68.33
0248 	Tyranitar 	Tyranitar 	100 	134 	110 	95 	100 	61 	600 	100
0249 	Lugia 	Lugia 	106 	90 	130 	90 	154 	110 	680 	113.33
0250 	Ho-Oh 	Ho-Oh 	106 	130 	90 	110 	154 	90 	680 	113.33
0251 	Celebi 	Celebi 	100 	100 	100 	100 	100 	100 	600 	100
0252 	Treecko 	Treecko 	40 	45 	35 	65 	55 	70 	310 	51.67
0253 	Grovyle 	Grovyle 	50 	65 	45 	85 	65 	95 	405 	67.5
0254 	Sceptile 	Sceptile 	70 	85 	65 	105 	85 	120 	530 	88.33
0255 	Torchic 	Torchic 	45 	60 	40 	70 	50 	45 	310 	51.67
0256 	Combusken 	Combusken 	60 	85 	60 	85 	60 	55 	405 	67.5
0257 	Blaziken 	Blaziken 	80 	120 	70 	110 	70 	80 	530 	88.33
0258 	Mudkip 	Mudkip 	50 	70 	50 	50 	50 	40 	310 	51.67
0259 	Marshtomp 	Marshtomp 	70 	85 	70 	60 	70 	50 	405 	67.5
0260 	Swampert 	Swampert 	100 	110 	90 	85 	90 	60 	535 	89.17
0261 	Poochyena 	Poochyena 	35 	55 	35 	30 	30 	35 	220 	36.67
0262 	Mightyena 	Mightyena 	70 	90 	70 	60 	60 	70 	420 	70
0263 	Zigzagoon 	Zigzagoon 	38 	30 	41 	30 	41 	60 	240 	40
0264 	Linoone 	Linoone 	78 	70 	61 	50 	61 	100 	420 	70
0265 	Wurmple 	Wurmple 	45 	45 	35 	20 	30 	20 	195 	32.5
0266 	Silcoon 	Silcoon 	50 	35 	55 	25 	25 	15 	205 	34.17
0267 	Beautifly 	Beautifly 	60 	70 	50 	90 	50 	65 	385 	64.17
0268 	Cascoon 	Cascoon 	50 	35 	55 	25 	25 	15 	205 	34.17
0269 	Dustox 	Dustox 	60 	50 	70 	50 	90 	65 	385 	64.17
0270 	Lotad 	Lotad 	40 	30 	30 	40 	50 	30 	220 	36.67
0271 	Lombre 	Lombre 	60 	50 	50 	60 	70 	50 	340 	56.67
0272 	Ludicolo 	Ludicolo 	80 	70 	70 	90 	100 	70 	480 	80
0273 	Seedot 	Seedot 	40 	40 	50 	30 	30 	30 	220 	36.67
0274 	Nuzleaf 	Nuzleaf 	70 	70 	40 	60 	40 	60 	340 	56.67
0275 	Shiftry 	Shiftry 	90 	100 	60 	90 	60 	80 	480 	80
0276 	Taillow 	Taillow 	40 	55 	30 	30 	30 	85 	270 	45
0277 	Swellow 	Swellow 	60 	85 	60 	50 	50 	125 	430 	71.67
0278 	Wingull 	Wingull 	40 	30 	30 	55 	30 	85 	270 	45
0279 	Pelipper 	Pelipper 	60 	50 	100 	85 	70 	65 	430 	71.67
0280 	Ralts 	Ralts 	28 	25 	25 	45 	35 	40 	198 	33
0281 	Kirlia 	Kirlia 	38 	35 	35 	65 	55 	50 	278 	46.33
0282 	Gardevoir 	Gardevoir 	68 	65 	65 	125 	115 	80 	518 	86.33
0283 	Surskit 	Surskit 	40 	30 	32 	50 	52 	65 	269 	44.83
0284 	Masquerain 	Masquerain 	70 	60 	62 	80 	82 	60 	414 	69
0285 	Shroomish 	Shroomish 	60 	40 	60 	40 	60 	35 	295 	49.17
0286 	Breloom 	Breloom 	60 	130 	80 	60 	60 	70 	460 	76.67
0287 	Slakoth 	Slakoth 	60 	60 	60 	35 	35 	30 	280 	46.67
0288 	Vigoroth 	Vigoroth 	80 	80 	80 	55 	55 	90 	440 	73.33
0289 	Slaking 	Slaking 	150 	160 	100 	95 	65 	100 	670 	111.67
0290 	Nincada 	Nincada 	31 	45 	90 	30 	30 	40 	266 	44.33
0291 	Ninjask 	Ninjask 	61 	90 	45 	50 	50 	160 	456 	76
0292 	Shedinja 	Shedinja 	1 	90 	45 	30 	30 	40 	236 	39.33
0293 	Whismur 	Whismur 	64 	51 	23 	51 	23 	28 	240 	40
0294 	Loudred 	Loudred 	84 	71 	43 	71 	43 	48 	360 	60
0295 	Exploud 	Exploud 	104 	91 	63 	91 	63 	68 	480 	80
0296 	Makuhita 	Makuhita 	72 	60 	30 	20 	30 	25 	237 	39.5
0297 	Hariyama 	Hariyama 	144 	120 	60 	40 	60 	50 	474 	79
0298 	Azurill 	Azurill 	50 	20 	40 	20 	40 	20 	190 	31.67
0299 	Nosepass 	Nosepass 	30 	45 	135 	45 	90 	30 	375 	62.5
0300 	Skitty 	Skitty 	50 	45 	45 	35 	35 	50 	260 	43.33
0301 	Delcatty 	Delcatty 	70 	65 	65 	55 	55 	70 	380 	63.33
0302 	Sableye 	Sableye 	50 	75 	75 	65 	65 	50 	380 	63.33
0303 	Mawile 	Mawile 	50 	85 	85 	55 	55 	50 	380 	63.33
0304 	Aron 	Aron 	50 	70 	100 	40 	40 	30 	330 	55
0305 	Lairon 	Lairon 	60 	90 	140 	50 	50 	40 	430 	71.67
0306 	Aggron 	Aggron 	70 	110 	180 	60 	60 	50 	530 	88.33
0307 	Meditite 	Meditite 	30 	40 	55 	40 	55 	60 	280 	46.67
0308 	Medicham 	Medicham 	60 	60 	75 	60 	75 	80 	410 	68.33
0309 	Electrike 	Electrike 	40 	45 	40 	65 	40 	65 	295 	49.17
0310 	Manectric 	Manectric 	70 	75 	60 	105 	60 	105 	475 	79.17
0311 	Plusle 	Plusle 	60 	50 	40 	85 	75 	95 	405 	67.5
0312 	Minun 	Minun 	60 	40 	50 	75 	85 	95 	405 	67.5
0313 	Volbeat 	Volbeat 	65 	73 	55 	47 	75 	85 	400 	66.67
0314 	Illumise 	Illumise 	65 	47 	55 	73 	75 	85 	400 	66.67
0315 	Roselia 	Roselia 	50 	60 	45 	100 	80 	65 	400 	66.67
0316 	Gulpin 	Gulpin 	70 	43 	53 	43 	53 	40 	302 	50.33
0317 	Swalot 	Swalot 	100 	73 	83 	73 	83 	55 	467 	77.83
0318 	Carvanha 	Carvanha 	45 	90 	20 	65 	20 	65 	305 	50.83
0319 	Sharpedo 	Sharpedo 	70 	120 	40 	95 	40 	95 	460 	76.67
0320 	Wailmer 	Wailmer 	130 	70 	35 	70 	35 	60 	400 	66.67
0321 	Wailord 	Wailord 	170 	90 	45 	90 	45 	60 	500 	83.33
0322 	Numel 	Numel 	60 	60 	40 	65 	45 	35 	305 	50.83
0323 	Camerupt 	Camerupt 	70 	100 	70 	105 	75 	40 	460 	76.67
0324 	Torkoal 	Torkoal 	70 	85 	140 	85 	70 	20 	470 	78.33
0325 	Spoink 	Spoink 	60 	25 	35 	70 	80 	60 	330 	55
0326 	Grumpig 	Grumpig 	80 	45 	65 	90 	110 	80 	470 	78.33
0327 	Spinda 	Spinda 	60 	60 	60 	60 	60 	60 	360 	60
0328 	Trapinch 	Trapinch 	45 	100 	45 	45 	45 	10 	290 	48.33
0329 	Vibrava 	Vibrava 	50 	70 	50 	50 	50 	70 	340 	56.67
0330 	Flygon 	Flygon 	80 	100 	80 	80 	80 	100 	520 	86.67
0331 	Cacnea 	Cacnea 	50 	85 	40 	85 	40 	35 	335 	55.83
0332 	Cacturne 	Cacturne 	70 	115 	60 	115 	60 	55 	475 	79.17
0333 	Swablu 	Swablu 	45 	40 	60 	40 	75 	50 	310 	51.67
0334 	Altaria 	Altaria 	75 	70 	90 	70 	105 	80 	490 	81.67
0335 	Zangoose 	Zangoose 	73 	115 	60 	60 	60 	90 	458 	76.33
0336 	Seviper 	Seviper 	73 	100 	60 	100 	60 	65 	458 	76.33
0337 	Lunatone 	Lunatone 	70 	55 	65 	95 	85 	70 	440 	73.33
0338 	Solrock 	Solrock 	70 	95 	85 	55 	65 	70 	440 	73.33
0339 	Barboach 	Barboach 	50 	48 	43 	46 	41 	60 	288 	48
0340 	Whiscash 	Whiscash 	110 	78 	73 	76 	71 	60 	468 	78
0341 	Corphish 	Corphish 	43 	80 	65 	50 	35 	35 	308 	51.33
0342 	Crawdaunt 	Crawdaunt 	63 	120 	85 	90 	55 	55 	468 	78
0343 	Baltoy 	Baltoy 	40 	40 	55 	40 	70 	55 	300 	50
0344 	Claydol 	Claydol 	60 	70 	105 	70 	120 	75 	500 	83.33
0345 	Lileep 	Lileep 	66 	41 	77 	61 	87 	23 	355 	59.17
0346 	Cradily 	Cradily 	86 	81 	97 	81 	107 	43 	495 	82.5
0347 	Anorith 	Anorith 	45 	95 	50 	40 	50 	75 	355 	59.17
0348 	Armaldo 	Armaldo 	75 	125 	100 	70 	80 	45 	495 	82.5
0349 	Feebas 	Feebas 	20 	15 	20 	10 	55 	80 	200 	33.33
0350 	Milotic 	Milotic 	95 	60 	79 	100 	125 	81 	540 	90
0351 	Castform 	Castform 	70 	70 	70 	70 	70 	70 	420 	70
0352 	Kecleon 	Kecleon 	60 	90 	70 	60 	120 	40 	440 	73.33
0353 	Shuppet 	Shuppet 	44 	75 	35 	63 	33 	45 	295 	49.17
0354 	Banette 	Banette 	64 	115 	65 	83 	63 	65 	455 	75.83
0355 	Duskull 	Duskull 	20 	40 	90 	30 	90 	25 	295 	49.17
0356 	Dusclops 	Dusclops 	40 	70 	130 	60 	130 	25 	455 	75.83
0357 	Tropius 	Tropius 	99 	68 	83 	72 	87 	51 	460 	76.67
0358 	Chimecho 	Chimecho 	65 	50 	70 	95 	80 	65 	425 	70.83
0359 	Absol 	Absol 	65 	130 	60 	75 	60 	75 	465 	77.5
0360 	Wynaut 	Wynaut 	95 	23 	48 	23 	48 	23 	260 	43.33
0361 	Snorunt 	Snorunt 	50 	50 	50 	50 	50 	50 	300 	50
0362 	Glalie 	Glalie 	80 	80 	80 	80 	80 	80 	480 	80
0363 	Spheal 	Spheal 	70 	40 	50 	55 	50 	25 	290 	48.33
0364 	Sealeo 	Sealeo 	90 	60 	70 	75 	70 	45 	410 	68.33
0365 	Walrein 	Walrein 	110 	80 	90 	95 	90 	65 	530 	88.33
0366 	Clamperl 	Clamperl 	35 	64 	85 	74 	55 	32 	345 	57.5
0367 	Huntail 	Huntail 	55 	104 	105 	94 	75 	52 	485 	80.83
0368 	Gorebyss 	Gorebyss 	55 	84 	105 	114 	75 	52 	485 	80.83
0369 	Relicanth 	Relicanth 	100 	90 	130 	45 	65 	55 	485 	80.83
0370 	Luvdisc 	Luvdisc 	43 	30 	55 	40 	65 	97 	330 	55
0371 	Bagon 	Bagon 	45 	75 	60 	40 	30 	50 	300 	50
0372 	Shelgon 	Shelgon 	65 	95 	100 	60 	50 	50 	420 	70
0373 	Salamence 	Salamence 	95 	135 	80 	110 	80 	100 	600 	100
0374 	Beldum 	Beldum 	40 	55 	80 	35 	60 	30 	300 	50
0375 	Metang 	Metang 	60 	75 	100 	55 	80 	50 	420 	70
0376 	Metagross 	Metagross 	80 	135 	130 	95 	90 	70 	600 	100
0377 	Regirock 	Regirock 	80 	100 	200 	50 	100 	50 	580 	96.67
0378 	Regice 	Regice 	80 	50 	100 	100 	200 	50 	580 	96.67
0379 	Registeel 	Registeel 	80 	75 	150 	75 	150 	50 	580 	96.67
0380 	Latias 	Latias 	80 	80 	90 	110 	130 	110 	600 	100
0381 	Latios 	Latios 	80 	90 	80 	130 	110 	110 	600 	100
0382 	Kyogre 	Kyogre 	100 	100 	90 	150 	140 	90 	670 	111.67
0383 	Groudon 	Groudon 	100 	150 	140 	100 	90 	90 	670 	111.67
0384 	Rayquaza 	Rayquaza 	105 	150 	90 	150 	90 	95 	680 	113.33
0385 	Jirachi 	Jirachi 	100 	100 	100 	100 	100 	100 	600 	100
0386 	Deoxys 	Deoxys
Normal Forme 	50 	150 	50 	150 	50 	150 	600 	100
0386 	Deoxys 	Deoxys
Attack Forme 	50 	180 	20 	180 	20 	150 	600 	100
0386 	Deoxys 	Deoxys
Defense Forme 	50 	70 	160 	70 	160 	90 	600 	100
0386 	Deoxys 	Deoxys
Speed Forme 	50 	95 	90 	95 	90 	180 	600 	100
0387 	Turtwig 	Turtwig 	55 	68 	64 	45 	55 	31 	318 	53
0388 	Grotle 	Grotle 	75 	89 	85 	55 	65 	36 	405 	67.5
0389 	Torterra 	Torterra 	95 	109 	105 	75 	85 	56 	525 	87.5
0390 	Chimchar 	Chimchar 	44 	58 	44 	58 	44 	61 	309 	51.5
0391 	Monferno 	Monferno 	64 	78 	52 	78 	52 	81 	405 	67.5
0392 	Infernape 	Infernape 	76 	104 	71 	104 	71 	108 	534 	89
0393 	Piplup 	Piplup 	53 	51 	53 	61 	56 	40 	314 	52.33
0394 	Prinplup 	Prinplup 	64 	66 	68 	81 	76 	50 	405 	67.5
0395 	Empoleon 	Empoleon 	84 	86 	88 	111 	101 	60 	530 	88.33
0396 	Starly 	Starly 	40 	55 	30 	30 	30 	60 	245 	40.83
0397 	Staravia 	Staravia 	55 	75 	50 	40 	40 	80 	340 	56.67
0398 	Staraptor 	Staraptor 	85 	120 	70 	50 	50 	100 	475 	79.17
0399 	Bidoof 	Bidoof 	59 	45 	40 	35 	40 	31 	250 	41.67
0400 	Bibarel 	Bibarel 	79 	85 	60 	55 	60 	71 	410 	68.33
0401 	Kricketot 	Kricketot 	37 	25 	41 	25 	41 	25 	194 	32.33
0402 	Kricketune 	Kricketune 	77 	85 	51 	55 	51 	65 	384 	64
0403 	Shinx 	Shinx 	45 	65 	34 	40 	34 	45 	263 	43.83
0404 	Luxio 	Luxio 	60 	85 	49 	60 	49 	60 	363 	60.5
0405 	Luxray 	Luxray 	80 	120 	79 	95 	79 	70 	523 	87.17
0406 	Budew 	Budew 	40 	30 	35 	50 	70 	55 	280 	46.67
0407 	Roserade 	Roserade 	60 	70 	55 	125 	105 	90 	505 	84.17
0408 	Cranidos 	Cranidos 	67 	125 	40 	30 	30 	58 	350 	58.33
0409 	Rampardos 	Rampardos 	97 	165 	60 	65 	50 	58 	495 	82.5
0410 	Shieldon 	Shieldon 	30 	42 	118 	42 	88 	30 	350 	58.33
0411 	Bastiodon 	Bastiodon 	60 	52 	168 	47 	138 	30 	495 	82.5
0412 	Burmy 	Burmy 	40 	29 	45 	29 	45 	36 	224 	37.33
0413 	Wormadam 	Wormadam
Plant Cloak 	60 	59 	85 	79 	105 	36 	424 	70.67
0413 	Wormadam 	Wormadam
Sandy Cloak 	60 	79 	105 	59 	85 	36 	424 	70.67
0413 	Wormadam 	Wormadam
Trash Cloak 	60 	69 	95 	69 	95 	36 	424 	70.67
0414 	Mothim 	Mothim 	70 	94 	50 	94 	50 	66 	424 	70.67
0415 	Combee 	Combee 	30 	30 	42 	30 	42 	70 	244 	40.67
0416 	Vespiquen 	Vespiquen 	70 	80 	102 	80 	102 	40 	474 	79
0417 	Pachirisu 	Pachirisu 	60 	45 	70 	45 	90 	95 	405 	67.5
0418 	Buizel 	Buizel 	55 	65 	35 	60 	30 	85 	330 	55
0419 	Floatzel 	Floatzel 	85 	105 	55 	85 	50 	115 	495 	82.5
0420 	Cherubi 	Cherubi 	45 	35 	45 	62 	53 	35 	275 	45.83
0421 	Cherrim 	Cherrim 	70 	60 	70 	87 	78 	85 	450 	75
0422 	Shellos 	Shellos 	76 	48 	48 	57 	62 	34 	325 	54.17
0423 	Gastrodon 	Gastrodon 	111 	83 	68 	92 	82 	39 	475 	79.17
0424 	Ambipom 	Ambipom 	75 	100 	66 	60 	66 	115 	482 	80.33
0425 	Drifloon 	Drifloon 	90 	50 	34 	60 	44 	70 	348 	58
0426 	Drifblim 	Drifblim 	150 	80 	44 	90 	54 	80 	498 	83
0427 	Buneary 	Buneary 	55 	66 	44 	44 	56 	85 	350 	58.33
0428 	Lopunny 	Lopunny 	65 	76 	84 	54 	96 	105 	480 	80
0429 	Mismagius 	Mismagius 	60 	60 	60 	105 	105 	105 	495 	82.5
0430 	Honchkrow 	Honchkrow 	100 	125 	52 	105 	52 	71 	505 	84.17
0431 	Glameow 	Glameow 	49 	55 	42 	42 	37 	85 	310 	51.67
0432 	Purugly 	Purugly 	71 	82 	64 	64 	59 	112 	452 	75.33
0433 	Chingling 	Chingling 	45 	30 	50 	65 	50 	45 	285 	47.5
0434 	Stunky 	Stunky 	63 	63 	47 	41 	41 	74 	329 	54.83
0435 	Skuntank 	Skuntank 	103 	93 	67 	71 	61 	84 	479 	79.83
0436 	Bronzor 	Bronzor 	57 	24 	86 	24 	86 	23 	300 	50
0437 	Bronzong 	Bronzong 	67 	89 	116 	79 	116 	33 	500 	83.33
0438 	Bonsly 	Bonsly 	50 	80 	95 	10 	45 	10 	290 	48.33
0439 	Mime Jr. 	Mime Jr. 	20 	25 	45 	70 	90 	60 	310 	51.67
0440 	Happiny 	Happiny 	100 	5 	5 	15 	65 	30 	220 	36.67
0441 	Chatot 	Chatot 	76 	65 	45 	92 	42 	91 	411 	68.5
0442 	Spiritomb 	Spiritomb 	50 	92 	108 	92 	108 	35 	485 	80.83
0443 	Gible 	Gible 	58 	70 	45 	40 	45 	42 	300 	50
0444 	Gabite 	Gabite 	68 	90 	65 	50 	55 	82 	410 	68.33
0445 	Garchomp 	Garchomp 	108 	130 	95 	80 	85 	102 	600 	100
0446 	Munchlax 	Munchlax 	135 	85 	40 	40 	85 	5 	390 	65
0447 	Riolu 	Riolu 	40 	70 	40 	35 	40 	60 	285 	47.5
0448 	Lucario 	Lucario 	70 	110 	70 	115 	70 	90 	525 	87.5
0449 	Hippopotas 	Hippopotas 	68 	72 	78 	38 	42 	32 	330 	55
0450 	Hippowdon 	Hippowdon 	108 	112 	118 	68 	72 	47 	525 	87.5
0451 	Skorupi 	Skorupi 	40 	50 	90 	30 	55 	65 	330 	55
0452 	Drapion 	Drapion 	70 	90 	110 	60 	75 	95 	500 	83.33
0453 	Croagunk 	Croagunk 	48 	61 	40 	61 	40 	50 	300 	50
0454 	Toxicroak 	Toxicroak 	83 	106 	65 	86 	65 	85 	490 	81.67
0455 	Carnivine 	Carnivine 	74 	100 	72 	90 	72 	46 	454 	75.67
0456 	Finneon 	Finneon 	49 	49 	56 	49 	61 	66 	330 	55
0457 	Lumineon 	Lumineon 	69 	69 	76 	69 	86 	91 	460 	76.67
0458 	Mantyke 	Mantyke 	45 	20 	50 	60 	120 	50 	345 	57.5
0459 	Snover 	Snover 	60 	62 	50 	62 	60 	40 	334 	55.67
0460 	Abomasnow 	Abomasnow 	90 	92 	75 	92 	85 	60 	494 	82.33
0461 	Weavile 	Weavile 	70 	120 	65 	45 	85 	125 	510 	85
0462 	Magnezone 	Magnezone 	70 	70 	115 	130 	90 	60 	535 	89.17
0463 	Lickilicky 	Lickilicky 	110 	85 	95 	80 	95 	50 	515 	85.83
0464 	Rhyperior 	Rhyperior 	115 	140 	130 	55 	55 	40 	535 	89.17
0465 	Tangrowth 	Tangrowth 	100 	100 	125 	110 	50 	50 	535 	89.17
0466 	Electivire 	Electivire 	75 	123 	67 	95 	85 	95 	540 	90
0467 	Magmortar 	Magmortar 	75 	95 	67 	125 	95 	83 	540 	90
0468 	Togekiss 	Togekiss 	85 	50 	95 	120 	115 	80 	545 	90.83
0469 	Yanmega 	Yanmega 	86 	76 	86 	116 	56 	95 	515 	85.83
0470 	Leafeon 	Leafeon 	65 	110 	130 	60 	65 	95 	525 	87.5
0471 	Glaceon 	Glaceon 	65 	60 	110 	130 	95 	65 	525 	87.5
0472 	Gliscor 	Gliscor 	75 	95 	125 	45 	75 	95 	510 	85
0473 	Mamoswine 	Mamoswine 	110 	130 	80 	70 	60 	80 	530 	88.33
0474 	Porygon-Z 	Porygon-Z 	85 	80 	70 	135 	75 	90 	535 	89.17
0475 	Gallade 	Gallade 	68 	125 	65 	65 	115 	80 	518 	86.33
0476 	Probopass 	Probopass 	60 	55 	145 	75 	150 	40 	525 	87.5
0477 	Dusknoir 	Dusknoir 	45 	100 	135 	65 	135 	45 	525 	87.5
0478 	Froslass 	Froslass 	70 	80 	70 	80 	70 	110 	480 	80
0479 	Rotom 	Rotom 	50 	50 	77 	95 	77 	91 	440 	73.33
0479 	Rotom 	Rotom
Heat Rotom 	50 	65 	107 	105 	107 	86 	520 	86.67
0479 	Rotom 	Rotom
Wash Rotom 	50 	65 	107 	105 	107 	86 	520 	86.67
0479 	Rotom 	Rotom
Frost Rotom 	50 	65 	107 	105 	107 	86 	520 	86.67
0479 	Rotom 	Rotom
Fan Rotom 	50 	65 	107 	105 	107 	86 	520 	86.67
0479 	Rotom 	Rotom
Mow Rotom 	50 	65 	107 	105 	107 	86 	520 	86.67
0480 	Uxie 	Uxie 	75 	75 	130 	75 	130 	95 	580 	96.67
0481 	Mesprit 	Mesprit 	80 	105 	105 	105 	105 	80 	580 	96.67
0482 	Azelf 	Azelf 	75 	125 	70 	125 	70 	115 	580 	96.67
0483 	Dialga 	Dialga 	100 	120 	120 	150 	100 	90 	680 	113.33
0484 	Palkia 	Palkia 	90 	120 	100 	150 	120 	100 	680 	113.33
0485 	Heatran 	Heatran 	91 	90 	106 	130 	106 	77 	600 	100
0486 	Regigigas 	Regigigas 	110 	160 	110 	80 	110 	100 	670 	111.67
0487 	Giratina 	Giratina
Altered Forme 	150 	100 	120 	100 	120 	90 	680 	113.33
0487 	Giratina 	Giratina
Origin Forme 	150 	120 	100 	120 	100 	90 	680 	113.33
0488 	Cresselia 	Cresselia 	120 	70 	120 	75 	130 	85 	600 	100
0489 	Phione 	Phione 	80 	80 	80 	80 	80 	80 	480 	80
0490 	Manaphy 	Manaphy 	100 	100 	100 	100 	100 	100 	600 	100
0491 	Darkrai 	Darkrai 	70 	90 	90 	135 	90 	125 	600 	100
0492 	Shaymin 	Shaymin
Land Forme 	100 	100 	100 	100 	100 	100 	600 	100
0492 	Shaymin 	Shaymin
Sky Forme 	100 	103 	75 	120 	75 	127 	600 	100
0493 	Arceus 	Arceus 	120 	120 	120 	120 	120 	120 	720 	120
0494 	Victini 	Victini 	100 	100 	100 	100 	100 	100 	600 	100
0495 	Snivy 	Snivy 	45 	45 	55 	45 	55 	63 	308 	51.33
0496 	Servine 	Servine 	60 	60 	75 	60 	75 	83 	413 	68.83
0497 	Serperior 	Serperior 	75 	75 	95 	75 	95 	113 	528 	88
0498 	Tepig 	Tepig 	65 	63 	45 	45 	45 	45 	308 	51.33
0499 	Pignite 	Pignite 	90 	93 	55 	70 	55 	55 	418 	69.67
0500 	Emboar 	Emboar 	110 	123 	65 	100 	65 	65 	528 	88
0501 	Oshawott 	Oshawott 	55 	55 	45 	63 	45 	45 	308 	51.33
0502 	Dewott 	Dewott 	75 	75 	60 	83 	60 	60 	413 	68.83
0503 	Samurott 	Samurott 	95 	100 	85 	108 	70 	70 	528 	88
0504 	Patrat 	Patrat 	45 	55 	39 	35 	39 	42 	255 	42.5
0505 	Watchog 	Watchog 	60 	85 	69 	60 	69 	77 	420 	70
0506 	Lillipup 	Lillipup 	45 	60 	45 	25 	45 	55 	275 	45.83
0507 	Herdier 	Herdier 	65 	80 	65 	35 	65 	60 	370 	61.67
0508 	Stoutland 	Stoutland 	85 	100 	90 	45 	90 	80 	490 	81.67
0509 	Purrloin 	Purrloin 	41 	50 	37 	50 	37 	66 	281 	46.83
0510 	Liepard 	Liepard 	64 	88 	50 	88 	50 	106 	446 	74.33
0511 	Pansage 	Pansage 	50 	53 	48 	53 	48 	64 	316 	52.67
0512 	Simisage 	Simisage 	75 	98 	63 	98 	63 	101 	498 	83
0513 	Pansear 	Pansear 	50 	53 	48 	53 	48 	64 	316 	52.67
0514 	Simisear 	Simisear 	75 	98 	63 	98 	63 	101 	498 	83
0515 	Panpour 	Panpour 	50 	53 	48 	53 	48 	64 	316 	52.67
0516 	Simipour 	Simipour 	75 	98 	63 	98 	63 	101 	498 	83
0517 	Munna 	Munna 	76 	25 	45 	67 	55 	24 	292 	48.67
0518 	Musharna 	Musharna 	116 	55 	85 	107 	95 	29 	487 	81.17
0519 	Pidove 	Pidove 	50 	55 	50 	36 	30 	43 	264 	44
0520 	Tranquill 	Tranquill 	62 	77 	62 	50 	42 	65 	358 	59.67
0521 	Unfezant 	Unfezant 	80 	105 	80 	65 	55 	93 	478 	79.67
0522 	Blitzle 	Blitzle 	45 	60 	32 	50 	32 	76 	295 	49.17
0523 	Zebstrika 	Zebstrika 	75 	100 	63 	80 	63 	116 	497 	82.83
0524 	Roggenrola 	Roggenrola 	55 	75 	85 	25 	25 	15 	280 	46.67
0525 	Boldore 	Boldore 	70 	105 	105 	50 	40 	20 	390 	65
0526 	Gigalith 	Gigalith 	85 	135 	130 	60 	70 	25 	505 	84.17
0527 	Woobat 	Woobat 	55 	45 	43 	55 	43 	72 	313 	52.17
0528 	Swoobat 	Swoobat 	67 	57 	55 	77 	55 	114 	425 	70.83
0529 	Drilbur 	Drilbur 	60 	85 	40 	30 	45 	68 	328 	54.67
0530 	Excadrill 	Excadrill 	110 	135 	60 	50 	65 	88 	508 	84.67
0531 	Audino 	Audino 	103 	60 	86 	60 	86 	50 	445 	74.17
0532 	Timburr 	Timburr 	75 	80 	55 	25 	35 	35 	305 	50.83
0533 	Gurdurr 	Gurdurr 	85 	105 	85 	40 	50 	40 	405 	67.5
0534 	Conkeldurr 	Conkeldurr 	105 	140 	95 	55 	65 	45 	505 	84.17
0535 	Tympole 	Tympole 	50 	50 	40 	50 	40 	64 	294 	49
0536 	Palpitoad 	Palpitoad 	75 	65 	55 	65 	55 	69 	384 	64
0537 	Seismitoad 	Seismitoad 	105 	85 	75 	85 	75 	74 	499 	83.17
0538 	Throh 	Throh 	120 	100 	85 	30 	85 	45 	465 	77.5
0539 	Sawk 	Sawk 	75 	125 	75 	30 	75 	85 	465 	77.5
0540 	Sewaddle 	Sewaddle 	45 	53 	70 	40 	60 	42 	310 	51.67
0541 	Swadloon 	Swadloon 	55 	63 	90 	50 	80 	42 	380 	63.33
0542 	Leavanny 	Leavanny 	75 	103 	80 	70 	70 	92 	490 	81.67
0543 	Venipede 	Venipede 	30 	45 	59 	30 	39 	57 	260 	43.33
0544 	Whirlipede 	Whirlipede 	40 	55 	99 	40 	79 	47 	360 	60
0545 	Scolipede 	Scolipede 	60 	90 	89 	55 	69 	112 	475 	79.17
0546 	Cottonee 	Cottonee 	40 	27 	60 	37 	50 	66 	280 	46.67
0547 	Whimsicott 	Whimsicott 	60 	67 	85 	77 	75 	116 	480 	80
0548 	Petilil 	Petilil 	45 	35 	50 	70 	50 	30 	280 	46.67
0549 	Lilligant 	Lilligant 	70 	60 	75 	110 	75 	90 	480 	80
0550 	Basculin 	Basculin 	70 	92 	65 	80 	55 	98 	460 	76.67
0551 	Sandile 	Sandile 	50 	72 	35 	35 	35 	65 	292 	48.67
0552 	Krokorok 	Krokorok 	60 	82 	45 	45 	45 	74 	351 	58.5
0553 	Krookodile 	Krookodile 	95 	117 	70 	65 	70 	92 	509 	84.83
0554 	Darumaka 	Darumaka 	70 	90 	45 	15 	45 	50 	315 	52.5
0555 	Darmanitan 	Darmanitan
Standard Mode 	105 	140 	55 	30 	55 	95 	480 	80
0555 	Darmanitan 	Darmanitan
Zen Mode 	105 	30 	105 	140 	105 	55 	540 	90
0556 	Maractus 	Maractus 	75 	86 	67 	106 	67 	60 	461 	76.83
0557 	Dwebble 	Dwebble 	50 	65 	85 	35 	35 	55 	325 	54.17
0558 	Crustle 	Crustle 	70 	95 	125 	65 	75 	45 	475 	79.17
0559 	Scraggy 	Scraggy 	50 	75 	70 	35 	70 	48 	348 	58
0560 	Scrafty 	Scrafty 	65 	90 	115 	45 	115 	58 	488 	81.33
0561 	Sigilyph 	Sigilyph 	72 	58 	80 	103 	80 	97 	490 	81.67
0562 	Yamask 	Yamask 	38 	30 	85 	55 	65 	30 	303 	50.5
0563 	Cofagrigus 	Cofagrigus 	58 	50 	145 	95 	105 	30 	483 	80.5
0564 	Tirtouga 	Tirtouga 	54 	78 	103 	53 	45 	22 	355 	59.17
0565 	Carracosta 	Carracosta 	74 	108 	133 	83 	65 	32 	495 	82.5
0566 	Archen 	Archen 	55 	112 	45 	74 	45 	70 	401 	66.83
0567 	Archeops 	Archeops 	75 	140 	65 	112 	65 	110 	567 	94.5
0568 	Trubbish 	Trubbish 	50 	50 	62 	40 	62 	65 	329 	54.83
0569 	Garbodor 	Garbodor 	80 	95 	82 	60 	82 	75 	474 	79
0570 	Zorua 	Zorua 	40 	65 	40 	80 	40 	65 	330 	55
0571 	Zoroark 	Zoroark 	60 	105 	60 	120 	60 	105 	510 	85
0572 	Minccino 	Minccino 	55 	50 	40 	40 	40 	75 	300 	50
0573 	Cinccino 	Cinccino 	75 	95 	60 	65 	60 	115 	470 	78.33
0574 	Gothita 	Gothita 	45 	30 	50 	55 	65 	45 	290 	48.33
0575 	Gothorita 	Gothorita 	60 	45 	70 	75 	85 	55 	390 	65
0576 	Gothitelle 	Gothitelle 	70 	55 	95 	95 	110 	65 	490 	81.67
0577 	Solosis 	Solosis 	45 	30 	40 	105 	50 	20 	290 	48.33
0578 	Duosion 	Duosion 	65 	40 	50 	125 	60 	30 	370 	61.67
0579 	Reuniclus 	Reuniclus 	110 	65 	75 	125 	85 	30 	490 	81.67
0580 	Ducklett 	Ducklett 	62 	44 	50 	44 	50 	55 	305 	50.83
0581 	Swanna 	Swanna 	75 	87 	63 	87 	63 	98 	473 	78.83
0582 	Vanillite 	Vanillite 	36 	50 	50 	65 	60 	44 	305 	50.83
0583 	Vanillish 	Vanillish 	51 	65 	65 	80 	75 	59 	395 	65.83
0584 	Vanilluxe 	Vanilluxe 	71 	95 	85 	110 	95 	79 	535 	89.17
0585 	Deerling 	Deerling 	60 	60 	50 	40 	50 	75 	335 	55.83
0586 	Sawsbuck 	Sawsbuck 	80 	100 	70 	60 	70 	95 	475 	79.17
0587 	Emolga 	Emolga 	55 	75 	60 	75 	60 	103 	428 	71.33
0588 	Karrablast 	Karrablast 	50 	75 	45 	40 	45 	60 	315 	52.5
0589 	Escavalier 	Escavalier 	70 	135 	105 	60 	105 	20 	495 	82.5
0590 	Foongus 	Foongus 	69 	55 	45 	55 	55 	15 	294 	49
0591 	Amoonguss 	Amoonguss 	114 	85 	70 	85 	80 	30 	464 	77.33
0592 	Frillish 	Frillish 	55 	40 	50 	65 	85 	40 	335 	55.83
0593 	Jellicent 	Jellicent 	100 	60 	70 	85 	105 	60 	480 	80
0594 	Alomomola 	Alomomola 	165 	75 	80 	40 	45 	65 	470 	78.33
0595 	Joltik 	Joltik 	50 	47 	50 	57 	50 	65 	319 	53.17
0596 	Galvantula 	Galvantula 	70 	77 	60 	97 	60 	108 	472 	78.67
0597 	Ferroseed 	Ferroseed 	44 	50 	91 	24 	86 	10 	305 	50.83
0598 	Ferrothorn 	Ferrothorn 	74 	94 	131 	54 	116 	20 	489 	81.5
0599 	Klink 	Klink 	40 	55 	70 	45 	60 	30 	300 	50
0600 	Klang 	Klang 	60 	80 	95 	70 	85 	50 	440 	73.33
0601 	Klinklang 	Klinklang 	60 	100 	115 	70 	85 	90 	520 	86.67
0602 	Tynamo 	Tynamo 	35 	55 	40 	45 	40 	60 	275 	45.83
0603 	Eelektrik 	Eelektrik 	65 	85 	70 	75 	70 	40 	405 	67.5
0604 	Eelektross 	Eelektross 	85 	115 	80 	105 	80 	50 	515 	85.83
0605 	Elgyem 	Elgyem 	55 	55 	55 	85 	55 	30 	335 	55.83
0606 	Beheeyem 	Beheeyem 	75 	75 	75 	125 	95 	40 	485 	80.83
0607 	Litwick 	Litwick 	50 	30 	55 	65 	55 	20 	275 	45.83
0608 	Lampent 	Lampent 	60 	40 	60 	95 	60 	55 	370 	61.67
0609 	Chandelure 	Chandelure 	60 	55 	90 	145 	90 	80 	520 	86.67
0610 	Axew 	Axew 	46 	87 	60 	30 	40 	57 	320 	53.33
0611 	Fraxure 	Fraxure 	66 	117 	70 	40 	50 	67 	410 	68.33
0612 	Haxorus 	Haxorus 	76 	147 	90 	60 	70 	97 	540 	90
0613 	Cubchoo 	Cubchoo 	55 	70 	40 	60 	40 	40 	305 	50.83
0614 	Beartic 	Beartic 	95 	110 	80 	70 	80 	50 	485 	80.83
0615 	Cryogonal 	Cryogonal 	70 	50 	30 	95 	135 	105 	485 	80.83
0616 	Shelmet 	Shelmet 	50 	40 	85 	40 	65 	25 	305 	50.83
0617 	Accelgor 	Accelgor 	80 	70 	40 	100 	60 	145 	495 	82.5
0618 	Stunfisk 	Stunfisk 	109 	66 	84 	81 	99 	32 	471 	78.5
0619 	Mienfoo 	Mienfoo 	45 	85 	50 	55 	50 	65 	350 	58.33
0620 	Mienshao 	Mienshao 	65 	125 	60 	95 	60 	105 	510 	85
0621 	Druddigon 	Druddigon 	77 	120 	90 	60 	90 	48 	485 	80.83
0622 	Golett 	Golett 	59 	74 	50 	35 	50 	35 	303 	50.5
0623 	Golurk 	Golurk 	89 	124 	80 	55 	80 	55 	483 	80.5
0624 	Pawniard 	Pawniard 	45 	85 	70 	40 	40 	60 	340 	56.67
0625 	Bisharp 	Bisharp 	65 	125 	100 	60 	70 	70 	490 	81.67
0626 	Bouffalant 	Bouffalant 	95 	110 	95 	40 	95 	55 	490 	81.67
0627 	Rufflet 	Rufflet 	70 	83 	50 	37 	50 	60 	350 	58.33
0628 	Braviary 	Braviary 	100 	123 	75 	57 	75 	80 	510 	85
0629 	Vullaby 	Vullaby 	70 	55 	75 	45 	65 	60 	370 	61.67
0630 	Mandibuzz 	Mandibuzz 	110 	65 	105 	55 	95 	80 	510 	85
0631 	Heatmor 	Heatmor 	85 	97 	66 	105 	66 	65 	484 	80.67
0632 	Durant 	Durant 	58 	109 	112 	48 	48 	109 	484 	80.67
0633 	Deino 	Deino 	52 	65 	50 	45 	50 	38 	300 	50
0634 	Zweilous 	Zweilous 	72 	85 	70 	65 	70 	58 	420 	70
0635 	Hydreigon 	Hydreigon 	92 	105 	90 	125 	90 	98 	600 	100
0636 	Larvesta 	Larvesta 	55 	85 	55 	50 	55 	60 	360 	60
0637 	Volcarona 	Volcarona 	85 	60 	65 	135 	105 	100 	550 	91.67
0638 	Cobalion 	Cobalion 	91 	90 	129 	90 	72 	108 	580 	96.67
0639 	Terrakion 	Terrakion 	91 	129 	90 	72 	90 	108 	580 	96.67
0640 	Virizion 	Virizion 	91 	90 	72 	90 	129 	108 	580 	96.67
0641 	Tornadus 	Tornadus
Incarnate Forme 	79 	115 	70 	125 	80 	111 	580 	96.67
0641 	Tornadus 	Tornadus
Therian Forme 	79 	100 	80 	110 	90 	121 	580 	96.67
0642 	Thundurus 	Thundurus
Incarnate Forme 	79 	115 	70 	125 	80 	111 	580 	96.67
0642 	Thundurus 	Thundurus
Therian Forme 	79 	105 	70 	145 	80 	101 	580 	96.67
0643 	Reshiram 	Reshiram 	100 	120 	100 	150 	120 	90 	680 	113.33
0644 	Zekrom 	Zekrom 	100 	150 	120 	120 	100 	90 	680 	113.33
0645 	Landorus 	Landorus
Incarnate Forme 	89 	125 	90 	115 	80 	101 	600 	100
0645 	Landorus 	Landorus
Therian Forme 	89 	145 	90 	105 	80 	91 	600 	100
0646 	Kyurem 	Kyurem 	125 	130 	90 	130 	90 	95 	660 	110
0646 	Kyurem 	Kyurem
Black Kyurem 	125 	170 	100 	120 	90 	95 	700 	116.67
0646 	Kyurem 	Kyurem
White Kyurem 	125 	120 	90 	170 	100 	95 	700 	116.67
0647 	Keldeo 	Keldeo 	91 	72 	90 	129 	90 	108 	580 	96.67
0648 	Meloetta 	Meloetta
Aria Forme 	100 	77 	77 	128 	128 	90 	600 	100
0648 	Meloetta 	Meloetta
Pirouette Forme 	100 	128 	90 	77 	77 	128 	600 	100
0649 	Genesect 	Genesect 	71 	120 	95 	120 	95 	99 	600 	100

*/