#ifndef POKEMON_H
#define POKEMON_H
#include <SFML/Graphics.hpp>
#include <queue>
#include "ui/HealthBar.h"
#include "attack/AllAttacks.h"
#include "PokemonStats.h"
#include "PickupItem.h"
#include "StatusEffect.h"
#include <array>


class Level;

class Pokemon {
protected:
	sf::Texture* m_texture{};
	sf::Sprite m_sprite{};
	HealthBar m_healthBar{};
	
	sf::Vector2f m_position{};

	PokemonStats* m_baseStats;
	PokemonStats stats{};

	float m_range{};
	
	float m_spriteChangeTime{};
	float m_timeSinceLastSpriteChange{m_spriteChangeTime};
	int   m_currentSprite{ 0 };
	int   m_spriteSize{};

	//0 = down
	//1 = left
	//2 = right
	//3 = up
	int   m_currentSpriteDirection{0};

	bool m_moving{ true };
	bool m_canAttack{ false };

	float m_attackCooldown{};
	AttackStats* m_currentAttack;
	std::array<AttackStats*, 4> m_attacks;

	bool m_canLevelUp{ false };
	std::vector<Attack*> m_activeAttacks;
	LinkedList<Pokemon>* m_enemiesHead{ nullptr };
	std::vector<Pokemon*> m_hitByPokemon;

	Level* m_levelReference{ nullptr };

	bool m_canCatch{ true };
	bool m_walkingForward{ true };
	int  m_currentWayPoint{};
	std::vector<sf::Vector2f>* m_path;

	PickupItem* m_pickupItem{ nullptr };
	StatusEffect m_statusEffect;


	bool m_whirldWinded{ false };
	void whirlwind();

	void changeStatusEffect(StatusEffect::EffectType effect);

	void updateSprite();
	void updateSprite(sf::Vector2f location);

	float calculateStat(float stat);
	void  reCalculateStats();

public:

	Pokemon(PokemonStats* baseStats, int level, sf::Texture* texture, std::array<AttackStats*, 4>& attacks);

	virtual ~Pokemon();


	virtual void update(float delta);
	virtual void draw(sf::RenderTarget& window);

	void checkIfTargetDead(Pokemon* pokemon);
	bool removeAttack(Attack* attack);
	virtual bool damage(Pokemon* pokemon, AttackStats* attack, float pokemonAtkStat, int pokemonLevel);
	void statusDamage(float damage);
	bool canStartAttack();
	virtual void attack(float delta);
	void deleteAllAttacks();

	float getRange();
	PokemonStats* getBaseStats();
	PokemonStats& getStats();
	float getHealth();
	bool  canCatch();
	void  setCanCatch(bool canCatch);
	sf::Vector2f getPosition();

	virtual void setPosition(sf::Vector2f position);
	virtual bool playerOwned();
	void setLevelReference(Level* level);
	sf::Texture* getTexture();
	std::string& getName();

	std::array<AttackStats*, 4>* getAttacks();
	void setCurrentAttack(AttackStats* attack);
	AttackStats* getCurrentAttack();
	bool addAttack(AttackStats* attack);
	void updateAttacks(std::array<AttackStats*, 4>& attacks);


	virtual void giveExp(float experience);
	virtual bool levelUp();
	virtual void evolve(PokemonStats* baseStats, sf::Texture* texture);
	bool canLevelUp();

	PickupItem* getPickupItem();
	bool canPickupItem();
	void setPickupItem(PickupItem* item);

	void setEnemies(LinkedList<Pokemon>* enemies);

	sf::FloatRect getFloatRect();
};






#endif