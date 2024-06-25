#include "Level.h"
#include <iostream>
#include "GameMath.h"
#include "ui/PokemonPlaceUI.h"
#include "ui/GameUI.h"
#include "SpriteLoader.h"

/*
* =========================================================================
*							INITIALIZE
* =========================================================================
*/

Level::Level(sf::RenderWindow& renderwindow, Player& playerref, LevelInfo level,sf::View& uiview)
	: window{ renderwindow }
	, player{ playerref }
	, waveManager{ level.id }
	, pokemonInfoUI{ playerref }
	, uiView{ uiview }
	, textBoxManager{ level.id }
{
	levelInfo = level;

	activeEnemiesHead = new LinkedList<Pokemon>;
	activePlayerPokemonHead = new LinkedList<Pokemon>;
	pickupItemsHead = new LinkedList<PickupItem>;


	view.reset(sf::FloatRect{ 0.f, 0.f, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});

	

	window.setView(view);

	path = level.path;

	pokeballUI.setPokemon(activeEnemiesHead);

	if(path.empty())
		waveManager.init(activeEnemiesHead, activePlayerPokemonHead,pickupItemsHead, this, sf::Vector2f{ 0.f,0.f }, level.waves);
	else
		waveManager.init(activeEnemiesHead,activePlayerPokemonHead,pickupItemsHead, this,path[0],level.waves);

	//tilemap
	if (!tileMap.loadFromFile("textures/levels/" + level.fileLocation)) {
		std::cout << "could not load tilemap\n";
		exit(0);
	}
	sprite.setTexture(tileMap);


	pauseTexture.loadFromFile("textures/pausebutton.png");
	pauseButton.setTexture(pauseTexture);
	pauseButton.setOrigin(pauseButton.getGlobalBounds().getSize().x / 2.f,0.f);
	pauseButton.setPosition(960.f,0.f);
	pauseButton.setScale(0.5f, 0.5f);


	m_playerMoney.setFont(GameUI::font);
	m_playerMoney.setCharacterSize(50);
	m_playerMoney.setOutlineThickness(3);
	m_playerMoney.setString("$" + std::to_string(player.getMoney()));



	// cant place sprites for dragging pokemon
	m_cantPlaceTexture.loadFromFile("textures/cantplace.png");
	m_cantPlaceSprite.setTexture(m_cantPlaceTexture);
	m_cantPlaceSprite.setOrigin(m_cantPlaceSprite.getGlobalBounds().getSize() / 2.f);

	for (float i{ 1 }; i < 5; ++i) {
		GameSpeedUI speed(i,{ 1570 + (i * 70), 10 });
		gameSpeeds.emplace_back(speed);
	}
	currentGameSpeed = &gameSpeeds[0];
	currentGameSpeed->setActive(true);

	cameraSpeed =  800;

	// characters
	for (auto& info : levelInfo.characters) {
		Character* c = new Character{ info.sprite,info.pos };
		characters.emplace_back(c);
	}
	
	//pokemon placements
	if (!m_pokemonPositionTexture.loadFromFile("textures/border.png")) {
		std::cout << "Couldnt find pokemon position texture\n";
		exit(0);
	}
	for (int i{ 0 }; i < level.placementLocations.size(); ++i) {
		PokemonPlacement pokemonplacement{ &m_pokemonPositionTexture };
		pokemonplacement.setPosition({ level.placementLocations[i] });

		m_pokemonPositions.push_back(pokemonplacement);
	}

	//pokemon ui initialize
	for (int i{ 0 }; i < 6; ++i) {
		PokemonPlaceUI pokemonUI({300 +  230.f * i,850.f });
		activePokemonUI.emplace_back(pokemonUI);
	}

	// active pokemon set in level
	for (int i{ 0 }; i < player.activePokemon.size(); ++i) {
		if (player.activePokemon[i] == nullptr)
			continue;
		player.activePokemon[i]->reset();
		player.activePokemon[i]->setGameUI(activePokemonUI[i]);
		player.activePokemon[i]->setEnemies(activeEnemiesHead);
		player.activePokemon[i]->setLevelReference(this);

		//activePlayerPokemonHead.emplace_back(&player.activePokemon[i]);
	}

	
	GameUI::music.openFromFile("sound/music/" + level.songPath);
	GameUI::music.setLoop(true);
	GameUI::music.play();
}




Level::~Level() {

	LinkedList<Pokemon>* tempEnemy = activeEnemiesHead->next;
	while (tempEnemy) {
		LinkedList<Pokemon>* temp2 = tempEnemy;
		tempEnemy = tempEnemy->next;
		delete temp2->value;
		delete temp2;
	}
	delete activeEnemiesHead;


	LinkedList<Pokemon>* tempPokemon = activePlayerPokemonHead->next;
	while(tempPokemon) {
		LinkedList<Pokemon>* temp2 = tempPokemon;
		tempPokemon = tempPokemon->next;
		static_cast<PlayerPokemon*>(temp2->value)->setActive(false);
		delete temp2;
	}
	delete activePlayerPokemonHead;

	LinkedList<PickupItem>* tempPickup = pickupItemsHead->next;
	while (tempPickup) {
		LinkedList<PickupItem>* temp2 = tempPickup;
		tempPickup = tempPickup->next;
		delete temp2->value;
		delete temp2;
	}
	delete pickupItemsHead;
	
	for (int i{ 0 }; i < characters.size(); i++) {
		delete characters[i];
	}
	characters.clear();


}



/*
* =========================================================================
*								GAME LOOP
* =========================================================================
*/




bool Level::updateEvents(sf::Event& event)
{
	window.setView(view);
	if (event.type == sf::Event::Resized) {
		view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
		view.zoom(m_zoom);
	}
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (gameEnded) {
			return !gameOverMenu.exitUI(window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView));
		}
	}
	if (pauseMenu.isPaused()) {
		if (pauseMenu.exitGame(window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView), event))
			return false;
		return true;
	}
	else if (textBoxManager.isActive()) {
		textBoxManager.update(event, window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView));
		return true;
	}
	else if (pokemonCaughtUI.isActive()) {
		pokemonCaughtUI.update(event);
	}
	keyPressed(event);
	return true;
}

void Level::run(float originalDelta)
{
	float delta = originalDelta * currentGameSpeed->getGameSpeed();


	if (pauseMenu.isPaused() || gameEnded || textBoxManager.isActive()) { // if no event happened
		render();
		return;
	} // dont do the rest of events if paused

	updateMouse(originalDelta);

	if (!m_draggingPokemon && !m_viewingPokemon && !pokeballUI.isActive() && !pokemonCaughtUI.isActive())
		update(delta);

	render();
}





/*
* =========================================================================
*							RENDERING
* =========================================================================
*/
void Level::render()
{

	window.setView(view);
	window.draw(sprite);
	
	for (int i{ 0 }; i < characters.size(); i++) {
		characters[i]->draw(window);
	}

	if (m_draggingPokemon) {
		m_draggingPokemon->drawRange(window);
		for (int i{ 0 }; i < m_pokemonPositions.size(); ++i) {
			if (m_pokemonPositions[i].contains(mouseWorldPosition))
				break;
			else if(m_pokemonPositions.size() - 1 == i) { // if doesnt contain and is last item in array
				m_cantPlaceSprite.setPosition(m_draggingPokemon->getPosition() + sf::Vector2f{ 0.f,10.f });
				window.draw(m_cantPlaceSprite);
			}
		}
	}
	if (m_viewingPokemon)
		m_viewingPokemon->drawRange(window);

	for (int i{ 0 }; i < m_pokemonPositions.size(); ++i) {
		m_pokemonPositions[i].draw(window);
	}

	

	for (LinkedList<Pokemon>* i{ activeEnemiesHead->next }; i; i = i->next) {
		i->value->draw(window);
	}


	for (LinkedList<Pokemon>* i{ activePlayerPokemonHead->next }; i; i = i->next) {
		i->value->draw(window);
	}

	for (LinkedList<PickupItem>* i{ pickupItemsHead->next }; i; i = i->next) {
		i->value->draw(window);
	}

	for (DamageText& damage : m_damageTexts) {
		damage.draw(window);
	}
	

	renderUI();

}

void Level::renderUI()
{
	window.setView(uiView);
	
	if (textBoxManager.isActive()) {
		textBoxManager.draw(window);
		return;
	}

	if (m_viewingPokemon) {
		pokemonInfoUI.draw(window);
		return;
	}

	window.draw(m_playerMoney);

	for (int i{ 0 }; i < activePokemonUI.size(); ++i) {
		activePokemonUI[i].draw(window);
	}

	if (pauseMenu.isPaused())
		pauseMenu.draw(window);
	else
		window.draw(pauseButton);

	for (int i{ 0 }; i < gameSpeeds.size(); ++i) {
		gameSpeeds[i].draw(window);
	}
	waveManager.draw(window);

	if (gameEnded) {
		gameOverMenu.draw(window);
		return;
	}


	if (pokemonCaughtUI.isActive()) {
		pokemonCaughtUI.draw(window);
	}


	pokeballUI.draw(window);
	if (pokeballUI.isActive()) {
		window.setView(view);
		pokeballUI.drawPokemon(window);
	}
	// make sure this is last because we switched the view
}



/*
* =========================================================================
*							UPDATE
* =========================================================================
*/


void Level::update(float delta)
{
	if (!waveManager.update(delta)) {
		// we either lost or won
		gameEnded = true;
		if (waveManager.won()) {
			gameOverMenu.setText(levelInfo.winText);
			gameOverMenu.setSprite(levelInfo.winImage);
			GameUI::music.openFromFile("sound/music/victorywild.mp3");
			GameUI::music.play();
			if (player.getProgression() < levelInfo.id)
				player.setProgression(levelInfo.id);
		}
		else {
			gameOverMenu.setText(levelInfo.loseText);
			gameOverMenu.setSprite(levelInfo.loseImage);
			GameUI::music.stop();
		}
		return;
	}

	LinkedList<Pokemon>* playerPokemon{ activePlayerPokemonHead->next };
	while(playerPokemon) {
		LinkedList<Pokemon>* lastPokemon = playerPokemon;
		playerPokemon = playerPokemon->next;
		lastPokemon->value->update(delta);
	}
	LinkedList<Pokemon>* enemy{ activeEnemiesHead->next };
	while(enemy) {
		LinkedList<Pokemon>* lastEnemy = enemy;
		enemy = enemy->next;
		lastEnemy->value->update(delta);
	}

	for (LinkedList<PickupItem>* i{ pickupItemsHead->next }; i; i = i->next) {
		i->value->update();
	}

	for (int i{ 0 }; i < m_damageTexts.size(); i++) {
		if (!m_damageTexts[i].update(delta)) {
			m_damageTexts.erase(m_damageTexts.begin() + i);
			i--;
		}
	}

	

}

void Level::playerPokemonDied(Pokemon* pokemon)
{
	if (!pokemon) {
		std::cout << "how can a null pointer die????\n";
		return;
	}

	LinkedList<Pokemon>* lastIndex = activePlayerPokemonHead;
	for (LinkedList<Pokemon>* it{ activePlayerPokemonHead->next }; it; lastIndex = it, it = it->next) {
		if (it->value == pokemon) {
			std::cout << "Killed " << pokemon->getName() << '\n';
			lastIndex->next = it->next;
			static_cast<PlayerPokemon*>(pokemon)->setActive(false);
			static_cast<PlayerPokemon*>(pokemon)->setPlacement(nullptr);

			for (LinkedList<Pokemon>* i{ activeEnemiesHead->next }; i; i = i->next) {
				i->value->checkIfTargetDead(pokemon);
			}

			delete it; // delete the linked list
			return;
		}
	}
	std::cout << "level.cpp couldn't find Dead player pokemon???\n";
}


void Level::enemyPokemonDied(Pokemon* pokemon)
{
	if (!pokemon) {
		std::cout << "how can a null pointer die????\n\n\n\n\n\n";
		return;
	}

	LinkedList<Pokemon>* lastIndex = activeEnemiesHead;
	for (LinkedList<Pokemon>* it{ activeEnemiesHead->next }; it; lastIndex = it, it = it->next) {
		if (it->value == pokemon) {
			// add money
			if (pokemon->getHealth() <= 0.f) {
				player.addMoney(pokemon->getStats().level); // give level amount of money to the player
				m_playerMoney.setString("$" + std::to_string(player.getMoney()));
				std::cout << "Killed " << pokemon->getName() << '\n';
			}
			lastIndex->next = it->next;

			for (LinkedList<Pokemon>* i{ activePlayerPokemonHead->next }; i; i = i->next) {
				i->value->checkIfTargetDead(pokemon);
			}


			delete it->value;
			delete it;
			return;
		}
	}
	std::cout << "level.cpp couldn't find Dead enemy pokemon???\n";
}

bool Level::reachedEnd(Pokemon* pokemon)
{
	if (!pokemon)
		return false;

	PickupItem* item = pokemon->getPickupItem();
	
	if (pickupItemsHead->next && item == nullptr)
		return false;

	// if there are no pickups left or we have an item kill pokemon
	if (pokemon->playerOwned())  
		playerPokemonDied(pokemon);
	else
		enemyPokemonDied(pokemon);

	if (item == nullptr) // if we dont have an item dont search for it and return true
		return true;

	LinkedList<PickupItem>* lastPickup { pickupItemsHead };
	for (LinkedList<PickupItem>* it{ pickupItemsHead->next }; it; it = it->next) {
		if (it->value == item) {
			lastPickup->next = it->next;
			delete it;
			break;
		}
		lastPickup = it;
	}
	delete item;
	return true;
}

bool Level::isDeadEnd()
{
	return levelInfo.deadEnd;
}

void Level::spawnDamageText(sf::Vector2f location,sf::Vector2f direction, int damage)
{
	DamageText dmgtext(location,direction,damage);
	m_damageTexts.emplace_back(dmgtext);
}

void Level::spawnDamageText(sf::Vector2f location, sf::Vector2f direction, std::string text)
{
	DamageText dmgtext(location, direction, text);
	m_damageTexts.emplace_back(dmgtext);
}

std::vector<sf::Vector2f>* Level::getPath()
{
	return &path;
}


/*
* =========================================================================
*							INPUT
* =========================================================================
*/


void Level::updateMouse(float delta)
{

	// make sure to give it the correct view
	if (mousePanning) {
		view.move(mouseWorldPosition - window.mapPixelToCoords(sf::Mouse::getPosition(window), view));
		if (view.getCenter().x * 2.f < 0.f)
			view.move(0.f - view.getCenter().x * 2.f, 0.f);
		else if (view.getCenter().x > sprite.getGlobalBounds().getSize().x)
			view.move(sprite.getGlobalBounds().getSize().x - view.getCenter().x,0.f);
		if (view.getCenter().y * 2.f < 0.f)
			view.move(0.f, 0.f - view.getCenter().y * 2.f);
		else if (view.getCenter().y > sprite.getGlobalBounds().getSize().y)
			view.move(0.f, sprite.getGlobalBounds().getSize().y - view.getCenter().y);
	}

	mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

	if (m_draggingPokemon) {
		m_draggingPokemon->setPosition(mouseWorldPosition);
	}

	if (pokeballUI.isActive()) {
		pokeballUI.setPosition(mouseWorldPosition, window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView));
	}

	if (m_draggingPokemon || pokeballUI.isActive()) { // moving
		if (sf::Mouse::getPosition(window).x < static_cast<int>(window.getSize().x) / 15) {
			view.move({ delta * -cameraSpeed, 0 });
		}
		else if (sf::Mouse::getPosition(window).x > static_cast<int>(window.getSize().x) / 15 * 14) {
			view.move({ delta * +cameraSpeed, 0 });
		}
		if (sf::Mouse::getPosition(window).y < static_cast<int>(window.getSize().y) / 15) {
			view.move({ 0, delta * -cameraSpeed });
		}
		else if (sf::Mouse::getPosition(window).y > static_cast<int>(window.getSize().y) / 15 * 14) {
			view.move({ 0, delta * +cameraSpeed });
		}
	}
}



void Level::keyPressed(sf::Event& event)
{
	// some inputs
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.scancode == sf::Keyboard::Scan::Z) {
			if (event.type == sf::Event::KeyPressed) {
				//std::cout << activePlayerPokemonHead.size() << '\n';
				std::cout << window.mapPixelToCoords(sf::Mouse::getPosition(window), view).x << ',' << window.mapPixelToCoords(sf::Mouse::getPosition(window), view).y << '\n';
			}
		}/*
		else if (event.key.scancode == sf::Keyboard::Scan::P) {
			std::cout << "p\n";
			std::array<AttackStats*, 4> pokemonAttacks{ &Database::allAttackStats[33],nullptr,nullptr ,nullptr };
			for (int i{ 1 }; i < 10; ++i) {
				Pokemon* enemy = new Pokemon(&Database::baseStats[i], 5, SpriteLoader::getPokemonSprite(i), pokemonAttacks);
				enemy->setPosition({ -50.f * i, 400.f });
				enemy->setEnemies(activePlayerPokemonHead);
				enemy->setLevelReference(this);
				activeEnemiesHead->add(enemy);
			}
		} */ // for debugging if you want to spawn pokemon
		if (event.key.scancode == sf::Keyboard::Scan::Escape) {
			pauseMenu.setPaused(!pauseMenu.isPaused());
		}
	}
	
	// LEFT MOUSE BUTTON
	// shoudl fix the trypickup pokemon to be boolean
	else if (event.mouseButton.button == sf::Mouse::Left) {
		if (event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2f mouseposition = window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView);



			if (m_viewingPokemon){
				if (pokemonInfoUI.exitUI(mouseposition))
					m_viewingPokemon = nullptr;
				return; // always return
			}

			if (pauseButton.getGlobalBounds().contains(mouseposition)) {
				GameUI::clickSound.play();
				pauseMenu.setPaused(true);
				return;
			}
			if (pokeballUI.contains(mouseposition)) {
				pokeballUI.setActive(true);
				return;
			}

			//pickup pokemon or open stat window should be bool
			if (tryPickupPokemon())
				return;

			// game speeds checking
			for (int i{ 0 }; i < gameSpeeds.size(); ++i) {
				if (gameSpeeds[i].contains(mouseposition)) {
					if (currentGameSpeed != &gameSpeeds[i]) {
						GameUI::clickSound.play();
						currentGameSpeed->setActive(false);
						currentGameSpeed = &gameSpeeds[i];
						currentGameSpeed->setActive(true);
					}
					return;
				}
			}
			// if no other event is chosen:
			mousePanning = true;
		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			tryPlacePokemon();
			mousePanning = false;

			if (pokeballUI.isActive()) {
				if (pokeballUI.tryCatch()) {
					Pokemon* caught = pokeballUI.tryCatch();
					PlayerPokemon* pokemon = new PlayerPokemon{ caught };
					pokemon->reset();
					pokemon->setEnemies(activeEnemiesHead);
					pokemon->setLevelReference(this);

					bool unactivePokemon{ true };
					for (int i{ 0 }; i < activePokemonUI.size(); ++i) {
						if (activePokemonUI[i].getPokemon() == nullptr) {
							player.activePokemon[i] = pokemon;
							pokemon->setGameUI(activePokemonUI[i]);
							unactivePokemon = false;
							break;
						}
					}
					if (unactivePokemon) {
						for (int i{ 0 }; i < player.reservePokemon.size(); i++) {
							if (player.reservePokemon[i] == nullptr) {
								player.reservePokemon[i] = pokemon;
								break;
							}
						}
					}
					enemyPokemonDied(caught);
					pokemonCaughtUI.load(pokemon);
				}
				pokeballUI.setActive(false);
			}
		}
	}
	else if (event.type == sf::Event::MouseWheelMoved) {
		if (event.mouseWheel.delta == 1) {
			if (m_zoom > .5f) { // lowest is .4
				m_zoom -= .2f;
				view.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
				view.zoom(m_zoom);
			}
		}
		else {
			if (m_zoom < 1.f) { // highest is 1.
				m_zoom += .2f;
				view.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
				view.zoom(m_zoom);
			}
		}
	}
}

void Level::tryPlacePokemon()
{
	if (!m_draggingPokemon)
		return;

	bool dragPokemonHasPlacement{ false };
	// check if pokemon already has a placement
	if (m_draggingPokemon->getPlacement()) {
		if (m_draggingPokemon->getPlacement()->contains(mouseWorldPosition)) {  // pokemon is dropped at same placement
			m_draggingPokemon->setPlacement(m_draggingPokemon->getPlacement());
			m_draggingPokemon = nullptr;
			return;
		}
		else {
			dragPokemonHasPlacement = true;
		}
	}

	// if not already put down
	for (int i{ 0 }; i < m_pokemonPositions.size(); ++i) {
		if (m_pokemonPositions[i].contains(mouseWorldPosition)) {
			if (dragPokemonHasPlacement && m_pokemonPositions[i].getPokemon()) { // the placement has a pokemon already and we are swapping
				// pokemon that gets swapped
				PlayerPokemon* temp = m_pokemonPositions[i].getPokemon();
				temp->setPlacement(m_draggingPokemon->getPlacement());
				temp->setActive(true); // reset attack cooldowns and stuff
				m_draggingPokemon->setPlacement(&m_pokemonPositions[i]);
				m_draggingPokemon->setActive(true);
				m_draggingPokemon = nullptr;
				std::cout << "Swapped Pokmeon\n";
				return;
			}
			
			if (m_pokemonPositions[i].getPokemon()) {
				PlayerPokemon* temp = m_pokemonPositions[i].getPokemon();
				playerPokemonDied(temp); // sets it to inactive and remove;
			}
			else if(dragPokemonHasPlacement) {// if dragging pokemon had a placement but new position diddnt so no swap
				m_draggingPokemon->getPlacement()->setPokemon(nullptr);
			}
			m_draggingPokemon->setPlacement(&m_pokemonPositions[i]);
			m_draggingPokemon->setActive(true);
			m_draggingPokemon = nullptr;
			return;
		}
	}

	// if still not placed down
	LinkedList<Pokemon>* lastIndex = activePlayerPokemonHead;
	for (LinkedList<Pokemon>* it{ activePlayerPokemonHead->next }; it; lastIndex = it, it = it->next) {
		if (it->value == m_draggingPokemon) {
			std::cout << "Dropped Pokemon "<< m_draggingPokemon->getName() << "\n";
			lastIndex->next = it->next;
			delete it;
			m_draggingPokemon->setActive(false);
			
			if (m_draggingPokemon->getPlacement()) {
				m_draggingPokemon->getPlacement()->setPokemon(nullptr);
				m_draggingPokemon->setPlacement(nullptr);
			}
			m_draggingPokemon = nullptr;
			return;
		}
	}
}

bool Level::tryPickupPokemon()
{
	//picking up pokemon by clicking on the ui
	sf::Vector2f uiMouseposition = window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView);
	for (int i{ 0 }; i < activePokemonUI.size(); ++i) { // go through ui list
		if (activePokemonUI[i].contains(uiMouseposition)) {
			if (activePokemonUI[i].getPokemon()){ // check if ui has a pokemon
				if (activePokemonUI[i].getPokemon()->getHealth() > 0) { // check if not dead
					if (activePokemonUI[i].getPokemon()->isActive()){
						m_viewingPokemon = activePokemonUI[i].getPokemon();
						pokemonInfoUI.setPokemon(m_viewingPokemon);
						pokemonInfoUI.updateMoney();
						// move view so pokemon is in view 
						view.move(m_viewingPokemon->getPosition() - window.mapPixelToCoords(window.mapCoordsToPixel(sf::Vector2f{ 650.f, 500.f }, uiView),view));
						GameUI::clickSound.play();
						return true;
					}
					else
						m_draggingPokemon = activePokemonUI[i].getPokemon(); // set pokemon
				}
			}
		}
	}
	// picking up pokemon by clicking on them
	if (!m_draggingPokemon) {
		for(LinkedList<Pokemon>* it{ activePlayerPokemonHead->next }; it; it = it->next) {
			if (static_cast<PlayerPokemon*>(it->value)->contains(mouseWorldPosition)) {
				m_draggingPokemon = static_cast<PlayerPokemon*>(it->value);
				break;
			}
		}
	}

	// if we chose a valid pokemon
	if (m_draggingPokemon) {
		for (LinkedList<Pokemon>* it{ activePlayerPokemonHead->next }; it; it = it->next) {
			if (m_draggingPokemon == it->value) {
				return true; // dont add it if it already is active
			}
		}
		activePlayerPokemonHead->add(m_draggingPokemon);
		return true;
	}
	return false;
}
