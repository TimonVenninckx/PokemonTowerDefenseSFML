#include "PokemonPickMenu.h"
#include "GameUI.h"
#include "../GameMath.h"
#include <algorithm>
#include <iostream>

PokemonPickMenu::PokemonPickMenu(Player& player, sf::View& view, sf::RenderWindow& window)
	: m_player{ player }
	, m_activePokemonIcons{
		PokemonPickIcon { nullptr,sf::Vector2{100.f,100.f} },
		PokemonPickIcon { nullptr,sf::Vector2{300.f,100.f} },
		PokemonPickIcon { nullptr,sf::Vector2{500.f,100.f} },
		PokemonPickIcon { nullptr,sf::Vector2{700.f,100.f} },
		PokemonPickIcon { nullptr,sf::Vector2{900.f,100.f} },
		PokemonPickIcon { nullptr,sf::Vector2{1100.f,100.f} } }
	, m_viewMenu{ player }
	, m_view{ view }
	, m_window{ window }
{

	m_doneButton.setCornerPointCount(5);
	m_doneButton.setCornersRadius(25);
	m_doneButton.setSize({ 340.f, 140.f });
	m_doneButton.setOrigin(m_doneButton.getGlobalBounds().getSize() / 2.f);
	m_doneButton.setPosition(1730.f, 90.f);
	m_doneButton.setFillColor(GameUI::buttonColor);

	m_doneButtonText.setFont(GameUI::font);
	m_doneButtonText.setCharacterSize(100);
	m_doneButtonText.setString("DONE");
	m_doneButtonText.setOrigin(m_doneButtonText.getGlobalBounds().getSize().x / 2.f,m_doneButtonText.getGlobalBounds().getSize().y / 1.4f);
	m_doneButtonText.setPosition(1730.f, 90.f);


	m_backButton.setCornerPointCount(5);
	m_backButton.setCornersRadius(25);
	m_backButton.setSize({ 240.f, 100.f });
	m_backButton.setOrigin(m_backButton.getGlobalBounds().getSize() / 2.f);
	m_backButton.setPosition(1420.f, 70.f);
	m_backButton.setFillColor(GameUI::buttonColor);

	m_backButtonText.setFont(GameUI::font);
	m_backButtonText.setCharacterSize(70);
	m_backButtonText.setString("BACK");
	m_backButtonText.setOrigin(m_backButtonText.getGlobalBounds().getSize().x / 2.f, m_backButtonText.getGlobalBounds().getSize().y / 1.4f);
	m_backButtonText.setPosition(1420.f, 70.f);

	m_topBox.setFillColor(sf::Color(50,50,50,255));
	m_topBox.setSize({ 4000.f, 1000.f });
	m_topBox.setOrigin(2000.f, 1000.f);
	m_topBox.setPosition(960.f, 200.f);


	m_scrollBar.setCornerPointCount(5);
	m_scrollBar.setCornersRadius(15);
	m_scrollBar.setSize({ 40.f, 140.f });
	m_scrollBar.setOrigin(m_scrollBar.getGlobalBounds().getSize() / 2.f);
	m_scrollBar.setPosition(1890.f, 275.f);
	m_scrollBar.setFillColor(GameUI::buttonColor);
	m_scrollBar.setOutlineColor(GameUI::backgroundColor);
	m_scrollBar.setOutlineThickness(3.f);


	m_scrollBarview.reset({ 0.f,-350.f,1920.f,1080.f });

}

void PokemonPickMenu::reload()
{
	m_reservePokemonIcons.clear();
	for (int i{ 0 }; i < m_player.reservePokemon.size(); i++) {
		PokemonPickIcon icon{ m_player.reservePokemon[i],sf::Vector2f{i % 9 * 200.f + 100.f,(200.f * (i / 9))} };
		m_reservePokemonIcons.emplace_back(icon);
	}
	for (int i{ 0 }; i < m_player.activePokemon.size(); i++) {
		m_activePokemonIcons[i].setPokemon(m_player.activePokemon[i]);
	}

	GameUI::music.openFromFile("sound/music/pokemoncenter.mp3");
	GameUI::music.play();

	m_draggingIcon = nullptr;
	m_dragging = false;
	recalculateScrollWindow();
}

void PokemonPickMenu::recalculateScrollWindow()
{
	//275 , 1050 scroll minheight and maxheight
	// it just works couple magic numbers
	float scrollbarheight = (m_scrollBar.getPosition().y - 375.f) / (1050.f - 325.f); // get number between 0.0 and 1.0
	scrollbarheight *= (m_reservePokemonIcons.back().getPosition().y - 800.f); // 4 rows of 200.f height

	if (m_window.getSize().x / 16.f > m_window.getSize().y / 9.f) {
		float normalWidth = m_window.getSize().x - ((m_window.getSize().y / 9.f) * 16.f);
		m_scrollBarview.reset(sf::FloatRect{ -normalWidth / 2.f, scrollbarheight,  1920 + normalWidth, 1080 });
	}
	else {
		float normalHeight = m_window.getSize().y - ((m_window.getSize().x / 16.f) * 9.f);
		m_scrollBarview.reset(sf::FloatRect{ 0, -normalHeight / 2.f + scrollbarheight, 1920, 1080 + normalHeight });
	}
}


bool PokemonPickMenu::doneUpdate(sf::Event& event, sf::Vector2f mousePos, float delta)
{
	
	if (event.type == sf::Event::Resized) {
		recalculateScrollWindow();
	}
	if (m_viewMenu.isActive()) { 
		if (m_viewMenu.doneUpdate(event, mousePos)) {
			if (m_viewMenu.released()) { // if the pokemon has been deleted / released
				delete m_viewMenu.getPokemon();
				for (auto& icon : m_activePokemonIcons) {
					if (icon.getPokemon() == m_viewMenu.getPokemon()) {
						icon.setPokemon(nullptr);
					}
				}
				for (auto& icon : m_reservePokemonIcons) {
					if (icon.getPokemon() == m_viewMenu.getPokemon()) {
						icon.setPokemon(nullptr);
					}
				}
			}
		}
		return false;
	}
	if (event.type == sf::Event::MouseWheelMoved) {
		float height = std::clamp(-event.mouseWheel.delta * delta * 10000.f + m_scrollBar.getPosition().y, 275.f, 1050.f);
		m_scrollBar.setPosition(m_scrollBar.getPosition().x, height);
		recalculateScrollWindow();
	}

	if (m_scrolling) {
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			m_scrolling = false;
		}
		else {
			//275 , 1050
			//std::cout << "scrollbar pos: " << m_scrollBar.getPosition().y << '\n';
			float height = std::clamp(m_scrollOffset + mousePos.y, 275.f, 1050.f);
			m_scrollBar.setPosition(m_scrollBar.getPosition().x, height);
			recalculateScrollWindow();
			return false;
		}
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && 
		m_scrollBar.getGlobalBounds().contains(mousePos)) {
		m_scrolling = true;
		m_scrollOffset = m_scrollBar.getPosition().y - mousePos.y;
		return false;
	}


	sf::Vector2f reserveMousePos{ m_window.mapPixelToCoords(m_window.mapCoordsToPixel(mousePos), m_scrollBarview) };

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (m_doneButton.getGlobalBounds().contains(mousePos)) {
			GameUI::clickSound.play();
			GameUI::music.stop();
			return true;
		}
		else if (m_backButton.getGlobalBounds().contains(mousePos)) {
			GameUI::clickSound.play();
			GameUI::music.stop();
			return true;
		}
		if (!m_draggingIcon) {
			for (PokemonPickIcon& icon : m_reservePokemonIcons) {
				if (icon.contains(reserveMousePos)) {
					m_dragginActivePokemon = false;
					m_draggingIcon = &icon;
					break;
				}
			}
		}
		if(!m_draggingIcon){ // if we haven't found an icon yet
			for (PokemonPickIcon& icon : m_activePokemonIcons) {
				if (icon.contains(mousePos) && icon.getPokemon()) { // pokemon is valid
					m_dragginActivePokemon = true;
					m_draggingIcon = &icon;
					break;
				}
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { // release pokemon
		if (m_draggingIcon) {
			if (m_topBox.getGlobalBounds().contains(mousePos)) {
				for (PokemonPickIcon& icon : m_activePokemonIcons) {
					if (icon.contains(mousePos)) {
						if (m_dragging) {
							PlayerPokemon* temp = icon.getPokemon();
							icon.setPokemon(m_draggingIcon->getPokemon());
							m_draggingIcon->setPokemon(temp);
							break;
						}
						else {
							GameUI::clickSound.play();
							m_viewMenu.setPokemon(m_draggingIcon->getPokemon());
							break;
						}
					}
				}
			}
			else {
				// reserve pokemons use different view for mousepos
				for (PokemonPickIcon& icon : m_reservePokemonIcons) {
					if (icon.contains(reserveMousePos)) {
						if (m_dragging) {
							PlayerPokemon* temp = icon.getPokemon();
							icon.setPokemon(m_draggingIcon->getPokemon());
							m_draggingIcon->setPokemon(temp);
							break;
						}
						else {
							GameUI::clickSound.play();
							m_viewMenu.setPokemon(m_draggingIcon->getPokemon());
							break;
						}
					}
				}
			}
			m_draggingIcon->stopDrag();
			m_draggingIcon = nullptr;
			m_dragging = false;
		}
	}
	if (m_draggingIcon) {
		if(m_dragging)
			m_draggingIcon->updateDrag(mousePos);
		else {
			if (m_dragginActivePokemon && GameMath::distance(m_draggingIcon->getPosition(), mousePos) > 110.f) {
				m_dragging = true;
				m_draggingIcon->updateDrag(mousePos);
			}
			if (!m_dragginActivePokemon && GameMath::distance(m_draggingIcon->getPosition(), reserveMousePos) > 110.f) {
				m_dragging = true;
				m_draggingIcon->updateDrag(mousePos);
			}
		}
	}

	return false;
}

void PokemonPickMenu::draw(sf::RenderWindow& window)
{
	if (m_viewMenu.isActive()) {
		m_viewMenu.draw(window);
		return;
	}


	window.setView(m_scrollBarview);
	for (PokemonPickIcon& icon : m_reservePokemonIcons) {
		icon.drawBox(window);
		if (m_dragging && icon.getPokemon() == m_draggingIcon->getPokemon())
			continue;
		icon.drawSprite(window);
	}
	window.setView(m_view);



	window.draw(m_topBox);
	window.draw(m_doneButton);
	window.draw(m_doneButtonText);
	window.draw(m_backButton);
	window.draw(m_backButtonText);
	window.draw(m_scrollBar);

	for (PokemonPickIcon& icon : m_activePokemonIcons) {
		icon.drawBox(window);
		icon.drawSprite(window);
	}

	
	if (m_dragging)
		m_draggingIcon->drawSprite(window); // draw it twice because we always want it at top layer;
}

bool PokemonPickMenu::startLevel(sf::Vector2f mousePos)
{
	for (int i{ 0 }; i < m_player.activePokemon.size(); i++) {
		m_player.activePokemon[i] = m_activePokemonIcons[i].getPokemon();
	}
	for (int i{ 0 }; i < m_player.reservePokemon.size(); i++) {
		m_player.reservePokemon[i] = m_reservePokemonIcons.size() > i ? m_reservePokemonIcons[i].getPokemon() : nullptr;
	}
	return m_doneButton.getGlobalBounds().contains(mousePos);
}
