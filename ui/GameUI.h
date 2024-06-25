#ifndef GAMEUI_H
#define GAMEUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
namespace GameUI {
	
	extern sf::Font font;
	extern sf::Texture levelUpArrow;
	extern sf::Texture border;
	extern sf::Texture statusTexture;
	extern sf::Texture statusIconTexture;
	extern sf::Texture levelLockedTexture;
	extern sf::Color   backgroundColor;
	extern sf::Color   buttonColor;
	extern sf::SoundBuffer  clickSoundBuffer;
	extern sf::Sound	    clickSound;
	extern sf::Music		music;
	extern float			musicVolume;
	extern float			sfxVolume;
	extern bool				isFullScreen;

}



#endif
