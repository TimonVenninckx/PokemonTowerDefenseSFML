#include "WaveManager.h"
#include "ui/GameUI.h"
#include "SpriteLoader.h"
#include "Level.h"

WaveManager::WaveManager(int levelId)
{
	m_waveText.setFont(GameUI::font);
	m_waveText.setCharacterSize(50);
	m_waveText.setString("Wave 1/7");
	m_waveText.setOutlineColor(sf::Color::Black);
	m_waveText.setOutlineThickness(3);

	m_waveText.setOrigin(m_waveText.getGlobalBounds().getSize());
	m_waveText.setPosition(1900.f, 800.f);

	m_levelId = levelId;
	m_currentWave = 0;
	m_maxWave = 7;
}

bool WaveManager::won()
{
	if (!m_canLose)
		return true;
	else if (m_pickupsHead->next == nullptr)
		return false;
	else
		return true;
}

void WaveManager::init(LinkedList<Pokemon>* enemyHead, LinkedList<Pokemon>* playerHead, LinkedList<PickupItem>* pickupsHead, Level* level, sf::Vector2f startPosition,int waves)
{
	m_enemysHead = enemyHead;
	m_playerHead = playerHead;
	m_pickupsHead = pickupsHead;
	m_levelRef = level;
	m_startPosition = startPosition;
	m_maxWave = waves;
	m_waveText.setString("Wave 1/" + std::to_string(waves));


	std::vector<PickupStats> pickupPositions = Database::getPickups(m_levelId);
	for (PickupStats stats : pickupPositions) {
		PickupItem* item = new PickupItem(stats.texture, stats.location, m_enemysHead);
		m_pickupsHead->add(item);
	}
	m_canLose = !pickupPositions.empty();
}

bool WaveManager::update(float delta)
{
	if (m_canLose && m_pickupsHead->next == nullptr) {
		std::cout << "YOU LOST\n";
		return false;
	}

	if(m_wave.prefabs.empty()) {
		if (m_currentWave == m_maxWave) // if next is not valid and we have no more incoming waves stop
			return m_enemysHead->next;
		m_currentWave++;
		m_waveText.setString("Wave " + std::to_string(m_currentWave) + '/' + std::to_string(m_maxWave));
		m_wave =  Database::getWave(m_levelId, m_currentWave);
	}
	else {
		for (int i{ 0 }; i < m_wave.prefabs.size();i++) {
			//std::cout << delta << " cool delta and " << it->initialSpawnTimer << " spawn timer\n";
			m_wave.prefabs[i].initialSpawnTimer -= delta;
			if (m_wave.prefabs[i].initialSpawnTimer <= 0.f) {
				m_wave.prefabs[i].amount--;
				Pokemon* pokemon = new Pokemon{ &Database::baseStats[m_wave.prefabs[i].id], m_wave.prefabs[i].level, SpriteLoader::getPokemonSprite(m_wave.prefabs[i].id), m_wave.prefabs[i].attacks};
				pokemon->setPosition(m_startPosition);
				pokemon->setEnemies(m_playerHead);
				pokemon->setLevelReference(m_levelRef);
				pokemon->setCanCatch(m_wave.prefabs[i].canCatch);
				m_enemysHead->add(pokemon);
				
				m_wave.prefabs[i].initialSpawnTimer = m_wave.prefabs[i].timeBetweenSpawn;

				if (m_wave.prefabs[i].amount == 0) {
					std::vector<Wave::Prefab>::iterator prefab = m_wave.prefabs.begin() + i;
					m_wave.prefabs.erase(prefab);
					i--;
				}
			}
		}
	}
	return true;
}

void WaveManager::draw(sf::RenderWindow& window)
{
	window.draw(m_waveText);
}
