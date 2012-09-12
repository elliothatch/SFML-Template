#pragma once
#include "SFMLEvent.h"
#include "SFMLStateMessage.h"
#include "ImageManager.h"
#include "SpriteManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "SFMLGUIElement.h"
#include "SFMLButton.h"
#include "AnimatedSprite.h"
#include "Functor.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>


class GameStateBase : public SFMLEvent
{
public:
	typedef std::unique_ptr<GameStateBase> ptr;

	GameStateBase(void);
	virtual ~GameStateBase(void);

	virtual void OnAwake(void) {}
	virtual void OnUpdate(void) {}
	virtual void OnRender(sf::RenderTarget& target) {drawDisplayList(target);}
	virtual void OnCleanup(void) {Cleanup();}
	virtual void OnSuspend(void) {}
	virtual void OnResume(void) {}

	//OnUpdate is used to update all game logic e.g. the position of a game entity
	//OnRender is used to update all the sprites and items in the m_displayList 
	//e.g. the position of a sprite on the screen based on its world position
	//the main render loop simply loops through the display list and draws everything from bottom to top

	void OnEvent(sf::Event& Event);

	SFMLStateMessage* GetStateMessage(void);
	
	//default events that most likely won't be overwritten
	virtual void OnClose(void);
	virtual void OnResize(int width, int height);

	struct MouseEvent
	{
		enum E
		{
			LeftPressed = 0,
			LeftReleased,
			RightPressed,
			RightReleased,
			MiddlePressed,
			MiddleReleased,
			RollOver,
			RollOut,
			Hover,
			Count
		};
	};

	void addDrawable(std::unique_ptr<sf::Drawable> target);
	void removeDrawable(int index);
	void addGUIElement(std::unique_ptr<SFMLGUIElement> target); //must be used so mouse listeners work
	void removeGUIElement(SFMLGUIElement* target);

	SFMLGUIElement* getTopGUIElement(int X, int Y);
	bool pointCollided(SFMLGUIElement* target, int X, int Y);

	//these are called in derived classes that use mouse driven GUIs to handle click events
	//place them in the virtual overrides for SFMLEvent e.g. OnMouseButtonPressed(...)
	void MouseEvent_Pressed(sf::Mouse::Button button, int x, int y);
	void MouseEvent_Released(sf::Mouse::Button button, int x, int y);
	void MouseEvent_Moved(sf::Mouse::Button button, int x, int y);

	void Cleanup();

	//this function is usually called by derived classes in their OnCleanup function to clear out the display list
private:
	void CleanupDisplayList(void);
	void CleanupGUIElements(void);

protected:
	std::vector<SFMLStateMessage*> m_messages;
private:
	std::vector<std::unique_ptr<sf::Drawable>> m_displayList;
	std::vector<SFMLGUIElement*> m_GUIElements;
public:
	bool m_transparent;
	bool m_allowSubUpdate;
	bool m_subUpdate;
	bool m_subRender;
protected:
	void drawDisplayList(sf::RenderTarget& target) const;
	const std::vector<std::unique_ptr<sf::Drawable>>& getDisplayList() const;
	const std::vector<SFMLGUIElement*>& getGUIElements() const;
};