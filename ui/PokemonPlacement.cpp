#include "PokemonPlacement.h"
#include "../PlayerPokemon.h"


PlayerPokemon* PokemonPlacement::getPokemon()
{
    return m_pokemon;
}

void PokemonPlacement::setPokemon(PlayerPokemon* pokemon) {
    m_pokemon = pokemon;
}


sf::Vector2f PokemonPlacement::getPosition()
{
    return sprite.getPosition();
}

void PokemonPlacement::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
    if (m_pokemon) {
        m_pokemon->setPosition(position);
    }
}

void PokemonPlacement::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

bool PokemonPlacement::contains(sf::Vector2f position)
{
    if (sprite.getGlobalBounds().contains(position))
        return true;
    return false;
}
