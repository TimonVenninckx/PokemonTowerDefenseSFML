#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include "SFML/Audio.hpp"
#include "Pokemon.h"
#include "PlayerPokemon.h"
#include "Player.h"
#include "ui/PokemonPlacement.h"
#include "attack/Attack.h"
#include "LinkedList.h"
#include "ui/GameSpeedUI.h"
#include "ui/LevelPokemonUI.h"
#include "ui/PauseMenuUI.h"
#include "ui/PokemonCaughtUI.h"
#include "ui/PokeBallUI.h"
#include "ui/GameOverUI.h"
#include "WaveManager.h"
#include "PickupItem.h"
#include "ui/DamageText.h"
#include "ui/TextBoxManager.h"
#include "Character.h"

#include "sqlite/Database.h"
class PlayerPokemon;
class EnemyPokemon;

struct LevelInfo {
	int id{};
	std::string fileLocation;
	std::string thumbnailLocation;
	std::string name;
	std::vector<sf::Vector2f> path;
	std::vector<sf::Vector2f> placementLocations;
	std::vector<SpriteAndPosition> characters;
	int waves{};
	std::string winText;
	std::string loseText;
	bool deadEnd{};
	std::string songPath;

	std::string winImage;
	std::string loseImage;
};




class Level
{
public:
	Level(sf::RenderWindow& renderwindow, Player& playerref, LevelInfo info,sf::View& view);
	~Level();

	bool updateEvents(sf::Event& event);
	void run(float delta);

	void playerPokemonDied(Pokemon* pokemon);
	void enemyPokemonDied(Pokemon* pokemon);

	bool reachedEnd(Pokemon* pokemon);
	bool isDeadEnd();
	void spawnDamageText(sf::Vector2f location, sf::Vector2f direction, int damage);
	void spawnDamageText(sf::Vector2f location,sf::Vector2f direction, std::string text);
	std::vector<sf::Vector2f>* getPath();

private:
	void render();
	void renderUI();
	void update(float delta);
	
	void updateMouse(float delta);

	void keyPressed(sf::Event& event);

	void tryPlacePokemon();
	bool tryPickupPokemon();

private:
	LevelInfo levelInfo;
	sf::RenderWindow& window;
	float m_zoom{ 1.f };
	sf::View view{};
	sf::View& uiView;
	sf::Texture tileMap{};
	sf::Sprite sprite{};
	sf::Text  m_playerMoney{};

	sf::Texture pauseTexture{};
	sf::Sprite pauseButton{};

	sf::Texture m_cantPlaceTexture;
	sf::Sprite m_cantPlaceSprite;



	sf::Texture m_pokemonPositionTexture;
	std::vector<PokemonPlacement> m_pokemonPositions;
	std::vector<DamageText> m_damageTexts;

	bool gameEnded{ false };
	bool mousePanning{ false };
	float cameraSpeed{ 500.f };
	sf::Vector2f mouseWorldPosition;

	PlayerPokemon* m_viewingPokemon{ nullptr };
	PlayerPokemon* m_draggingPokemon{ nullptr };

	LinkedList<Pokemon>* activeEnemiesHead{};
	LinkedList<Pokemon>* activePlayerPokemonHead{}; // cast it to PlayerPokemon everytime :(
	LinkedList<PickupItem>* pickupItemsHead{};
	std::vector<Attack*>* pokemonAttacks{};
	GameSpeedUI* currentGameSpeed{};

	std::vector<sf::Vector2f> path;
	std::vector<PokemonPlaceUI> activePokemonUI;
	std::vector<GameSpeedUI> gameSpeeds;
	std::vector<Character*>   characters;

	Player& player;

	WaveManager waveManager;
	PokeBallUI pokeballUI{};
	PauseMenuUI pauseMenu{};
	GameOverUI gameOverMenu{};
	LevelPokemonUI pokemonInfoUI;
	PokemonCaughtUI pokemonCaughtUI;
	TextBoxManager textBoxManager;


};

#endif
