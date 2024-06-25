#include "PauseMenuUI.h"
#include "GameUI.h"

PauseMenuUI::PauseMenuUI()
	: m_sfxVolumeSlider{ GameUI::sfxVolume,&GameUI::clickSound,{735.f,500.f},{450.f,40.f} }
	, m_musicVolumeSlider{ GameUI::musicVolume,&GameUI::music,{735.f,600.f},{450.f,40.f} }
{
	m_exitButton.setFillColor(GameUI::buttonColor);
	m_exitButton.setSize({ 450.f, 150.f });
	m_exitButton.setCornerPointCount(5);
	m_exitButton.setCornersRadius(15.f);
	m_exitButton.setOutlineThickness(-5.f);
	m_exitButton.setOutlineColor(GameUI::backgroundColor);
	m_exitButton.setOrigin(m_exitButton.getGlobalBounds().getSize() / 2.f);
	m_exitButton.setPosition(960.f, 750.f);
	m_continueButton.setFillColor(GameUI::buttonColor);
	m_continueButton.setSize({ 450.f, 150.f });
	m_continueButton.setCornerPointCount(5);
	m_continueButton.setCornersRadius(15.f);
	m_continueButton.setOutlineThickness(-5.f);
	m_continueButton.setOutlineColor(GameUI::backgroundColor);
	m_continueButton.setOrigin(m_continueButton.getGlobalBounds().getSize() / 2.f);
	m_continueButton.setPosition(960.f, 350.f);

	m_exitText.setFont(GameUI::font);
	m_exitText.setCharacterSize(80);
	m_exitText.setString("Exit");
	m_exitText.setOrigin(m_exitText.getGlobalBounds().getSize().x / 2.f, m_exitText.getGlobalBounds().getSize().y / 1.4f);
	m_exitText.setPosition(960.f, 750.f);

	m_continueText.setFont(GameUI::font);
	m_continueText.setCharacterSize(80);
	m_continueText.setString("Continue");
	m_continueText.setOrigin(m_continueText.getGlobalBounds().getSize().x / 2.f,m_continueText.getGlobalBounds().getSize().y / 1.4f);
	m_continueText.setPosition(960.f, 350.f);


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


}

bool PauseMenuUI::isPaused()
{
	return m_paused;
}

void PauseMenuUI::setPaused(bool pause)
{
	m_musicVolumeSlider.reload();
	m_sfxVolumeSlider.reload();
	m_paused = pause;
}


bool PauseMenuUI::exitGame(sf::Vector2f mousePos, sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (m_exitButton.getGlobalBounds().contains(mousePos)) {
			GameUI::clickSound.play();
			return true;
		}
		if (m_continueButton.getGlobalBounds().contains(mousePos)) {
			m_paused = false;
			GameUI::clickSound.play();
			return false;
		}
	}
	m_musicVolumeSlider.changed(mousePos, event);
	m_sfxVolumeSlider.changed(mousePos,event);

	return false;
}

void PauseMenuUI::draw(sf::RenderWindow& window)
{
	window.draw(m_exitButton);
	window.draw(m_continueButton);
	window.draw(m_exitText);
	window.draw(m_continueText);
	window.draw(m_sfxText);
	window.draw(m_musicText);
	m_sfxVolumeSlider.draw(window);
	m_musicVolumeSlider.draw(window);
}
