#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include "sqlite/Database.h"
#include "SFML/Graphics.hpp"
#include "LinkedList.h"
#include "Pokemon.h"
#include "PickupItem.h"

class WaveManager
{
	Wave m_wave;
	sf::Text m_waveText;
	int m_levelId;
	int m_currentWave;
	int m_maxWave;
	bool m_canLose{ false };

	Level* m_levelRef;
	LinkedList<Pokemon>* m_enemysHead{};
	LinkedList<Pokemon>* m_playerHead{};
	LinkedList<PickupItem>* m_pickupsHead{};
	sf::Vector2f m_startPosition;
public:
	WaveManager(int levelId);

	bool won();
	void init(LinkedList<Pokemon>* enemyHead, LinkedList<Pokemon>* playerHead,LinkedList<PickupItem>* pickupsHead, Level* level, sf::Vector2f startPosition,int waves);
	bool update(float delta);
	void draw(sf::RenderWindow& window);
};

#endif