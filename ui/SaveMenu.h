#ifndef SAVEMENU_H
#define SAVEMENU_H


#include "SaveInfoUI.h"
#include "../Player.h"
#include "StarterPickMenu.h"
#include "TwoOptionMenu.h"
#include "OptionsMenu.h"

class SaveMenu
{
	StarterPickMenu* m_starterPickmenu;
	OptionsMenu*	 m_optionsMenu;

	sf::RenderWindow& m_window;
	sf::View&   m_view;
	Player&		m_player;
	int	   m_chosenSave{ 0 };
	std::array<SaveInfoUI,3> m_saveInfos;

	int m_deletingSave{ 0 };
	TwoOptionMenu m_deleteSaveMenu;
	sf::Texture m_deleteTexture;
	std::array<sf::Sprite,3> m_deleteButtons;

	sf::Text m_optionsText;
	sf::RoundedRectangleShape m_optionsButton;
	

	sf::Text m_quitText;
	sf::RoundedRectangleShape m_quitButton;

public:
	SaveMenu(Player& player, sf::RenderWindow& window, sf::View& view);
	~SaveMenu();
	bool chooseSave(sf::Event& event);
	void draw(sf::RenderWindow& window);
};


#endif
