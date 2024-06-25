#include "LevelMenu.h"
#include "../sqlite/Database.h"
#include "GameUI.h"

LevelMenu::LevelMenu(sf::RenderWindow& window, Player& player,sf::View& uiview)
	: m_player{ player }
	, m_view{ uiview }
	, m_pickPokemonMenu{ player, uiview, window }
{
	for (int i{ 1 }; i < 10; ++i) {
		LevelInfo info{ Database::getLevelInformation(i) };
		if (info.fileLocation == "")
			break;
		LevelThumbnailUI* thumb = new LevelThumbnailUI{ info, { 300.f * ((i - 1) % 6) + 100.f , 150.f + (i / 7) * 350}, { 250.f,280.f } };
		m_thumbnails.emplace_back(thumb);
	}

	m_saveButton.setCornerPointCount(5);
	m_saveButton.setCornersRadius(15.f);
	m_saveButton.setSize({250.f, 120.f});
	m_saveButton.setOrigin(m_saveButton.getGlobalBounds().getSize() / 2.f);
	m_saveButton.setPosition(960.f, 900.f);
	m_saveButton.setFillColor(GameUI::buttonColor);

	m_saveText.setFont(GameUI::font);
	m_saveText.setString("SAVE");
	m_saveText.setCharacterSize(70);
	m_saveText.setOrigin(m_saveText.getGlobalBounds().getSize().x / 2.f, m_saveText.getGlobalBounds().getSize().y / 1.4f);
	m_saveText.setPosition(960.f, 900.f);


	m_backButton.setCornerPointCount(5);
	m_backButton.setCornersRadius(15.f);
	m_backButton.setSize({ 250.f, 120.f });
	m_backButton.setOrigin(m_backButton.getGlobalBounds().getSize() / 2.f);
	m_backButton.setPosition(135.f, 70.f);
	m_backButton.setFillColor(GameUI::buttonColor);

	m_backText.setFont(GameUI::font);
	m_backText.setString("BACK");
	m_backText.setCharacterSize(70);
	m_backText.setOrigin(m_backText.getGlobalBounds().getSize().x / 2.f, m_backText.getGlobalBounds().getSize().y / 1.4f);
	m_backText.setPosition(135.f, 70.f);



	m_twoOptionSaveMenu.setButton1Text("Save");
	m_twoOptionSaveMenu.setButton2Text("Dont Save");
	m_twoOptionSaveMenu.setInfoText("Do you want to save\nbefore returning to the Main Menu");
}

LevelMenu::~LevelMenu()
{
	for (LevelThumbnailUI* thumbnail : m_thumbnails)
		delete thumbnail;
	m_thumbnails.clear();
}

bool LevelMenu::clickedBackButton(sf::RenderWindow& window, sf::Event& event)
{
	if (m_chosenLevel)
		return false;
	if (!m_goingBack)
		return false;

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), m_view);
	if (event.type == sf::Event::MouseButtonPressed) {
		if (int option = m_twoOptionSaveMenu.pickedOption(mousePos)) {
			if (option == 1) {
				m_player.save();
			}
			m_goingBack = false;
			return true;
		}
	}
	return false;
}

LevelInfo* LevelMenu::chosenLevelId(sf::RenderWindow& window, float delta,sf::Event& event)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), m_view);
	if (m_chosenLevel) {
		if (m_pickPokemonMenu.doneUpdate(event, mousePos, delta)) {
			if (m_pickPokemonMenu.startLevel(mousePos)) {
				std::cout << m_chosenLevel->getLevel()->name << " level opened\n";
				return m_chosenLevel->getLevel();
			}
			else {
				m_chosenLevel = nullptr;
				GameUI::music.openFromFile("sound/music/lyra.mp3");
				GameUI::music.play();
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed){
		//std::cout << "we clicked mannn\n";
		for (LevelThumbnailUI* thumbnail : m_thumbnails) {
			if (!thumbnail->isLocked() && thumbnail->contains(mousePos)) {
				GameUI::clickSound.play();
				m_chosenLevel = thumbnail;
				m_pickPokemonMenu.reload();
			}
		}
		if (m_saveButton.getGlobalBounds().contains(mousePos)) {
			m_player.save();
			GameUI::clickSound.play();
		}
		else if (m_backButton.getGlobalBounds().contains(mousePos)) {
			GameUI::clickSound.play();
			m_goingBack = true;
		}
	}
	return nullptr;
}

void LevelMenu::reload()
{
	m_goingBack = false;
	m_pickPokemonMenu.reload();
	m_chosenLevel = nullptr;
	GameUI::music.openFromFile("sound/music/lyra.mp3");
	GameUI::music.play();

	for (int i{ 0 }; i < m_thumbnails.size(); i++) {
		m_thumbnails[i]->lock(i > m_player.getProgression());
	}
}

void LevelMenu::draw(sf::RenderWindow& window)
{
	window.setView(m_view);

	if (m_chosenLevel) {
		m_pickPokemonMenu.draw(window);
		return;
	}

	for (LevelThumbnailUI* thumbnail : m_thumbnails)
		thumbnail->draw(window);

	window.draw(m_saveButton);
	window.draw(m_saveText);
	window.draw(m_backButton);
	window.draw(m_backText);
	
	if (m_goingBack)
		m_twoOptionSaveMenu.draw(window);
}

