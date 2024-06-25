#include "Pokemon.h" 
#include "GameMath.h"
#include "Level.h"
#include <iostream>
#include "Random.h"

static constexpr float basemoveSpeed = 60.f;


Pokemon::Pokemon(PokemonStats* baseStats,int level, sf::Texture* texture, std::array<AttackStats*, 4>& attacks)
	: m_baseStats{ baseStats }
	, m_statusEffect{ *this }
{

	stats = *baseStats;
	stats.level = level;
	stats.experience = 5.f * (level * level * level) / 4.f;

	reCalculateStats();
	//std::cout << stats.name << " has " << stats.health << " health\n";

	m_range = 150.f;
	m_spriteSize = texture->getSize().x / 4;
	m_texture = texture;
	m_sprite.setTexture(*m_texture);
	m_sprite.setTextureRect(sf::IntRect{ 0,0,m_spriteSize, m_spriteSize});
	m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);

	m_spriteChangeTime = .3f;
	m_timeSinceLastSpriteChange = m_spriteChangeTime;

	// init attacks
	m_attacks = attacks;

	if (!m_attacks[0])
		std::cout << "POKEMON FIRST MOVE IS NOT VALID ERROR INCOMING";
	m_currentAttack = m_attacks[0];
	m_attackCooldown = m_currentAttack->attackCooldown;

}

void Pokemon::changeStatusEffect(StatusEffect::EffectType effect)
{
	if (effect == StatusEffect::Freeze && (stats.type1 == PokemonStats::ICE || stats.type2 == PokemonStats::ICE)) {
		return;
	}
	m_statusEffect.changeEffect(effect);
	m_healthBar.changeStatusEffect(effect);
	switch (effect) {
	case StatusEffect::Burn:
		m_sprite.setColor(sf::Color::Red); break;
	case StatusEffect::Paralysis:
		m_sprite.setColor(sf::Color{ 255,255,0,255 }); break;
	case StatusEffect::Freeze:
		m_sprite.setColor(sf::Color::Blue); break;
	case StatusEffect::Poison:
		m_sprite.setColor(sf::Color::Magenta); break;
	default:
		m_sprite.setColor(sf::Color::White); break;
	}
}

void Pokemon::whirlwind()
{
	if (m_whirldWinded)
		return;
	m_whirldWinded = true;
	if (m_walkingForward) {
		m_walkingForward = false;
		m_currentWayPoint--;
	}
	else { // if walking backwards
		m_walkingForward = true;
		m_currentWayPoint++;
	}
}

Pokemon::~Pokemon() {
	deleteAllAttacks();
	if (m_pickupItem) {
		std::cout << "pokemon.cpp drop item\n";
		m_pickupItem->drop();
	}
}


void Pokemon::updateSprite()
{
	m_timeSinceLastSpriteChange = m_spriteChangeTime;
	if (m_currentSprite == 3)
		m_currentSprite = 0;
	else
		++m_currentSprite;

	m_sprite.setTextureRect(sf::IntRect{ m_spriteSize * m_currentSprite, m_spriteSize * m_currentSpriteDirection ,m_spriteSize,m_spriteSize });
}

void Pokemon::updateSprite(sf::Vector2f direction)
{
	if (abs(direction.x) >= abs(direction.y)) {
		if (direction.x >= 0)
			m_currentSpriteDirection = 2;
		else
			m_currentSpriteDirection = 1;
	}
	else {
		if (direction.y >= 0)
			m_currentSpriteDirection = 0;
		else
			m_currentSpriteDirection = 3;
	}
	m_sprite.setTextureRect(sf::IntRect{ m_spriteSize * m_currentSprite, m_spriteSize * m_currentSpriteDirection ,m_spriteSize,m_spriteSize });
}



void Pokemon::update(float delta)
{
	// no negative delta
	if (delta < 0)
		return;

	if (stats.health <= 0)
		return;
	
	m_timeSinceLastSpriteChange -= delta;
	if (m_timeSinceLastSpriteChange <= 0.f) {
		updateSprite();
	}

	if (m_canAttack) {
		m_attackCooldown -= delta;
		attack(delta);
	}


	// movement
	if (m_moving && m_path) {
		if (float extraDistance = GameMath::interpolate(m_position, (*m_path)[m_currentWayPoint], (basemoveSpeed + stats.speed / 2.f) * delta * m_statusEffect.getMoveSpeedMultiplier()).leftOverStepSize > 0.f) {
			m_position = (*m_path)[m_currentWayPoint];
			if (m_walkingForward) {
				m_currentWayPoint++;
				if (m_currentWayPoint == m_path->size()) {
					m_currentWayPoint--;
					if (!m_levelReference->isDeadEnd()) {
						if(m_levelReference->reachedEnd(this))
							return;
					}
					m_walkingForward = false;
					m_currentWayPoint--; // again -- because we incremented before
				}
			}
			else { // if walking backwards
				m_currentWayPoint--;
				if (m_currentWayPoint == -1) {// if we are beyond the end
					m_currentWayPoint++; // make it 0 again to dodge errors aswell;
					if (m_levelReference->isDeadEnd()) {
						if (m_levelReference->reachedEnd(this))
							return;
					}
					m_walkingForward = true;
					m_currentWayPoint++; // again -- because we incremented before
				}
			}
			m_position = GameMath::interpolate(m_position, (*m_path)[m_currentWayPoint], extraDistance).position;
		}
		else {
			// IF WE WAIT LONG TO DRAW A FRAME MIGHT GET PAST
			// WAYPOINT POSITION DO SOMETHING WITH LEFT OVER FLOAT FROM INTERPOLATE
			m_position = GameMath::interpolate(m_position, (*m_path)[m_currentWayPoint], (basemoveSpeed + stats.speed / 2.f) * delta * m_statusEffect.getMoveSpeedMultiplier()).position;
			setPosition(m_position);
			//std::cout << m_position.x <<  ',' << m_position.y << '\n';
		}
		updateSprite((*m_path)[m_currentWayPoint] - m_position);
	}
	m_statusEffect.update(delta, m_sprite.getPosition());
}



void Pokemon::checkIfTargetDead(Pokemon* pokemon)
{
	for (int i{ 0 }; i < m_activeAttacks.size(); ++i) {
		if (m_activeAttacks[i]->removeTarget(pokemon)) {

			if(removeAttack(m_activeAttacks[i]))
				--i; // because vector resized;
		}
	}
}

bool Pokemon::removeAttack(Attack* attack) {
	std::vector<Attack*>::iterator it = std::find(m_activeAttacks.begin(), m_activeAttacks.end(), attack);
	if (it != m_activeAttacks.end()) {
		m_activeAttacks.erase(it);
		delete attack;
		return true;
	}
	std::cout << "coulndt find attack??";
	return false;
}

void Pokemon::deleteAllAttacks()
{
	for (int i{ 0 }; i < m_activeAttacks.size(); ++i) {
		delete m_activeAttacks[i];
	}
	m_activeAttacks.clear();
}

float Pokemon::getRange()
{
	return m_range;
}


void Pokemon::draw(sf::RenderTarget& window)
{
	if (stats.health <= 0)
		return;

	window.draw(m_sprite);
	for (int i{ 0 }; i < m_activeAttacks.size(); ++i) {
		m_activeAttacks[i]->draw(window);
	}

	m_healthBar.draw(window);
	m_statusEffect.draw(window);
}

void Pokemon::statusDamage(float damage)
{

	float oldHealth = stats.health;
	stats.health -= damage; // pure damage for status effects;

	sf::Vector2f direction{ 0.f,1.f }; // damage direction
	m_levelReference->spawnDamageText(m_sprite.getPosition(), direction, static_cast<int>(oldHealth - stats.health));


	m_healthBar.update(stats.health, stats.maxHealth);
	if (stats.health <= 0.f) {
		for (auto poke : m_hitByPokemon) // base exp 50 for now https://bulbapedia.bulbagarden.net/wiki/Experience#Experience_at_each_level
			poke->giveExp((100.f * stats.level) / 7.f * (1.f / m_hitByPokemon.size()));
		m_levelReference->enemyPokemonDied(this);
	}
}


bool Pokemon::damage(Pokemon* pokemon, AttackStats* attack ,float pokemonAtkStat, int pokemonLevel) {

	if (stats.health <= 0.f)
		return false;
	if (pokemon) { // for giving experience points
		bool addPokemon{ true };
		for (Pokemon* poke : m_hitByPokemon) {
			if (poke == pokemon) {
				addPokemon = false;
				break;
			}
		}
		if(addPokemon)
			m_hitByPokemon.push_back(pokemon);
	}
	float oldHealth = stats.health;
	// attack calculation  https://bulbapedia.bulbagarden.net/wiki/Damage#Generation_V_onward

	if (attack->category != AttackStats::AttackCategory::STATUS) {
		if (attack->category == AttackStats::AttackCategory::SPECIAL)
			stats.health -= ((((pokemonLevel * 2) / 5 + 2) * attack->power * (pokemonAtkStat / this->stats.spDefence) / 50) + 2) * PokemonStats::effectiveness(attack->type, stats.type1);
		else if (attack->category == AttackStats::AttackCategory::PHYSICAL)
			stats.health -= ((((pokemonLevel * 2) / 5 + 2) * attack->power * (pokemonAtkStat / this->stats.defense) / 50) + 2) * PokemonStats::effectiveness(attack->type, stats.type1);
		// health at 0 so its doesnt invert weirdly on ui stuff

		sf::Vector2f direction{ 0.f,1.f }; // always goes down standard 
		if (pokemon)
			direction = GameMath::normalize(m_sprite.getPosition() - pokemon->getPosition());
		m_levelReference->spawnDamageText(m_sprite.getPosition(), direction, static_cast<int>(oldHealth - stats.health));
	}


	switch (attack->additionalEffect) {
	case AttackStats::AdditionalEffects::WHIRLWIND:
		whirlwind();
		std::cout << "we got whirlwinded\n";
		break;

	default:
		break;
	}
	
	if (stats.health < 0.f) {
		stats.health = 0.f;
		if(!playerOwned())
			for (auto poke : m_hitByPokemon) // base exp 50 for now https://bulbapedia.bulbagarden.net/wiki/Experience#Experience_at_each_level
				poke->giveExp((100.f * stats.level) / 7.f * (1.f / m_hitByPokemon.size()) );
	}
	m_healthBar.update(stats.health, stats.maxHealth);
	if (attack->statusEffect != StatusEffect::None) {
		if (Random::get(1, 100) <= attack->statusEffectAccuracy)
			changeStatusEffect(attack->statusEffect);
	}


	return true;
}

bool Pokemon::canStartAttack() {
	if (m_attackCooldown <= 0.f)
		return true;
	return false;
}

PokemonStats& Pokemon::getStats()
{
	return stats;
}




float Pokemon::getHealth()
{
	return stats.health;
}


bool Pokemon::canCatch()
{
	return m_canCatch && stats.health / stats.maxHealth < 0.3f;
}

PokemonStats* Pokemon::getBaseStats()
{
	return m_baseStats;
}



sf::Vector2f Pokemon::getPosition()
{
	return m_position;
}



void Pokemon::setPosition(sf::Vector2f position)
{
	m_position = position;
	m_sprite.setPosition(position);
	m_healthBar.setPosition(m_position - sf::Vector2f{ 0.f,20.f });
}

std::string& Pokemon::getName()
{
	return stats.name;
}



sf::Texture* Pokemon::getTexture()
{
	return m_texture;
}




// for killing pokemon
void Pokemon::setLevelReference(Level* level)
{
	m_levelReference = level;
	m_path = level->getPath();
}

bool Pokemon::playerOwned()
{
	return false;
}

std::array<AttackStats*, 4>* Pokemon::getAttacks()
{
	return &m_attacks;
}


void Pokemon::setCurrentAttack(AttackStats* attack) {
	//std::cout << static_cast<int>(m_currentAttack.attackClass);
	m_currentAttack = attack;
	m_attackCooldown = attack->attackCooldown;
	//std::cout << static_cast<int>(m_currentAttack.attackClass);
}

AttackStats* Pokemon::getCurrentAttack() {
	return m_currentAttack;
}

bool Pokemon::addAttack(AttackStats* attack)
{
	if (!attack)
		return false;

	for (int i{ 0 }; i < m_attacks.size(); ++i) {
		if (m_attacks[i] == attack)
			return false; // because we already have that attack;

		if (m_attacks[i] == nullptr) {
			m_attacks[i] = attack;
			return true;
		}
	}
	return false;
}

void Pokemon::updateAttacks(std::array<AttackStats*, 4>& attacks)
{
	bool hasCurrentAttack{ false };
	for (int i{ 0 }; i < 4; ++i) {
		if (m_currentAttack == attacks[i])
			hasCurrentAttack = true;
		m_attacks[i] = attacks[i];
	}
	if (!hasCurrentAttack)
		m_currentAttack = m_attacks[0];
}

bool Pokemon::levelUp()
{
	if (m_canLevelUp) {
		m_canLevelUp = false;
		++stats.level;
		reCalculateStats();
		return true;
	}
	return false;
}

bool Pokemon::canLevelUp()
{
	return m_canLevelUp;
}

void Pokemon::setCanCatch(bool canCatch)
{
	m_canCatch = canCatch;
	m_healthBar.canCatch(canCatch);
}

void Pokemon::evolve(PokemonStats* baseStats, sf::Texture* texture)
{
	std::cout << "Evolved " << m_baseStats->name << " INTO: " << baseStats->name << "\n";
	
	stats.id    = baseStats->id;
	stats.name  = baseStats->name;
	stats.type1 = baseStats->type1;
	stats.type2 = baseStats->type2;
	m_baseStats = baseStats;
	reCalculateStats();
	//std::cout << stats.name << " has " << stats.health << " health\n";

	m_range = 150.f;
	m_spriteSize = texture->getSize().x / 4;
	m_texture = texture;
	m_sprite.setTexture(*m_texture);
	m_sprite.setTextureRect(sf::IntRect{ 0,0,m_spriteSize, m_spriteSize });
	m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

void Pokemon::giveExp(float experience)
{
	if (m_canLevelUp)
		return;
	stats.experience += experience;
	int nextlevel = stats.level + 1;
	float nextlevelExperience = 5.f * (nextlevel * nextlevel * nextlevel) / 4.f;
	if(stats.experience >= nextlevelExperience) { // level cubed
		m_canLevelUp = true;
		stats.experience = nextlevelExperience;
	}
}


float Pokemon::calculateStat(float stat) {
	return floor(0.01f * (2.f * stat + 16.f) * stats.level) + 5.f;
}

void Pokemon::reCalculateStats()
{
	// iv is 16 for now https://pokemon.fandom.com/wiki/Statistics
	stats.maxHealth = floor(0.01f * (2.f * m_baseStats->maxHealth + 16.f) * stats.level + 10.f);
	stats.health = stats.maxHealth;

	stats.attack = calculateStat(m_baseStats->attack);
	stats.spAttack = calculateStat(m_baseStats->spAttack);
	stats.defense = calculateStat(m_baseStats->defense);
	stats.spDefence = calculateStat(m_baseStats->spDefence);
	stats.speed = calculateStat(m_baseStats->speed);
	m_healthBar.updateLevel(stats.level);
}


void Pokemon::setEnemies(LinkedList<Pokemon>* enemies) {
	m_enemiesHead = enemies;
}

PickupItem* Pokemon::getPickupItem()
{
	return m_pickupItem;
}

void Pokemon::setPickupItem(PickupItem* item)
{
	if (m_levelReference->isDeadEnd()) {
		if (m_walkingForward) {
			m_currentWayPoint--;
			m_walkingForward = false;
		}
	}
	
	m_pickupItem = item;
}

bool Pokemon::canPickupItem()
{
	return m_pickupItem == nullptr && stats.health > 0.f;
}

sf::FloatRect Pokemon::getFloatRect()
{
	return m_sprite.getGlobalBounds();
}


void Pokemon::attack(float delta)
{
	// always call this function to lower the attack cooldown
	// if attack went well return
	for (int i{ 0 }; i < m_activeAttacks.size(); ++i) {
		if (!m_activeAttacks[i]->update(delta)) {
			removeAttack(m_activeAttacks[i]);
			--i; // because vector resized;
		}
	}
	// enemies is not initalised or attack is on cooldown return
	if (!canStartAttack() || !m_enemiesHead)
		return;


	// if there is no linkedlist enemy initialised or attack is on cooldown return
	Pokemon* closestPokemon{ nullptr };
	for (LinkedList<Pokemon>* i{ m_enemiesHead->next }; i; i = i->next) {
		if (GameMath::distance(m_position, i->value->getPosition()) <= m_range) {
			if (!closestPokemon)
				closestPokemon = i->value;
			else if (GameMath::distance(m_position, i->value->getPosition()) < GameMath::distance(m_position, closestPokemon->getPosition()))
				closestPokemon = i->value;
		}
	}
	if (closestPokemon == nullptr) {
		m_attackCooldown = 0.f;
		return;
	}

	Attack* temp;
	switch (m_currentAttack->attackClass) {
	case AttackStats::AttackClass::PROJECTILE_CLASS:
		//std::cout << "projectile\n";
		temp = new ProjectileAttack(m_currentAttack, stats);
		m_activeAttacks.emplace_back(temp);
		m_activeAttacks.back()->startAttack(this, closestPokemon, delta, m_levelReference);
		break;

	case AttackStats::AttackClass::TACKLE_CLASS:
		//std::cout << "tackle\n";
		temp = new AttackPokemonSprite(m_currentAttack, stats);
		m_activeAttacks.emplace_back(temp);
		m_activeAttacks.back()->startAttack(this, closestPokemon, delta, m_levelReference);
		break;
	case AttackStats::AttackClass::STANDING_CLASS:

		temp = new AttackStanding(m_currentAttack, stats);
		m_activeAttacks.emplace_back(temp);
		m_activeAttacks.back()->startAttack(this, closestPokemon, delta, m_levelReference);

		break;
	case AttackStats::AttackClass::AOESTANDING_CLASS:

		temp = new AOEAttackStanding(m_currentAttack, stats);
		temp->startAOEAttack(this, closestPokemon, delta, m_levelReference, m_enemiesHead);
		m_activeAttacks.emplace_back(temp);
		break;
	case AttackStats::AttackClass::AOEPROJECTILE_CLASS:

		temp = new AOEAttackProjectile(m_currentAttack, stats);
		temp->startAOEAttack(this, closestPokemon, delta, m_levelReference, m_enemiesHead);
		m_activeAttacks.emplace_back(temp);
		break; 
	case AttackStats::AttackClass::CONTACT_CLASS:
		//std::cout << "tackle\n";
		temp = new AttackContact(m_currentAttack, stats);
		m_activeAttacks.emplace_back(temp);
		m_activeAttacks.back()->startAttack(this, closestPokemon, delta, m_levelReference);
		break;

	case AttackStats::AttackClass::AOEHITBOX_CLASS:
		temp = new AOEHitboxAttack(m_currentAttack, stats);
		temp->startAOEAttack(this, closestPokemon, delta, m_levelReference, m_enemiesHead);
		m_activeAttacks.emplace_back(temp);
		break;
	case AttackStats::AttackClass::AOEHITBOXMOVING_CLASS:
		temp = new AOEHitboxAttackMoving(m_currentAttack, stats);
		temp->startAOEAttack(this, closestPokemon, delta, m_levelReference, m_enemiesHead);
		m_activeAttacks.emplace_back(temp);
		break;
	case AttackStats::AttackClass::NONE_CLASS:
		std::cout << "NONE\n";
		return;
	default:
		std::cout << "ERROR PLAYER POKEMON\n";
	}
	updateSprite(closestPokemon->getPosition() - m_position);
	m_attackCooldown += m_currentAttack->attackCooldown;
	std::cout << m_attackCooldown << " is the attack cooldown\n";
	//std::cout << m_name << m_position.x << m_position.y << " Attacked: " << (*m_enemies)[i].getPosition().x << "," << (*m_enemies)[i].getPosition().y << GameMath::distance(m_position, (*m_enemies)[i].getPosition()) << "\n";
}