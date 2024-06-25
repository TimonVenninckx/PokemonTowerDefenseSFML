#include "Game.h"
#include "ui/GameUI.h";
#include <iostream>
#include "sqlite/Database.h"
#include "SoundEffectManager.h"

sf::Font    GameUI::font;
sf::Texture GameUI::levelUpArrow;
sf::Texture GameUI::border;
sf::Texture GameUI::statusTexture;
sf::Texture GameUI::statusIconTexture;
sf::Texture GameUI::levelLockedTexture;
sf::Color	GameUI::backgroundColor = sf::Color{ 30,30,30,255 };
sf::Color	GameUI::buttonColor = sf::Color{ sf::Color::Red };
sf::Sound	GameUI::clickSound;
sf::SoundBuffer GameUI::clickSoundBuffer;
sf::Music		GameUI::music;
float			GameUI::musicVolume{ 10.f };
float			GameUI::sfxVolume{ 10.f };
bool			GameUI::isFullScreen{ false };



sf::SoundBuffer SoundEffectManager::hitSoundBuffer{};
LinkedList<sf::Sound> SoundEffectManager::sounds;


Game::Game()
{
	GameUI::clickSoundBuffer.loadFromFile("sound/cutclick.mp3");
	GameUI::clickSound.setBuffer(GameUI::clickSoundBuffer);
	GameUI::clickSound.setVolume(GameUI::sfxVolume);
	GameUI::music.setVolume(GameUI::musicVolume);
	GameUI::music.setLoop(true);

	SoundEffectManager::hitSoundBuffer.loadFromFile("sound/attack/tackle.mp3");

	// set font
	if (!GameUI::font.loadFromFile("fonts/PixeloidSans-mLxMm.ttf")) {
		std::cout << "Could not load font\n";
		exit(0);
	}
	if (!GameUI::levelUpArrow.loadFromFile("textures/uparrow.png")) {
		std::cout << "could not load levelupArrow\n";
	}
	if (!GameUI::border.loadFromFile("textures/bar.png")) {
		std::cout << "could not load border\n";
	}
	if (!GameUI::statusIconTexture.loadFromFile("textures/statuseffects/statusicons.png")) {
		std::cout << "could not load statusicons\n";
	}
	if (!GameUI::statusTexture.loadFromFile("Textures/statuseffects/statuseffects.png")) {
		std::cout << "could not load statuseffects\n";
	}
	if (!GameUI::levelLockedTexture.loadFromFile("Textures/levellocked.png")) {
		std::cout << "could not load statuseffects\n";
	}
	
	GameUI::border.setSmooth(true);
	GameUI::statusIconTexture.setSmooth(false);



	fpsCounter.setFont(GameUI::font);
	fpsCounter.setPosition(0.f, 1040.f);

	window.create(sf::VideoMode(1080, 690), "Tower Defense"); // -30 in the Y because of bar???

	levelMenu = new LevelMenu(window,player,uiView);
	saveMenu = new SaveMenu(player, window, uiView);
	//window.setFramerateLimit(60);

	// setting ui
	if (window.getSize().x / 16.f > window.getSize().y / 9.f) {
		float normalWidth = window.getSize().x - ((window.getSize().y / 9.f) * 16.f);
		uiView.reset(sf::FloatRect{ -normalWidth / 2.f, 0,  1920 + normalWidth, 1080 });
	}
	else {
		float normalHeight = window.getSize().y - ((window.getSize().x / 16.f) * 9.f);
		uiView.reset(sf::FloatRect{ 0, -normalHeight / 2.f, 1920, 1080 + normalHeight });
	}
	

	run();
}

void Game::run()
{
	window.setView(uiView);

	sf::Clock sfmlClock;
	while (window.isOpen()) {


		window.clear();


		float deltaTime = sfmlClock.restart().asSeconds();
		int fps = static_cast<int>(1.f / deltaTime);
		fpsCounter.setString(std::to_string(fps));

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized) {
				if (event.size.width / 16.f > event.size.height / 9.f) {
					float normalWidth = event.size.width - ((event.size.height / 9.f) * 16.f);
					uiView.reset(sf::FloatRect{ -normalWidth / 2.f, 0,  1920 + normalWidth, 1080 });
				}
				else {
					float normalHeight = event.size.height - ((event.size.width / 16.f) * 9.f);
					uiView.reset(sf::FloatRect{ 0, -normalHeight / 2.f, 1920, 1080 + normalHeight });
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.scancode == sf::Keyboard::Scan::F11) {
					if (event.type == sf::Event::KeyPressed) {
						if (GameUI::isFullScreen) {
							GameUI::isFullScreen = false;
							window.create(sf::VideoMode(1080, 690), "Tower Defense"); // -30 in the Y because of bar???
							window.setView(uiView);
						}
						else {
							GameUI::isFullScreen = true;
							window.create(sf::VideoMode::getFullscreenModes()[0], "Tower Defense", sf::Style::Fullscreen);
							window.setView(uiView);
						}
					}
				}
			}
			
			if (saveMenu) {
				if (saveMenu->chooseSave(event)) {
					delete saveMenu;
					saveMenu = nullptr;
					levelMenu->reload();
				}
			}
			else if (currentLevel == 0) { // not equal to 0
				if (levelMenu->clickedBackButton(window,event)) {
					saveMenu = new SaveMenu(player, window, uiView); 
					continue;
				}
				LevelInfo* info = levelMenu->chosenLevelId(window, deltaTime, event);
				if (info) {
					currentLevel = info->id;
					level = new Level(window, player, Database::getLevelInformation(currentLevel), uiView);
				};
			}
			else if (currentLevel != 0) {
				if (!level->updateEvents(event)) {
					delete level;
					currentLevel = 0;
					levelMenu->reload();
				}
			}
		}

		if (saveMenu)
			saveMenu->draw(window);
		else if (currentLevel == 0)
			levelMenu->draw(window);
		else {
			if (deltaTime > 0.3f)
				continue;
			level->run(deltaTime);
		}

		window.draw(fpsCounter);


		window.display();
	}
}
