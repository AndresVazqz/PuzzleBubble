#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Game.h"


void Game::initScene()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.setLevel(1);
	scene.setCredits(2);
	puntuation = 0;
	scene.setPoints(0);
	scene.narrow = 1;
	scene.nbomb = 1;
	scene.init();
}

void Game::initMenu()
{
	bPlay = true;
	bMenu = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	menu.init();
	menu.music.play();
	menu.setStart(false);
	menu.setH(false);
}

void Game::initManual()
{
	bPlay = true;
	bManual = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	manual.init();
}


void Game::initCredits()
{
	bPlay = true;
	menu.music.stop();
	bCredits = true;
	int round = scene.getRound();
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	credits.init(scene.getStageClear(), puntuation,round);
}
bool Game::update(int deltaTime)
{


	if (bMenu){
		menu.update(deltaTime);
	}
	if (bManual){
		manual.update(deltaTime);
	}
	if (bCredits){
		credits.update(deltaTime);
	}
	if (manual.getStart()){
		manual.setStart(false);
		bMenu = true;
		bManual = false;
		initMenu();
	}
	if (menu.getStart()){
		menu.setStart(false);
		bScene = true;
		bMenu = false;
		initScene();
	}
	if (menu.getH()){
		menu.setH(false);
		bManual = true;
		bMenu = false;
		initManual();
	}
	if (scene.getEndgame()){
		scene.setEndgame(false);
		bScene = false;
		bCredits = true;
		credits.setStart(false);
		initCredits();
		credits.setStart(false);
	}
	if (credits.getStart()){
		credits.setStart(false);
		bCredits = false;
		bMenu = true;
		initMenu();
	}
	if (bScene){
		scene.update(deltaTime);
		puntuation += scene.getPoints();
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (bScene){
		scene.render();
	}
	if (bMenu){
		menu.render();
	}
	if(bManual){
		manual.render();
	}
	if (bCredits){
		credits.render();
	}
}

void Game::keyPressed(int key)
{

	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

int Game::getPuntuation()
{
	return puntuation;
}
bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}




