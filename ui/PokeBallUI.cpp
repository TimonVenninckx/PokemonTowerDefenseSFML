#include "PokeBallUI.h"
#include "../GameMath.h"

PokeBallUI::PokeBallUI()
{
    m_texture.loadFromFile("textures/pokeball.png");

    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect({ static_cast<int>(m_texture.getSize().x / 2), 0, static_cast<int>(m_texture.getSize().x / 2), static_cast<int>(m_texture.getSize().y) });
    m_sprite.setOrigin(m_sprite.getGlobalBounds().getSize() / 2.f);
    m_sprite.setPosition(1750.f, 950.f);

}

void PokeBallUI::setActive(bool active)
{
    if (!active) {
        m_catchingPokemon = nullptr;
        m_sprite.setPosition(1750.f, 950.f);
        m_sprite.setTextureRect({ static_cast<int>(m_texture.getSize().x / 2), 0, static_cast<int>(m_texture.getSize().x / 2), static_cast<int>(m_texture.getSize().y) });
    }
    m_active = active;
}

bool PokeBallUI::isActive()
{
    return m_active;
}

Pokemon* PokeBallUI::tryCatch()
{
    return m_catchingPokemon;
}

void PokeBallUI::setPokemon(LinkedList<Pokemon>* pokemonHead)
{
    m_pokemonHead = pokemonHead;
}

void PokeBallUI::setPosition(sf::Vector2f mousePos, sf::Vector2f uiPos)
{
    bool updated{ false };
    if (m_catchingPokemon){
        if (GameMath::distance(m_catchingPokemon->getPosition(), mousePos) > 25.f) {
            m_catchingPokemon = nullptr;
            updated = true;
        }
    }
    for (LinkedList<Pokemon>* it{ m_pokemonHead->next }; it; it = it->next) {
        if (GameMath::distance(it->value->getPosition(), mousePos) > 25.f)
            continue;
        if (!it->value->canCatch())
            continue;
        if (!m_catchingPokemon) {
            m_catchingPokemon = it->value;
            updated = true;
            m_sprite.setTextureRect({ 0, 0, static_cast<int>(m_texture.getSize().x / 2), static_cast<int>(m_texture.getSize().y) });
        }
        else {
            if (GameMath::distance(it->value->getPosition(), mousePos) < GameMath::distance(m_catchingPokemon->getPosition(), mousePos)) {
                m_catchingPokemon = it->value;
                updated = true;
            }
        }
    }
    if (updated && !m_catchingPokemon) {
        m_sprite.setTextureRect({ static_cast<int>(m_texture.getSize().x / 2), 0, static_cast<int>(m_texture.getSize().x / 2), static_cast<int>(m_texture.getSize().y) });
    }

    m_sprite.setPosition(uiPos);
}

bool PokeBallUI::contains(sf::Vector2f mousePos)
{
    return m_sprite.getGlobalBounds().contains(mousePos);
}

void PokeBallUI::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}


void PokeBallUI::drawPokemon(sf::RenderWindow& window)
{
    if (m_active && m_catchingPokemon) {
        m_catchingPokemon->draw(window);
    }
}