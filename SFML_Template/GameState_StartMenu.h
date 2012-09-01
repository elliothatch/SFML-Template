#pragma once
#include "gamestatebase.h"
#include <SFML/Graphics.hpp>

class GameState_StartMenu :
	public GameStateBase
{
public:
	GameState_StartMenu(void);
	virtual ~GameState_StartMenu(void);

	void OnAwake(void);
	void OnUpdate(void);
	//void OnRender(void);
	void OnCleanup(void);
	void OnSuspend(void);
	void OnResume(void);

	void printPressed(void) {printf("Pressed...\n");}
	void printReady(void) {printf("Ready...\n");}
	void printClicky(void) {printf("CLIIICKYYYYYY!!!\n");}
	void incrementByOne(void) {counter++; printf("Counter is now at %d\n",counter);}
	void playBroSound(void){printf("Play sound\n");bro.play();}
	void pauseAnim(void) {anim->Pause();}
	void playAnim(void) {anim->Play();}
	void stopAnim(void) {anim->Stop();}
	void resetAnim(void) {anim->Reset();}
	void nextFrameAnim(void) {anim->NextFrame();}
	void prevFrameAnim(void) {anim->PreviousFrame();}
	void gotoFrame1(void) {anim->GotoFrame(1);}

	//SFMLEvent Overrides
	virtual void OnKeyPressed(sf::Keyboard::Key key, bool alt, bool control, bool shift);
	virtual void OnMouseButtonPressed(sf::Mouse::Button button, int x, int y);
	virtual void OnMouseButtonReleased(sf::Mouse::Button button, int x, int y);
private:
	sf::Sound bro;
	int counter;
	AnimatedSprite* anim;
};

