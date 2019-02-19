#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <random> 

enum PlayerAnims
{
	STAND, MOVE_LEFT, MOVE_RIGHT, MOVE, DROP
};

enum BobbleAnims
{
	BLUE, GRAY, RED, YELLOW, GREEN, BLACK, ORANGE, PURPLE
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	bufferThrow.loadFromFile("music/pbobble-002.wav");
	soundThrow.setBuffer(bufferThrow);
	timedropLanded = 0;
	timedropExplode = 0;
	timedropBomb = 0;
	timedrop = 0;
	ballsThrown = 0;
	throwBall = false;
	landed = false;
	exploded = false;
	explodedBomb = false;
	srand(time(NULL));
	gameOver = false;
	stageClear = false;
	restart = false;
	downColorBall = 1;
	busy = false;
	targetPoint = 0;
	pointing = false;
	
	string image = string("images/bobbleExpl.png");
	explodesheet.loadFromFile(image, TEXTURE_PIXEL_FORMAT_RGBA);

	pointssheet.loadFromFile("images/numerospunt.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sparklesheet.loadFromFile("images/superbobbles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bombexplsheet.loadFromFile("images/explosionbomb.png", TEXTURE_PIXEL_FORMAT_RGBA);

	ernestosheet.loadFromFile("images/ernesto.png", TEXTURE_PIXEL_FORMAT_RGBA);
	ernesto = Sprite::createSprite(glm::ivec2(56, 40), glm::vec2(0.5, 0.125), &ernestosheet, &shaderProgram);
	ernesto->setNumberAnimations(3);

	ernesto->setAnimationSpeed(STAND, 8);
	ernesto->addKeyframe(STAND, glm::vec2(0.5f, 0.f));
	ernesto->addKeyframe(STAND, glm::vec2(0.5f, 0.125f));
	ernesto->addKeyframe(STAND, glm::vec2(0.5f, 0.25f));

	ernesto->setAnimationSpeed(MOVE_RIGHT, 32);
	ernesto->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	ernesto->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.625f));
	ernesto->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.75f));
	ernesto->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.875f));
	ernesto->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	ernesto->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.125f));
	ernesto->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));
	ernesto->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.375f));

	ernesto->setAnimationSpeed(MOVE_LEFT, 32);
	ernesto->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
	ernesto->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.375f));
	ernesto->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	ernesto->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.125f));
	ernesto->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	ernesto->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.875f));
	ernesto->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.75f));
	ernesto->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.625f));

	felipesheet.loadFromFile("images/felipe.png", TEXTURE_PIXEL_FORMAT_RGBA);
	felipe = Sprite::createSprite(glm::ivec2(48, 40), glm::vec2(0.5, 0.125), &felipesheet, &shaderProgram);
	felipe->setNumberAnimations(5);
	
	felipe->setAnimationSpeed(STAND, 8);
	felipe->addKeyframe(STAND, glm::vec2(0.f, 0.f));
	felipe->addKeyframe(STAND, glm::vec2(0.f, 0.125f));
	felipe->addKeyframe(STAND, glm::vec2(0.f, 0.25f));
	felipe->addKeyframe(STAND, glm::vec2(0.f, 0.375f));

	felipe->setAnimationSpeed(DROP,8);
	felipe->addKeyframe(DROP, glm::vec2(0.f, 0.5f));
	felipe->addKeyframe(DROP, glm::vec2(0.f, 0.625f));
	felipe->addKeyframe(DROP, glm::vec2(0.f, 0.75f));
	/*
	felipe->setAnimationSpeed(MOVE_LEFT, 32);
	felipe->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
	felipe->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.625f));
	felipe->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.75f));
	felipe->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.875f));
	felipe->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	felipe->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.125f));
	felipe->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	felipe->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.375f));
	
	felipe->setAnimationSpeed(MOVE_RIGHT, 32);
	felipe->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	felipe->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.375f));
	felipe->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));
	felipe->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.125f));
	felipe->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	felipe->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.875f));
	felipe->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.75f));
	felipe->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.625f));
	*/
	bagsheet.loadFromFile("images/bag.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bagup = Sprite::createSprite(glm::ivec2(112, 48), glm::vec2(0.5, 1.0), &bagsheet, &shaderProgram);
	bagup->setNumberAnimations(1);
	
	bagup->setAnimationSpeed(STAND, 8);
	bagup->addKeyframe(STAND, glm::vec2(0.f, 0.0f));

	bagdown = Sprite::createSprite(glm::ivec2(112, 48), glm::vec2(0.5, 1.0), &bagsheet, &shaderProgram);
	bagdown->setNumberAnimations(1);

	bagdown->setAnimationSpeed(STAND, 8);
	bagdown->addKeyframe(STAND, glm::vec2(0.5f, 1.0f));

	cranksheet.loadFromFile("images/crank.png", TEXTURE_PIXEL_FORMAT_RGBA);
	crank = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.5), &cranksheet, &shaderProgram);
	crank->setNumberAnimations(3);

	crank->setAnimationSpeed(STAND, 8);
	crank->addKeyframe(STAND, glm::vec2(0.f, 0.5f));

	crank->setAnimationSpeed(MOVE_RIGHT, 32);
	crank->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	crank->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));
	crank->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.5f));
	crank->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.5f));
	crank->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	crank->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
	crank->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));
	crank->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.f));

	crank->setAnimationSpeed(MOVE_LEFT, 32);
	crank->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
	crank->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.f));
	crank->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));
	crank->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));
	crank->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	crank->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.5f));
	crank->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));
	crank->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));

	arksheet.loadFromFile("images/ark.png", TEXTURE_PIXEL_FORMAT_RGBA);
	ark = Sprite::createSprite(glm::ivec2(68, 32), glm::vec2(1.0, 0.5), &arksheet, &shaderProgram);
	ark->setNumberAnimations(2);
		ark->setAnimationSpeed(STAND, 8);
		ark->addKeyframe(STAND, glm::vec2(0.f, 0.f));

		ark->setAnimationSpeed(MOVE, 8);
		ark->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
		ark->addKeyframe(MOVE, glm::vec2(0.f, 0.5f));

	cannonsheet.loadFromFile("images/cannon.png", TEXTURE_PIXEL_FORMAT_RGBA);
	cannon = Sprite::createSprite(glm::ivec2(112,48), glm::vec2(0.125, 0.5), &cannonsheet, &shaderProgram);
	cannon->setNumberAnimations(3);
			
		cannon->setAnimationSpeed(STAND, 8);
		cannon->addKeyframe(STAND, glm::vec2(0.f, 0.f));

		cannon->setAnimationSpeed(MOVE_RIGHT, 16);
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.375f, 0.f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.625f, 0.f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.0f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.875f, 0.f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.5f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));
		cannon->addKeyframe(MOVE_RIGHT, glm::vec2(0.375f, 0.5f));
			
		cannon->setAnimationSpeed(MOVE_LEFT, 16); 
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.5f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.5f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.875f, 0.f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.0f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));
		cannon->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.f));
		
	ballssheet.loadFromFile("images/bobbles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	balls = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 0.125), &ballssheet, &shaderProgram);
	balls->setNumberAnimations(8);
	balls->setAnimationSpeed(BLUE, 8);
	balls->setAnimationSpeed(GRAY, 8);
	balls->setAnimationSpeed(RED, 8);
	balls->setAnimationSpeed(YELLOW, 8);
	balls->setAnimationSpeed(GREEN, 8);
	balls->setAnimationSpeed(BLACK, 8);
	balls->setAnimationSpeed(ORANGE, 8);
	balls->setAnimationSpeed(PURPLE, 8);
	balls->addKeyframe(BLUE,    glm::vec2(0.f, 0.f));
	balls->addKeyframe(GRAY,    glm::vec2(0.f, 0.125f));
	balls->addKeyframe(RED,		glm::vec2(0.f, 0.25f));
	balls->addKeyframe(YELLOW,  glm::vec2(0.f, 0.375f));
	balls->addKeyframe(GREEN,   glm::vec2(0.f, 0.5f));
	balls->addKeyframe(BLACK,   glm::vec2(0.f, 0.625f));
	balls->addKeyframe(ORANGE,  glm::vec2(0.f, 0.75f));
	balls->addKeyframe(PURPLE,  glm::vec2(0.f, 0.875f));

	hurrysheet.loadFromFile("images/hurryup.png", TEXTURE_PIXEL_FORMAT_RGBA);
	hurry = Sprite::createSprite(glm::ivec2(64, 56), glm::vec2(0.125f, 1.f), &hurrysheet, &shaderProgram);
	hurry->setNumberAnimations(8);
	hurry->setAnimationSpeed(0, 4);
	hurry->addKeyframe(0, glm::vec2(0.f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.125f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.25f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.375f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.50f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.625f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));
	hurry->addKeyframe(0, glm::vec2(0.f, 0.f));
	hurry->addKeyframe(0, glm::vec2(0.875f, 0.0));

	hurry->setAnimationSpeed(1, 8);
	hurry->addKeyframe(1, glm::vec2(0.875f, 0.0));

	hurry->changeAnimation(1);
	hurry->setPosition(glm::vec2(SCREEN_WIDTH / 2 - 94, SCREEN_HEIGHT - 134.f));
	
	cannon->changeAnimation(0);
	ark->changeAnimation(0);
	crank->changeAnimation(0);
	ernesto->changeAnimation(0);
	felipe->changeAnimation(0);
	bagdown->changeAnimation(0);
	balls->changeAnimation(downColorBall-1);
	bagup->changeAnimation(0);
	lastTime = 0.0f;
	tileMapDispl = tileMapPos;
}

void Player::update(int deltaTime)
{
	if (!gameOver && !stageClear){
		cannon->update(deltaTime);
		crank->update(deltaTime);
		ernesto->update(deltaTime);
		ark->update(deltaTime);
		felipe->update(deltaTime);
		hurry->update(deltaTime);
		lastTime += deltaTime;
		if (lastTime >= 5000){
			if (hurry->animation() != 0){
				hurry->changeAnimation(0);
			}
		}

		if (throwBall && !(landed || exploded)){
			throwBall = false;
			actualAngle = targetPoint;
		}
		else{
			if (!busy && !(landed || exploded)){
				if (Game::instance().getSpecialKey(GLUT_KEY_UP) || lastTime > 10000)
				{
					if (!throwBall){
						soundThrow.play();
						lastTime = 0;
						throwBall = true;
						actualAngle = targetPoint;
						ballsThrown++;
						hurry->changeAnimation(1);
					}
				}
			}
		}

		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (cannon->animation() != MOVE_LEFT)
				cannon->changeAnimation(MOVE_LEFT);
			if (ernesto->animation() != MOVE_LEFT)
				ernesto->changeAnimation(MOVE_LEFT);
			if (crank->animation() != MOVE_LEFT)
				crank->changeAnimation(MOVE_LEFT);
			if (ark->animation() != MOVE)
				ark->changeAnimation(MOVE);
			//posPlayer.x -= 2;

			targetPoint -= 2.9f / 125;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (cannon->animation() != MOVE_RIGHT)
				cannon->changeAnimation(MOVE_RIGHT);
			if (ernesto->animation() != MOVE_RIGHT)
				ernesto->changeAnimation(MOVE_RIGHT);
			if (crank->animation() != MOVE_RIGHT)
				crank->changeAnimation(MOVE_RIGHT);
			if (ark->animation() != MOVE)
				ark->changeAnimation(MOVE);
			//posPlayer.x += 2;
			targetPoint += 2.9f / 125;

		}
		else
		{
			if (cannon->animation() != STAND){
				cannon->changeAnimation(STAND);
			}
			if (ernesto->animation() != STAND){
				ernesto->changeAnimation(STAND);
			}
			if (crank->animation() != STAND){
				crank->changeAnimation(STAND);
			}
			if (ark->animation() != STAND){
				ark->changeAnimation(STAND);
			}
			if (felipe->animation() != STAND){
				if (timedrop > deltaTime){
					if (felipe->animation() != STAND){
						felipe->changeAnimation(STAND);
					}
					timedrop = 0;
				}
				else ++timedrop;
			}
		}
		if (1.45f <= targetPoint){
			targetPoint = 1.45f;
		}
		if (-1.45f >= targetPoint){
			targetPoint = -1.45f;
		}		
	}
	else {
		cannon->changeAnimation(0);
		ark->changeAnimation(0);
		crank->changeAnimation(0);
		ernesto->changeAnimation(0);
		felipe->changeAnimation(0);
		bagdown->changeAnimation(0);
		balls->changeAnimation(BLACK);
		bagup->changeAnimation(0);
		if (Game::instance().getKey(13))
		{
			restart = true;
		}
		cannon->update(deltaTime);
		crank->update(deltaTime);
		ernesto->update(deltaTime);
		ark->update(deltaTime);
		felipe->update(deltaTime);
	}
	if (landed) {
		sparkle->update(deltaTime);

		if (timedropLanded > deltaTime){
			landed = false;
			timedropLanded = 0;
			sparkle->free();
		}
		else ++timedropLanded;
	}
	if (explodedBomb) {
		explodeBomb->update(deltaTime);

		if (timedropBomb > deltaTime/2){
			explodedBomb = false;
			timedropBomb = 0;
			explodeBomb->free();
		}
		else ++timedropBomb;
	}
	if (exploded) {
		if (!pointing){
			for (int ii = 0; ii < explodeBalls.size(); ++ii){
				explodeBalls[ii]->update(deltaTime);
			}
		}
		else {
			for (int ii = 0; ii < pointsBalls.size(); ++ii){
				glm::vec2 pos = pointsBalls[ii]->getPosition();
				pointsBalls[ii]->setPosition(glm::vec2(pos.x, pos.y - 2.f));
				pointsBalls[ii]->update(deltaTime);
			}
			
		}
		if (timedropExplode > deltaTime/2){
			if (!pointing){
				timedropExplode = 0;
				for (int ii = 0; ii < explodeBalls.size(); ++ii){
					explodeBalls[ii]->free();
				}
				explodeBalls.clear();
				pointing = true;
			}
			else {
				exploded = false;
				timedropExplode = 0;
				for (int ii = 0; ii < pointsBalls.size(); ++ii){
					pointsBalls[ii]->free();
				}
				pointsBalls.clear();
				pointing = false;
			}
		}
		else ++timedropExplode;
	}
}

void Player::sparkleLand(const glm::ivec2 &tile, int colr, int incrFloor, ShaderProgram &shaderProgram){
	--colr;
	
	
	sparkle = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.125), &sparklesheet, &shaderProgram);
	sparkle->setNumberAnimations(1);

	sparkle->setAnimationSpeed(STAND, 16);
	sparkle->addKeyframe(STAND, glm::vec2(0.1f, colr*0.125f));
	sparkle->addKeyframe(STAND, glm::vec2(0.4f, colr*0.125f));
	sparkle->addKeyframe(STAND, glm::vec2(0.5f, colr*0.125f));
	sparkle->addKeyframe(STAND, glm::vec2(0.6f, colr*0.125f));
	sparkle->addKeyframe(STAND, glm::vec2(0.7f, colr*0.125f));
	sparkle->addKeyframe(STAND, glm::vec2(0.8f, colr*0.125f));
	sparkle->addKeyframe(STAND, glm::vec2(0.9f, colr*0.125f));
	sparkle->addKeyframe(STAND, glm::vec2(0.0f, colr*0.125f));

	sparkle->changeAnimation(0);
	float x = 16.f + 32.f * tile.x + 16.f * (tile.y % 2);
	float y = 16.f + (tile.y * 32.f) - (tile.y * 32.f / 8.f) + incrFloor * (32.f - 32.f/8);
	sparkle->setPosition(glm::vec2((x + 192 - 16 + .5f), (y + 64 - 16 + .5f)));
	landed = true;

}


void Player::explodeLand(stack<glm::ivec2> tiles, int color, int incrFloor, ShaderProgram &shaderProgram){
	--color;
	
	int i = 0;
	while (!tiles.empty()){
		glm::ivec2 tile = tiles.top();
		tiles.pop();

		explodeBalls.push_back(Sprite::createSprite(glm::ivec2(63, 63), glm::vec2(0.2, 0.125), &explodesheet, &shaderProgram));
		pointsBalls.push_back(Sprite::createSprite(glm::ivec2(28, 20), glm::vec2(0.2, 1), &pointssheet, &shaderProgram));
		explodeBalls[i]->setNumberAnimations(1);
		pointsBalls[i]->setNumberAnimations(1);
		explodeBalls[i]->setAnimationSpeed(STAND, 32);
		pointsBalls[i]->setAnimationSpeed(STAND, 32);
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.0f, color*.125f));
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.2f, color*.125f));
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.4f, color*.125f));
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.6f, color*.125f));
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.8f, color*.125f));

		pointsBalls[i]->addKeyframe(STAND, glm::vec2(0.0f, 0.f));
		pointsBalls[i]->addKeyframe(STAND, glm::vec2(0.0f, 0.f));

		explodeBalls[i]->changeAnimation(0);

		pointsBalls[i]->changeAnimation(0);
		float x = 16.f + 32.f * tile.x + 16.f * ((tile.y)% 2);
		float y = 16.f + (tile.y * 32.f) - (tile.y * 32.f / 8.f) + incrFloor * (32.f - 32.f / 8);
		explodeBalls[i]->setPosition(glm::vec2((x + 192 - 31), (y + 64 - 31)));

		pointsBalls[i]->setPosition(glm::vec2((x + 192 - 31 + 17), (y + 64 - 31 + 20)));
		++i;
	}
	pointing = false;
	exploded = true;
}

void Player::explodeArrow(stack<glm::ivec2> tiles, stack<int> colors, int incrFloor, ShaderProgram &shaderProgram){


	int i = 0;
	while (!tiles.empty()){
		glm::ivec2 tile = tiles.top();
		tiles.pop();
		int color = colors.top();
		colors.pop();
		--color;
		explodeBalls.push_back(Sprite::createSprite(glm::ivec2(63, 63), glm::vec2(0.2, 0.125), &explodesheet, &shaderProgram));
		pointsBalls.push_back(Sprite::createSprite(glm::ivec2(28, 20), glm::vec2(0.2, 1), &pointssheet, &shaderProgram));
		explodeBalls[i]->setNumberAnimations(1);
		pointsBalls[i]->setNumberAnimations(1);
		explodeBalls[i]->setAnimationSpeed(STAND, 32);
		pointsBalls[i]->setAnimationSpeed(STAND, 32);
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.0f, color*.125f));
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.2f, color*.125f));
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.4f, color*.125f));
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.6f, color*.125f));
		explodeBalls[i]->addKeyframe(STAND, glm::vec2(0.8f, color*.125f));

		pointsBalls[i]->addKeyframe(STAND, glm::vec2(0.0f, 0.f));
		pointsBalls[i]->addKeyframe(STAND, glm::vec2(0.0f, 0.f));

		explodeBalls[i]->changeAnimation(0);

		pointsBalls[i]->changeAnimation(0);
		float x = 16.f + 32.f * tile.x + 16.f * ((tile.y) % 2);
		float y = 16.f + (tile.y * 32.f) - (tile.y * 32.f / 8.f) + incrFloor * (32.f - 32.f / 8);
		explodeBalls[i]->setPosition(glm::vec2((x + 192 - 31), (y + 64 - 31)));

		pointsBalls[i]->setPosition(glm::vec2((x + 192 - 31 + 17), (y + 64 - 31 + 20)));
		++i;
	}
	pointing = false;
	exploded = true;
}

void Player::exploteBomb(glm::ivec2 tile, int incrFloor, ShaderProgram &shaderProgram){

	explodeBomb = Sprite::createSprite(glm::ivec2(128, 128), glm::vec2(0.2, 1.0), &bombexplsheet, &shaderProgram);
	explodeBomb->setNumberAnimations(3);
	explodeBomb->setAnimationSpeed(0, 32);
	explodeBomb->addKeyframe(0, glm::vec2(0.0f, 0.f));
	explodeBomb->addKeyframe(0, glm::vec2(0.2f, 0.f));
	explodeBomb->addKeyframe(0, glm::vec2(0.4f, 0.f));
	explodeBomb->addKeyframe(0, glm::vec2(0.6f, 0.f));
	explodeBomb->addKeyframe(0, glm::vec2(0.8f, 0.f));

	explodeBomb->changeAnimation(0);
	float x = 16.f + 32.f * tile.x + 16.f * ((tile.y) % 2);
	float y = 16.f + (tile.y * 32.f) - (tile.y * 32.f / 8.f) + incrFloor * (32.f - 32.f / 8);
	explodeBomb->setPosition(glm::vec2((x + 192 - 64), (y + 64 - 64)));
	explodedBomb = true;
}

void Player::render1()
{

	ark->render();
	cannon->render();
	crank->render();
	bagdown->render();
}

void Player::render2()
{
	bagup->render();
	balls->render();
	if (landed)	sparkle->render();
	

	if (exploded){
		if (!pointing){
			for (int i = 0; i < explodeBalls.size(); ++i){
				explodeBalls[i]->render();
			}
		}
		else {
			for (int i = 0; i < pointsBalls.size(); ++i){
				pointsBalls[i]->render();
			}
		}
	}
	if (explodedBomb)explodeBomb->render();
	ernesto->render();
	felipe->render();
	hurry->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;

	changeColor();
	changeColor();
	felipe->changeAnimation(STAND);
}

void Player::setStageClear(bool clear){
	stageClear = clear;
}

void Player::changeColor(){
	if (map->getBalls(0) > 0){
		colorBall = downColorBall;
		int num = 1 + rand() % (8);
		while (map->getBalls(num) == 0){
			num = 1 + rand() % (8);
		}

		downColorBall = num;
		balls->changeAnimation(downColorBall-1);
		felipe->changeAnimation(DROP);
	}
}

void Player::setBusy(bool bussy)
{
	busy = bussy;
}


float Player::getTarget()
{
	return targetPoint;
}

int Player::getBallsThrown()
{
	return ballsThrown;
}

float Player::getActualAngle()
{
	return actualAngle;
}

bool Player::getThrowBall()
{
	return throwBall;
}

bool Player::getRestart()
{
	return restart;
}

int Player::getColor(){
	return colorBall;
}

void Player::setGameOver(bool over){
	gameOver = over;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	cannon->setPosition(glm::vec2(float(posPlayer.x - 64), float(tileMapDispl.y + posPlayer.y - 32 - 48)));
	ark->setPosition(glm::vec2(float(posPlayer.x - 34), float(tileMapDispl.y + posPlayer.y - 32 - 48 - 32 + 1 )));
	crank->setPosition(glm::vec2(float(posPlayer.x - 16 + 56), float(tileMapDispl.y + posPlayer.y - 32 -32)));
	ernesto->setPosition(glm::vec2(float(posPlayer.x - 28 + 72), float(tileMapDispl.y + posPlayer.y - 32 -40)));
	felipe->setPosition(glm::vec2(float(posPlayer.x - 28 - 32), float(tileMapDispl.y + posPlayer.y - 32 - 40)));
	balls->setPosition(glm::vec2(float(posPlayer.x - 24 - 32 - 32), float(tileMapDispl.y + posPlayer.y - 32 - 32)));
	bagdown->setPosition(glm::vec2(float(posPlayer.x - 56 - 88-8), float(tileMapDispl.y + posPlayer.y - 32 - 48)));
	bagup->setPosition(glm::vec2(float(posPlayer.x - 56 - 88 -8), float(tileMapDispl.y + posPlayer.y - 32 - 48)));
}




