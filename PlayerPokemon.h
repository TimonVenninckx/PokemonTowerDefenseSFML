#ifndef PLAYERPOKEMON_H
#define PLAYERPOKEMON_H

#include "Pokemon.h"
#include "ui/PokemonPlaceUI.h"
#include "LinkedList.h"
class EnemyPokemon;
class PokemonPlacement;

class PlayerPokemon : public Pokemon
{
private:
	bool m_active{false};
	sf::CircleShape m_rangeCircle;

	PokemonPlacement* m_placeholder{ nullptr };

	PokemonPlaceUI* m_ui{ nullptr };

public:
	PlayerPokemon(PokemonStats* baseStats,int level, sf::Texture* texture, std::array<AttackStats*, 4>& attacks);
	PlayerPokemon(Pokemon* pokemon);

	void reset();
	void update(float delta) override;
	void draw(sf::RenderTarget& window) override;
	void drawRange(sf::RenderTarget& window);


	bool damage(Pokemon* pokemon, AttackStats* attack, float pokemonAtkStat, int pokemonLevel) override;
	float getHealth();

	virtual void setPosition(sf::Vector2f position) override;

	bool contains(sf::Vector2f position);


	PokemonPlacement* getPlacement();
	void setPlacement(PokemonPlacement*);
	void setGameUI(PokemonPlaceUI& ui);

	void setActive(bool active);
	bool isActive();

	bool playerOwned() override;
	void giveExp(float experience) override;
	bool levelUp() override;
	int getLevelUpCost();

	void evolve(PokemonStats* baseStats, sf::Texture* texture) override;
};


#endif // PLAYERPOKEMON_H