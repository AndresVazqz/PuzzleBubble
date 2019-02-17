#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "Credits.h"
#include "manual.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}

	int puntuation;
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void initCredits();
	void initMenu();
	void initManual();
	void initScene();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	void updatePuntuation(int points) const;
	bool getKey(int key) const;
	int getPuntuation();
	bool getSpecialKey(int key) const;

private:
	bool bPlay, bMenu, bScene, bCredits, bManual;                       // Continue to play game?
	Scene scene;   // Scene to render
	Menu menu;
	Credits credits;
	Manual manual;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time

};


#endif // _GAME_INCLUDE


