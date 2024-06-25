#ifndef SOUNDEFFECTMANAGER_H
#define SOUNDEFFECTMANAGER_H

#include <SFML/Audio.hpp>
#include "LinkedList.h"
#include "ui/GameUI.h"

namespace SoundEffectManager
{
	extern sf::SoundBuffer hitSoundBuffer;

	extern LinkedList<sf::Sound> sounds;


	void add(sf::Sound* sound);
	void update();
	void clear();

};



#endif
