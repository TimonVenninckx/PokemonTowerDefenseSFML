#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "VolumeSliderUI.h"


class OptionsMenu
{

	sf::RoundedRectangleShape m_backButton;
	sf::Text				  m_backText;


	VolumeSliderUI m_musicVolumeSlider;
	VolumeSliderUI m_sfxVolumeSlider;

	sf::Text   m_musicText;
	sf::Text   m_sfxText;
	
	sf::RenderWindow& m_window;
	sf::View&		  m_view;
	sf::Text				  m_fullScreenText;
	sf::RoundedRectangleShape m_fullScreenButton;
	bool lastFrameFullScreen;

public:
	OptionsMenu(sf::RenderWindow& window, sf::View& view);
	bool exitMenu(sf::Vector2f mousePos, sf::Event& event);
	void draw(sf::RenderWindow& window);


};


#endif
