#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include <iostream>
#include <stack>

#include "SFML\Graphics.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"

#include "Source\Framework\EventManager.hpp"

#include "Source\GameState\GameState.hpp"
#include "Source\GameState\MainMenuState.hpp"
#include "Source\GameState\RaceState.hpp"
#include "Source\GameState\EditorState.hpp"
#include "Source\GameState\LearnState.hpp"
#include "Source\GameState\EditorMenuState.hpp"
#include "Source\GameState\RaceSimMenuState.hpp"
#include "Source\GameState\LearnMenuState.hpp"

#include "Source\Framework\TextureManager.hpp"
#include "Source\Framework\SoundManager.hpp"


class Framework
{
private:
	//RenderWindow
	static sf::RenderWindow *pRenderWindow;

	//Fonts
	mySFML::Class::Fonts mFonts;

	//Frametime & Utilization
	sf::Clock mFPSClock;
	sf::Time mFrametime = sf::seconds(0.f);
	sf::Time mRenderTimePerFrame = sf::seconds(0.f);
	unsigned int mFPS = 0;
	float mUtilization = 0.f;
	mySFML::Class::Text mFPSText;
	mySFML::Class::Text mUtilizationText;
	bool mShowFPS = true;

	//GameState
	std::stack<GameState::GameState*> mStackOfGameStates;

private:
	void determineFrametime();
	void handleEvents();
	void update();
	void render();


public:
	Framework();
	~Framework();

	void run();

public:
	static sf::RenderWindow const * getRenderWindow();
	static void setFramerateLimit(bool active, int fps = 50);

};



#endif
