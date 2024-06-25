#include "PokemonPickIcon.h"
#include "GameUI.h"

PokemonPickIcon::PokemonPickIcon(PlayerPokemon* pokemon,sf::Vector2f position)
{
	m_box.setSize({150.f, 150.f});
	m_box.setCornerPointCount(5);
	m_box.setCornersRadius(25);
	m_box.setFillColor(GameUI::backgroundColor);
	m_box.setOrigin(m_box.getGlobalBounds().getSize() / 2.f);
	m_box.setOutlineThickness(2.f);
	m_box.setOutlineColor(GameUI::buttonColor);

	m_text.setFont(GameUI::font);
	m_text.setCharacterSize(34);
	m_text.setOutlineThickness(2);

	setPokemon(pokemon);
	setPosition(position);
}

sf::Vector2f PokemonPickIcon::getPosition()
{
	return m_box.getPosition();
}

void PokemonPickIcon::setPosition(sf::Vector2f position)
{
	m_box.setPosition(position);
	m_text.setPosition(position + sf::Vector2f{ 0.f,50.f });
	m_sprite.setPosition(position);
}

void PokemonPickIcon::updateDrag(sf::Vector2f position) {
	m_sprite.setPosition(position);
	m_text.setPosition(position + sf::Vector2f{ 0.f,50.f });
}

void PokemonPickIcon::stopDrag()
{
	m_sprite.setPosition(m_box.getPosition());
	m_text.setPosition(m_box.getPosition() + sf::Vector2f{ 0.f,50.f });
}


void PokemonPickIcon::drawBox(sf::RenderWindow& window)
{
	window.draw(m_box);
}


void PokemonPickIcon::drawSprite(sf::RenderWindow& window)
{
	window.draw(m_sprite);
	window.draw(m_text);
}


bool PokemonPickIcon::contains(sf::Vector2f mousePos)
{
	return m_box.getGlobalBounds().contains(mousePos);
}

void PokemonPickIcon::setPokemon(PlayerPokemon* pokemon)
{
	m_pokemon = pokemon;
	if (pokemon) {
		m_text.setString("LVL" + std::to_string(m_pokemon->getStats().level));
		m_text.setOrigin(m_text.getLocalBounds().getSize().x / 2.f, m_text.getGlobalBounds().getSize().y / 1.4f);

		m_sprite.setTexture(*m_pokemon->getTexture());
		m_sprite.setTextureRect({ 0, 0,static_cast<int>(m_pokemon->getTexture()->getSize().x) / 4 ,static_cast<int>(m_pokemon->getTexture()->getSize().y) / 4 });
		m_sprite.setOrigin(m_pokemon->getTexture()->getSize().x / 4 / 2, m_pokemon->getTexture()->getSize().y / 4 / 2);
		m_sprite.setScale(128.f / (m_pokemon->getTexture()->getSize().x / 4), 128.f / (m_pokemon->getTexture()->getSize().y / 4));
		
		sf::Color color{ PokemonStats::getTypeColor(m_pokemon->getStats().type1) };
		//color.a = 100;
		m_box.setOutlineColor(color);
		color.a = 100;
		m_box.setFillColor(color);
	}
	else {
		m_sprite.setTextureRect({0, 0, 0, 0 });
		m_text.setString("");
		m_box.setFillColor(GameUI::backgroundColor);
		m_box.setOutlineColor(sf::Color::Black);
		m_box.setFillColor(GameUI::backgroundColor);
	}
}

PlayerPokemon* PokemonPickIcon::getPokemon()const
{
	return m_pokemon;
}
