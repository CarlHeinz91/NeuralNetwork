#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "SFML\Graphics.hpp"

#include "Source\Track\Track.hpp"

#include "Source\ControlElements\Button.hpp"
#include "Source\ControlElements\CheckBox.hpp"
#include "Source\ControlElements\TextField.hpp"

#include "Source\Framework\EventManager.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"

#include <iostream>
#include <string>


class Editor
{
public:
	//Define Event Enumeration Class (Handles Changes of GameStates)
	enum class Event
	{
		NONE,
		OPEN_EDITOR_MENU
	};

private:
	//Access to the Fonts
	mySFML::Class::Fonts mFonts;

	//The current Event (Will be polled by pollEditorEvent)
	Event mEvent = Event::NONE;

private:
	//General Editor Stuff (Views, Layout Ratios, ...)
	sf::Vector2u const mEditorWindowSize;
	float const mGUIRatio = 0.3f;
	sf::View mGUIView;
	sf::View mTrackView;

	//Track, which is currently loaded
	Track* pCurrentTrack = nullptr;

	//Mode
	enum class Mode
	{
		MODIFY_BORDER_TRACK_SEGMENTS,
		MODIFY_CENTER_TRACK_SEGMENTS
	} mMode = Mode::MODIFY_CENTER_TRACK_SEGMENTS;

	//Menu Objects
	sf::RectangleShape mGUIBackgroundRectShape;

	sf::Vector2f const mLoadTextFieldPos = sf::Vector2f(20.f, 30.f);
	sf::Vector2f const mTextFieldSize = sf::Vector2f(225.f, 25.f);
	sf::Vector2f const mButtonSize = sf::Vector2f(75.f, 25.f);
	sf::Vector2f const mRelDistBetweenTextFields = sf::Vector2f(0.f, 40.f);
	sf::Vector2f const mRelDistBetweenTextFieldAndButton = sf::Vector2f(10.f, 0.f);
	unsigned int const mCharacterSize = 16u;

	sf::Vector2f const mSaveTextFieldPos = mLoadTextFieldPos + mRelDistBetweenTextFields;
	sf::Vector2f const mLoadButtonPos = mLoadTextFieldPos + sf::Vector2f(mTextFieldSize.x, 0.f) + mRelDistBetweenTextFieldAndButton;
	sf::Vector2f const mSaveButtonPos = mLoadButtonPos + mRelDistBetweenTextFields;

	TextField mLoadTextField;
	TextField mSaveTextField;
	Button mLoadButton;
	Button mSaveButton;
	
	sf::Vector2f const mStatusTextFieldPos = sf::Vector2f(20.f, 115.f);
	sf::Vector2f const mStatusTextFieldSize = sf::Vector2f(310.f, 25.f);
	TextField mStatusTextField;

	sf::Vector2f const mModeCheckBoxPos = sf::Vector2f(50.f, 200.f);
	CheckBox mModeCheckBox;
	mySFML::Class::Text mModeText;

	sf::Vector2f const mCreateCircleTrackButtonPos = sf::Vector2f(50.f, 300.f);
	sf::Vector2f const mCreateCircleTrackButtonSize = sf::Vector2f(200.f, 40.f);
	Button mCreateCircleTrackButton;



public:
	//Constructors, Destructor & Assignment Operator
	Editor(sf::Vector2u const & editorWindowSize);
	~Editor();
	Editor(Editor const &) = delete;
	Editor& operator=(Editor const &) = delete;

	//Render & Update
	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow * renderWindow);

private:
	//Track Loading & Discarding (Also Manages Resources)
	void loadTrack(std::string const & trackPath);
	void discardTrack();

	void showStatus(std::string const & status);

public:
	//Functions for external communication
	Event pollEditorEvent();
	void reactOnESC();


private:
	//Views & Rendering
	void setViews();
	void changeTrackView(sf::RenderWindow const * renderWindow);
	void renderGUI(sf::RenderWindow * renderWindow);
	void renderTrack(sf::RenderWindow * renderWindow);
	bool checkIfMouseIsInTrackViewport(sf::Vector2i const & mousePos) const;

	//Track Manipulation
	void manipulateTrack(sf::RenderWindow const * renderWindow);
	void modifyBorderOrCenterTrackSegments(sf::RenderWindow const * renderWindow);
	void createCircleTrack();

};






#endif //EDITOR_HPP

