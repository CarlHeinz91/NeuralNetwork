#include "stdafx.h"
#include "Source\MainModules\RaceSimMenu.hpp"


RaceSimMenu::RaceSimMenu()
	: mContinueButton(sf::Vector2f(200.f, 200.f), sf::Vector2f(200.f, 50.f), "Continue", mySFML::Class::FontName::ARIAL, 2.f, 18u),
	  mExitButton(sf::Vector2f(200.f, 300.f), sf::Vector2f(200.f, 50.f), "Exit", mySFML::Class::FontName::ARIAL, 2.f, 18u),
	  mTitleText(*mFonts.getFont(mySFML::Class::FontName::ARIAL), sf::Vector2f(200.f, 100.f), "Race Simulation Menu", 28u, sf::Color::White)
{

}

RaceSimMenu::~RaceSimMenu()
{

}


void RaceSimMenu::render(sf::RenderWindow * renderWindow)
{
	renderWindow->draw(*mTitleText.pointer);
	mContinueButton.render(renderWindow);
	mExitButton.render(renderWindow);
}

void RaceSimMenu::update(sf::Time const & time, sf::RenderWindow * renderWindow)
{
	mContinueButton.updateState(renderWindow);
	mExitButton.updateState(renderWindow);

	if (mContinueButton.getMouseReleasedEventOccured())
	{
		mEvent = Event::CONTINUE;
	}
	if (mExitButton.getMouseReleasedEventOccured())
	{
		mEvent = Event::EXIT_RACE_SIMULATION;
	}
}


RaceSimMenu::Event RaceSimMenu::pollRaceSimMenuEvent()
{
	Event backup = mEvent;
	mEvent = Event::NONE;
	return backup;
}

void RaceSimMenu::reactOnESC()
{
	mEvent = Event::CONTINUE;
}

