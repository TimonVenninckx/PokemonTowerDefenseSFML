#include "LevelThumbnailUI.h"
#include "GameUI.h"
#include <iostream>

LevelThumbnailUI::LevelThumbnailUI(LevelInfo info, sf::Vector2f position, sf::Vector2f size)
{
	m_levelInfo = info;

	m_box.setPosition(position);
	m_box.setSize(size);
	m_box.setCornerPointCount(5);
	m_box.setCornersRadius(25);
	m_box.setFillColor(GameUI::buttonColor);

	m_levelName.setFont(GameUI::font);
	m_levelName.setCharacterSize(static_cast<unsigned int>(size.y / 10.f));
	m_levelName.setString(m_levelInfo.name);
	m_levelName.setPosition(position.x + size.x / 20.f,position.y + 5.f);

	m_texture.loadFromFile("textures/thumbnails/" + m_levelInfo.thumbnailLocation);
	m_thumbnail.setTexture(m_texture);
	m_thumbnail.setOrigin(m_thumbnail.getLocalBounds().getSize() / 2.f);
	size.y -= 30.f;
	m_thumbnail.setPosition(position + size / 2.f + sf::Vector2f{ 0.f,30.f });
	m_thumbnail.setScale((size.x - size.x / 10.f) / m_texture.getSize().x,(size.y - size.y / 10.f) / m_texture.getSize().y);

	m_lockedSprite.setTexture(GameUI::levelLockedTexture);
	m_lockedSprite.setPosition(m_thumbnail.getPosition());
	m_lockedSprite.setOrigin(m_lockedSprite.getGlobalBounds().getSize() / 2.f);
}

LevelThumbnailUI::~LevelThumbnailUI()
{
	std::cout << "already deleting stuff M????\n";
}


LevelInfo* LevelThumbnailUI::getLevel()
{
	return &m_levelInfo;
}


bool LevelThumbnailUI::contains(sf::Vector2f mousePos)
{
	return m_box.getGlobalBounds().contains(mousePos);
}

bool LevelThumbnailUI::isLocked()
{
	return m_locked;
}

void LevelThumbnailUI::lock(bool locked)
{
	m_locked = locked;
	if (locked)
		m_box.setFillColor(GameUI::backgroundColor);
	else
		m_box.setFillColor(GameUI::buttonColor);
}



void LevelThumbnailUI::draw(sf::RenderWindow& window)
{
	window.draw(m_box);

	window.draw(m_thumbnail);
	window.draw(m_levelName);

	if (m_locked)
		window.draw(m_lockedSprite);
}
