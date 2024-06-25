#ifndef VOLUMESLIDERUI_H
#define VOLUMESLIDERUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "RoundedRectangleShape.h"


class VolumeSliderUI
{
	float& m_volume;
	bool   m_holding;
	sf::RoundedRectangleShape m_bar;
	sf::RoundedRectangleShape m_button;
	sf::Music* m_music{ nullptr };
	sf::Sound* m_sound{ nullptr };

public:
	VolumeSliderUI(float& bindVolume, sf::Music* music, sf::Vector2f position, sf::Vector2f size);
	VolumeSliderUI(float& bindVolume, sf::Sound* sound, sf::Vector2f position, sf::Vector2f size);
	void reload();
	bool changed(sf::Vector2f mousePos, sf::Event& event);
	void updateVolume();
	void draw(sf::RenderWindow& window);
};


#endif
