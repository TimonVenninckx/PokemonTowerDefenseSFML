#include "SaveMenu.h"
#include "../sqlite/Database.h"

SaveMenu::SaveMenu(Player& player, sf::RenderWindow& window, sf::View& view)
	: m_saveInfos{{
	{ {960.f,300.f} },
	{ {960.f,600.f} },
	{ {960.f,900.f} } }}
	, m_window{ window }
	, m_view{ view }
	, m_player{ player }
{

	m_saveInfos[0].loadInfo(Database::getSaveInformation(1));
	m_saveInfos[1].loadInfo(Database::getSaveInformation(2));
	m_saveInfos[2].loadInfo(Database::getSaveInformation(3));

	GameUI::music.openFromFile("sound/music/titlescreen.mp3");
	GameUI::music.play();

	m_deleteTexture.loadFromFile("textures/cantplace.png");

	for (int i{ 0 }; i < m_deleteButtons.size(); i++) {
		m_deleteButtons[i].setPosition(1300.f, 300.f + 300.f * i);
		m_deleteButtons[i].setTexture(m_deleteTexture);
		m_deleteButtons[i].setOrigin(m_deleteButtons[i].getGlobalBounds().getSize() / 2.f);
	}

	m_deleteSaveMenu.setInfoText("Do you want to delete this save?");
	m_deleteSaveMenu.setButton1Text("Delete");
	m_deleteSaveMenu.setButton2Text("No");

	m_optionsButton.setCornerPointCount(5);
	m_optionsButton.setCornersRadius(15.f);
	m_optionsButton.setSize({ 400.f, 180.f });
	m_optionsButton.setPosition(250.f, 900.f);
	m_optionsButton.setFillColor(GameUI::buttonColor);
	m_optionsButton.setOrigin(m_optionsButton.getGlobalBounds().getSize() / 2.f);

	m_optionsText.setFont(GameUI::font);
	m_optionsText.setCharacterSize(80);
	m_optionsText.setString("Options");
	m_optionsText.setPosition(m_optionsButton.getPosition());
	m_optionsText.setOrigin(m_optionsText.getGlobalBounds().getSize().x / 2.f, m_optionsText.getCharacterSize() / 1.4f);

	m_quitButton.setCornerPointCount(5);
	m_quitButton.setCornersRadius(15.f);
	m_quitButton.setSize({ 300.f, 150.f });
	m_quitButton.setPosition(200.f, 125.f);
	m_quitButton.setFillColor(GameUI::buttonColor);
	m_quitButton.setOrigin(m_quitButton.getGlobalBounds().getSize() / 2.f);

	m_quitText.setFont(GameUI::font);
	m_quitText.setCharacterSize(80);
	m_quitText.setString("Exit");
	m_quitText.setPosition(m_quitButton.getPosition());
	m_quitText.setOrigin(m_quitText.getGlobalBounds().getSize().x / 2.f, m_quitText.getCharacterSize() / 1.4f);

}

SaveMenu::~SaveMenu()
{
	delete m_optionsMenu;
	delete m_starterPickmenu;
}



bool SaveMenu::chooseSave(sf::Event& event)
{
	sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window), m_view);


	if (m_optionsMenu) {
		if (m_optionsMenu->exitMenu(mousePos, event)) {
			delete m_optionsMenu;
			m_optionsMenu = nullptr;
		}
		return false;
	}

	if (event.type == sf::Event::MouseButtonPressed) {
		if (m_deletingSave) {
			if (int option = m_deleteSaveMenu.pickedOption(mousePos)) {
				if (option == 1) {
					m_saveInfos[m_deletingSave - 1].resetSave();
					Database::deleteSave(m_saveInfos[m_deletingSave - 1].getSaveInfo().id);
					m_deletingSave = 0;
				}
				else if (option == 2) {
					m_deletingSave = 0;
				}
			}
			return false;
		}
		else if (m_starterPickmenu) {
			if (m_starterPickmenu->chosePokemon(mousePos)) {
				m_player.activePokemon[0] = m_starterPickmenu->chosePokemon(mousePos);
				delete m_starterPickmenu;
				m_starterPickmenu = nullptr;
				return true;
			}
			else if (m_starterPickmenu->clickedBackButton(mousePos)) {
				delete m_starterPickmenu;
				m_starterPickmenu = nullptr;
				GameUI::music.openFromFile("sound/music/titlescreen.mp3");
				GameUI::music.play();
			}
			return false;
		}
		if (m_quitButton.getGlobalBounds().contains(mousePos))
			exit(0);

		for (int i{ 0 }; i < m_saveInfos.size(); i++) {
			if (m_saveInfos[i].contains(mousePos)) {
				m_chosenSave = m_saveInfos[i].getSaveInfo().id;

				if (m_saveInfos[i].getSaveInfo().pokemonAmount > 0) {
					m_player.loadSave(m_saveInfos[i].getSaveInfo().id);
					return true;
				}
				else {
					if (!m_starterPickmenu) {
						m_starterPickmenu = new StarterPickMenu();
						m_player.createSave(m_saveInfos[i].getSaveInfo());
					}
				}
			}
		}
		for (int i{ 0 }; i < m_deleteButtons.size(); i++) {
			if (m_deleteButtons[i].getGlobalBounds().contains(mousePos)) {
				GameUI::clickSound.play();
				m_deletingSave = i + 1;
			}
		}
		if (m_optionsButton.getGlobalBounds().contains(mousePos))
			m_optionsMenu = new OptionsMenu(m_window,m_view);
	}
	return false;
}

void SaveMenu::draw(sf::RenderWindow& window)
{
	if (m_optionsMenu) {
		m_optionsMenu->draw(window);
		return;
	}

	if (m_starterPickmenu) {
		m_starterPickmenu->draw(window);
		return;
	}
				
	for (int i{ 0 }; i < m_saveInfos.size(); i++) {
		m_saveInfos[i].draw(window);
	}
	for (int i{ 0 }; i < m_deleteButtons.size(); i++) {
		window.draw(m_deleteButtons[i]);
	}

	window.draw(m_optionsButton);
	window.draw(m_optionsText);
	window.draw(m_quitButton);
	window.draw(m_quitText);

	if (m_deletingSave)
		m_deleteSaveMenu.draw(window);
}
