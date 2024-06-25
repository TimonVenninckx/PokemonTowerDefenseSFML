#include "EvolutionUI.h"
#include "../sqlite/Database.h"
#include "../SpriteLoader.h"
#include "GameUI.h"

float maxEvolutionTime = 6.f;

EvolutionUI::EvolutionUI(sf::Vector2f position, sf::Vector2f size)
{
    m_position = position;
    m_size = size;

    m_box.setSize(size);
    m_box.setPosition(position);
    m_box.setCornerPointCount(12);
    m_box.setCornersRadius(20);
    m_box.setFillColor(sf::Color(30, 30, 30, 255));


    m_cancelButton.setSize({ size.x / 2.f, size.y / 6.f });
    m_cancelButton.setOrigin(m_cancelButton.getSize() / 2.f);
    m_cancelButton.setPosition(position + sf::Vector2f{size.x / 2.f, size.y / 6.f * 5.f});
    m_cancelButton.setCornerPointCount(12);
    m_cancelButton.setCornersRadius(20);
    m_cancelButton.setFillColor(sf::Color(255, 30, 30, 255));

    m_cancelText.setFont(GameUI::font);
    m_cancelText.setString("Cancel Evolution");
    m_cancelText.setOrigin(m_cancelText.getLocalBounds().getSize().x / 2.f,m_cancelText.getLocalBounds().getSize().y / 1.4f);
    m_cancelText.setPosition(position + sf::Vector2f{ size.x / 2.f, size.y / 6.f * 5.f });


    m_evolutionText.setFont(GameUI::font);
    m_evolutionText.setPosition(position + sf::Vector2f{ size.x / 2.f, size.y / 6.f * 5.f });

    m_song.openFromFile("sound/evolution.mp3");
}

bool EvolutionUI::isEvolving() {
    return m_evolving;
}


bool EvolutionUI::startEvolution(PlayerPokemon* pokemon)
{
    m_pokemon = pokemon;

    int id = m_pokemon->getStats().id;
    int level = m_pokemon->getStats().level;
    m_evolutionId = Database::canEvolve(id, level);
    if(m_evolutionId == 0)
        return false;


    m_currentPokemon.setScale({ 4.f,4.f });
    m_evolvedPokemon.setScale({ 4.f,4.f });

    sf::Texture* currentPokemonTexture = SpriteLoader::getPokemonSprite(id);
    m_currentPokemon.setTexture(*currentPokemonTexture);
    int currentPokemonSpriteSize = currentPokemonTexture->getSize().x / 4;
    m_currentPokemon.setTextureRect({ 0,0,currentPokemonSpriteSize, currentPokemonSpriteSize });

    sf::Texture* evolvedPokemonTexture = SpriteLoader::getPokemonSprite(m_evolutionId);
    m_evolvedPokemon.setTexture(*evolvedPokemonTexture);
    int evolvedPokemonSpriteSize = evolvedPokemonTexture->getSize().x / 4;
    m_evolvedPokemon.setTextureRect({ 0,0,evolvedPokemonSpriteSize, evolvedPokemonSpriteSize });

    // setting sprite position
    m_currentPokemon.setOrigin(m_currentPokemon.getLocalBounds().getSize() / 2.f);
    m_evolvedPokemon.setOrigin(m_evolvedPokemon.getLocalBounds().getSize() / 2.f);
    sf::Vector2f pokemonPosition = m_position + sf::Vector2f{ m_size.x / 2.f, m_size.y / 3.f };
    m_currentPokemon.setPosition(pokemonPosition);
    m_evolvedPokemon.setPosition(pokemonPosition);
    

    m_evolutionTime = maxEvolutionTime;

    m_evolving = true;
    m_doneEvolving = false;
    std::cout << id << " is currently evolving into " << m_evolutionId << "\n";
    m_clock.restart().asSeconds();

    GameUI::music.pause();
    m_song.setVolume(GameUI::sfxVolume);
    m_song.play();
    
    return true;
}

bool EvolutionUI::cancelEvolution(sf::Vector2f mousePos)
{
    if (m_evolutionTime <= 0.f) {
        m_evolving = false;
        m_song.stop();
        GameUI::music.play();
        return true;
    }
    if (m_cancelButton.getGlobalBounds().contains(mousePos)) {
        m_evolving = false;
        m_song.stop();
        GameUI::music.play();
        return true;
    }
    return false;
}

void EvolutionUI::draw(sf::RenderWindow& window)
{
    window.draw(m_box);


    float deltaTime = m_clock.restart().asSeconds();
    m_evolutionTime -= deltaTime;
    //std::cout << m_evolutionTime << '\n';

    if (m_evolutionTime <= 0.f) {
        if (!m_doneEvolving) {
            m_song.stop();
            m_song.openFromFile("sound/evolutionfinished.mp3");
            m_song.play();
            m_doneEvolving = true;
            std::string text{ m_pokemon->getName() };
            text += " evolved into ";
            m_pokemon->evolve(&Database::baseStats[m_evolutionId], SpriteLoader::getPokemonSprite(m_evolutionId));
            text += m_pokemon->getName() + '\n';
            m_evolutionText.setString(text);
            m_evolutionText.setOrigin(m_evolutionText.getLocalBounds().getSize().x / 2.f, m_evolutionText.getLocalBounds().getSize().y);
        }
        m_evolvedPokemon.setScale(4.f,4.f);
        window.draw(m_evolvedPokemon);
        window.draw(m_evolutionText);
    }
    else {

        float passedTime = maxEvolutionTime - m_evolutionTime;
        
        float scale = std::cos(passedTime * passedTime * 2) * 4.f;

        if (scale >= 0.f) {
            m_currentPokemon.setScale(std::abs(scale), std::abs(scale));
            window.draw(m_currentPokemon);
        }
        else {
            m_evolvedPokemon.setScale(std::abs(scale), std::abs(scale));
            window.draw(m_evolvedPokemon);
        }
        window.draw(m_cancelButton);
        window.draw(m_cancelText);
    }
}
