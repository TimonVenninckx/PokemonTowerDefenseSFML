#include "OptionsMenu.h"
#include "GameUI.h"

OptionsMenu::OptionsMenu(sf::RenderWindow& window, sf::View& view)
	: m_window{ window }
	, m_view{ view }
	, m_sfxVolumeSlider{ GameUI::sfxVolume,&GameUI::clickSound,{735.f,500.f},{450.f,40.f} }
	, m_musicVolumeSlider{ GameUI::musicVolume,&GameUI::music,{735.f,600.f},{450.f,40.f} }
{

	m_backButton.setFillColor(GameUI::buttonColor);
	m_backButton.setSize({ 450.f, 150.f });
	m_backButton.setCornerPointCount(5);
	m_backButton.setCornersRadius(15.f);
	m_backButton.setOutlineThickness(-5.f);
	m_backButton.setOutlineColor(GameUI::backgroundColor);
	m_backButton.setOrigin(m_backButton.getGlobalBounds().getSize() / 2.f);
	m_backButton.setPosition(300.f,150.f);

	m_backText.setFont(GameUI::font);
	m_backText.setCharacterSize(80);
	m_backText.setString("Back");
	m_backText.setOrigin(m_backText.getGlobalBounds().getSize().x / 2.f, m_backText.getGlobalBounds().getSize().y / 1.4f);
	m_backText.setPosition(m_backButton.getPosition());


	m_sfxText.setFont(GameUI::font);
	m_sfxText.setCharacterSize(50);
	m_sfxText.setString("SFX");
	m_sfxText.setOrigin(m_sfxText.getGlobalBounds().getSize().x / 2.f, m_sfxText.getGlobalBounds().getSize().y / 1.4f);
	m_sfxText.setPosition(960.f, 470.f);
	m_sfxText.setOutlineThickness(4.f);


	m_musicText.setFont(GameUI::font);
	m_musicText.setCharacterSize(50);
	m_musicText.setString("Music");
	m_musicText.setOrigin(m_musicText.getGlobalBounds().getSize().x / 2.f, m_musicText.getGlobalBounds().getSize().y / 1.4f);
	m_musicText.setPosition(960.f, 570.f);
	m_musicText.setOutlineThickness(4.f);


	m_fullScreenText.setFont(GameUI::font);
	m_fullScreenText.setCharacterSize(50);
	m_fullScreenText.setString("Fullscreen");
	m_fullScreenText.setOrigin(m_fullScreenText.getGlobalBounds().getSize().x / 2.f, m_musicText.getGlobalBounds().getSize().y / 1.4f);
	m_fullScreenText.setPosition(960.f, 350.f);
	m_fullScreenText.setOutlineThickness(4.f);
	
	m_fullScreenButton.setCornerPointCount(5);
	m_fullScreenButton.setCornersRadius(15.f);
	m_fullScreenButton.setSize({ 50.f, 50.f });
	m_fullScreenButton.setPosition(960.f, 405.f);
	m_fullScreenButton.setOrigin(-m_fullScreenText.getGlobalBounds().getSize().x / 2.f, m_fullScreenButton.getGlobalBounds().getSize().y / 2.f);
	m_fullScreenButton.setOrigin(m_fullScreenButton.getGlobalBounds().getSize() / 2.f);
	m_fullScreenButton.setOutlineThickness(5.f);
	m_fullScreenButton.setOutlineColor(GameUI::buttonColor);
	m_fullScreenButton.setFillColor(sf::Color(0, 0, 0, 0));



	m_musicVolumeSlider.reload();
	m_sfxVolumeSlider.reload();
}

bool OptionsMenu::exitMenu(sf::Vector2f mousePos, sf::Event& event)
{
	m_musicVolumeSlider.changed(mousePos, event);
	m_sfxVolumeSlider.changed(mousePos, event);

	if (lastFrameFullScreen != GameUI::isFullScreen) {
		lastFrameFullScreen = GameUI::isFullScreen;
		if (GameUI::isFullScreen)
			m_fullScreenButton.setFillColor(GameUI::buttonColor);
		else
			m_fullScreenButton.setFillColor(sf::Color(0, 0, 0, 0));
	}

	

	if (event.type == sf::Event::MouseButtonPressed) {
		if (m_fullScreenButton.getGlobalBounds().contains(mousePos)) {
			if (GameUI::isFullScreen) {
				GameUI::isFullScreen = false;
				m_window.create(sf::VideoMode(1080, 690), "Tower Defense"); // -30 in the Y because of bar???
				m_window.setView(m_view);
			}
			else {
				GameUI::isFullScreen = true;
				m_window.create(sf::VideoMode::getFullscreenModes()[0], "Tower Defense", sf::Style::Fullscreen);
				m_window.setView(m_view);
			}
		}

		if (m_backButton.getGlobalBounds().contains(mousePos))
			return true;
	}

	return false;
}

void OptionsMenu::draw(sf::RenderWindow& window)
{
	window.draw(m_backButton);
	window.draw(m_backText);

	window.draw(m_sfxText);
	window.draw(m_musicText);
	window.draw(m_fullScreenText);
	window.draw(m_fullScreenButton);
	
	m_sfxVolumeSlider.draw(window);
	m_musicVolumeSlider.draw(window);
}
