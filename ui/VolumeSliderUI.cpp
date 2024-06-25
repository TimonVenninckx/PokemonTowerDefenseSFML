#include "VolumeSliderUI.h"
#include "GameUI.h"


VolumeSliderUI::VolumeSliderUI(float& bindVolume,sf::Music* music, sf::Vector2f position, sf::Vector2f size)
	: m_volume{ bindVolume }
	, m_music{ music }
{
	m_bar.setPosition(position);
	m_bar.setSize(size);
	m_bar.setCornerPointCount(10);
	m_bar.setCornersRadius(15.f);
	m_bar.setFillColor(GameUI::backgroundColor);

	m_button.setSize({ size.x / 7.f, size.y });
	m_button.setOrigin(0.f, m_button.getSize().y / 2.f);
	m_button.setPosition(position + sf::Vector2f{ 0.f,size.y / 2.f });
	m_button.setCornerPointCount(10);
	m_button.setCornersRadius(15.f);
	m_button.setFillColor(GameUI::buttonColor);
}

VolumeSliderUI::VolumeSliderUI(float& bindVolume, sf::Sound* sound, sf::Vector2f position, sf::Vector2f size)
	: m_volume{ bindVolume }
	, m_sound{ sound }
{
	m_bar.setPosition(position);
	m_bar.setSize(size);
	m_bar.setCornerPointCount(10);
	m_bar.setCornersRadius(15.f);
	m_bar.setFillColor(GameUI::backgroundColor);

	m_button.setSize({ size.x / 7.f, size.y });
	m_button.setOrigin(0.f, m_button.getSize().y / 2.f);
	m_button.setPosition(position + sf::Vector2f{ 0.f,size.y / 2.f });
	m_button.setCornerPointCount(10);
	m_button.setCornersRadius(15.f);
	m_button.setFillColor(GameUI::buttonColor);
}

void VolumeSliderUI::reload()
{
	m_holding = false;
	float position = m_volume / 100.f * (m_bar.getSize().x - m_button.getSize().x) + m_bar.getPosition().x;
	m_button.setPosition(position, m_button.getPosition().y);
}

bool VolumeSliderUI::changed(sf::Vector2f mousePos, sf::Event& event)
{
	if (!m_holding) {
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if (m_bar.getGlobalBounds().contains(mousePos)) {
				GameUI::clickSound.play();
				float position = std::clamp(mousePos.x - m_button.getSize().x / 2.f, m_bar.getPosition().x, m_bar.getPosition().x + m_bar.getSize().x - m_button.getSize().x);
				m_button.setPosition(position, m_button.getPosition().y);
				m_holding = true;
				updateVolume();
			}
		}
		if(!m_holding)
			return false;
	}
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		m_holding = false;
		return false;
	}


	if (m_bar.getGlobalBounds().contains(mousePos)) {
		float position = std::clamp(mousePos.x - m_button.getSize().x / 2.f, m_bar.getPosition().x, m_bar.getPosition().x + m_bar.getSize().x - m_button.getSize().x);
		m_button.setPosition(position, m_button.getPosition().y);
		updateVolume();
	}
	return true;
}

void VolumeSliderUI::updateVolume()
{
	m_volume = (m_button.getPosition().x - m_bar.getPosition().x) / (m_bar.getSize().x - m_button.getSize().x) * 100.f; // volume goes from 0-100
	if (m_music)
		m_music->setVolume(m_volume);
	if (m_sound)
		m_sound->setVolume(m_volume);
}

void VolumeSliderUI::draw(sf::RenderWindow& window)
{
	window.draw(m_bar);
	window.draw(m_button);
}

