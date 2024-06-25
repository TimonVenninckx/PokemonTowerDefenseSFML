#ifndef PauseMenuUI_H
#define PauseMenuUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "RoundedRectangleShape.h"
#include "VolumeSliderUI.h"

class PauseMenuUI
{

	sf::RoundedRectangleShape m_exitButton;
	sf::Text				  m_exitText;
	sf::RoundedRectangleShape m_continueButton;
	sf::Text				  m_continueText;


	VolumeSliderUI m_musicVolumeSlider;
	VolumeSliderUI m_sfxVolumeSlider;

	sf::Text   m_musicText;
	sf::Text   m_sfxText;

	bool m_paused{ false };
public:
	PauseMenuUI();
	bool isPaused();
	void setPaused(bool pause);
	bool exitGame(sf::Vector2f mousePos,sf::Event& event);
	void draw(sf::RenderWindow& window);

};


#endif
