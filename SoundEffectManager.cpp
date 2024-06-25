#include "SoundEffectManager.h"


void SoundEffectManager::add(sf::Sound* sound)
{
	if (!sound)
		return;
	sound->setVolume(GameUI::sfxVolume);
	sound->setBuffer(hitSoundBuffer);
	sound->play();
	sounds.add(sound);
	
}

void SoundEffectManager::update()
{
	LinkedList<sf::Sound>* lastSound{ &sounds };
	LinkedList<sf::Sound>* sound{ sounds.next };
	while (sound) {
		if (sound->value->Stopped()) {
			lastSound->next = sound->next;
			delete sound->value;
			delete sound;
			sound = lastSound->next;
		}
		else {
			lastSound = sound;
			sound = sound->next;
		}
		
	}
}

void SoundEffectManager::clear()
{

}
