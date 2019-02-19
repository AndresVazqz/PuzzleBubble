#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GL/glut.h>
#include "Scene.h"
#include "Game.h"



#define SCREEN_X 96*2
#define SCREEN_Y 32*2
#define MARGIN_SCREEN 16.f
#define MAP_X 32*8
#define MAP_Y 32*11
#define INIT_PLAYER_X_TILES 6
#define INIT_PLAYER_Y_TILES 2

#define SPEED_MODULE 12

Scene::Scene()
{
	map = NULL;
	player = NULL;
	ball = NULL;
	level = 1;
	credits = 2;
	nbomb = 1;
	narrow = 1;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
}


void Scene::init()
{
	initShaders();
	instalose = false;
	instawin = false;
	points = 0;
	if (stageClear || gameOver)	end->free();
	restart = false;
	gameOver = false;
	falling = -1;
	for (int ii = 0; ii < roof.size(); ++ii){
		roof[ii]->free();
	}
	isarrow = false;
	isbomb = false;
	texBackground.loadFromFile("images/background.bmp", TEXTURE_PIXEL_FORMAT_RGBA);

	texBottomBar.loadFromFile("images/bottomBar.png", TEXTURE_PIXEL_FORMAT_RGBA);

	texRound.loadFromFile("images/rounds.png", TEXTURE_PIXEL_FORMAT_RGBA);

	texCredit.loadFromFile("images/credit.png", TEXTURE_PIXEL_FORMAT_RGBA);

	texTube.loadFromFile("images/tube.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sarrowsheet.loadFromFile("images/esparrow.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sbombsheet.loadFromFile("images/espbomb.png", TEXTURE_PIXEL_FORMAT_RGBA);

	specialContsheet.loadFromFile("images/espcont.png", TEXTURE_PIXEL_FORMAT_RGBA);

	texArrow.loadFromFile("images/arrow.png", TEXTURE_PIXEL_FORMAT_RGBA);

	levelsheet.loadFromFile("images/levels.png", TEXTURE_PIXEL_FORMAT_RGBA);

	ballsheet.loadFromFile("images/bobbles.png", TEXTURE_PIXEL_FORMAT_RGBA);

	texLine.loadFromFile("images/line.png", TEXTURE_PIXEL_FORMAT_RGBA);

	endsheet.loadFromFile("images/endgame.png", TEXTURE_PIXEL_FORMAT_RGBA);

	toContsheet.loadFromFile("images/tocontinue.png", TEXTURE_PIXEL_FORMAT_RGBA);

	string image = string("images/bobbles.png");
	fallsheet.loadFromFile(image, TEXTURE_PIXEL_FORMAT_RGBA);

	image = string("images/roof.png");
	roofsheet.loadFromFile(image, TEXTURE_PIXEL_FORMAT_RGBA);



	bufferExp.loadFromFile("music/pbobble-005.wav");
	soundExp.setBuffer(bufferExp);
	bufferBomb.loadFromFile("music/slap.wav");
	soundBomb.setBuffer(bufferBomb);
	bufferArr.loadFromFile("music/fleachasonido.wav");
	soundArr.setBuffer(bufferArr);
	bufferSpr.loadFromFile("music/pbobble-004.wav");
	soundSpr.setBuffer(bufferSpr);
	bufferFloor.loadFromFile("music/pbobble-003.wav");
	soundFloor.setBuffer(bufferFloor);
	bufferWin.loadFromFile("music/soundclear.wav");
	soundWin.setBuffer(bufferWin);
	bufferLose.loadFromFile("music/soundlose.wav");
	soundLose.setBuffer(bufferLose);
	ended = false;
	roof.clear();
	stageClear = false;
	timeUp = 0;
	yBall = 0.f;
	xBall = 0.f;
	spdx = 0.0f;
	spdy = 0.0f;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texBackground.setMagFilter(GL_NEAREST);

	glm::vec2 geombb[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH,16) };
	glm::vec2 texCoordsbb[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	bottomBar = TexturedQuad::createTexturedQuad(geombb, texCoordsbb, texProgram);
	texBottomBar.setMagFilter(GL_NEAREST);

	glm::vec2 geomRound[2] = { glm::vec2(0.f, 0.f), glm::vec2(128, 16) };
	glm::vec2 texCoordsRound[2] = { glm::vec2(0.f, ((level) - 1) * (1.f / 5.f)), glm::vec2(1.f, level*(1.f/5.f)) };
	round = TexturedQuad::createTexturedQuad(geomRound, texCoordsRound, texProgram);
	texRound.setMagFilter(GL_NEAREST);

	glm::vec2 geomCredit[2] = { glm::vec2(0.f, 0.f), glm::vec2(120, 16) };
	glm::vec2 texCoordsCredit[2] = { glm::vec2(0.f, (credits) * 0.25f), glm::vec2(1.f, (credits +1) * 0.25f) };
	credit = TexturedQuad::createTexturedQuad(geomCredit, texCoordsCredit, texProgram);
	texCredit.setMagFilter(GL_NEAREST);

	glm::vec2 geomTube[2] = { glm::vec2(0.f, 0.f), glm::vec2(26, 22) };
	glm::vec2 texCoordsTube[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	tube = TexturedQuad::createTexturedQuad(geomTube, texCoordsTube, texProgram);
	texTube.setMagFilter(GL_NEAREST);

	glm::vec2 geomArr[2] = { glm::vec2(0.f, 0.f), glm::vec2(44, 112) };
	glm::vec2 texCoordsArr[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	arrow = TexturedQuad::createTexturedQuad(geomArr, texCoordsArr, texProgram);
	texArrow.setMagFilter(GL_NEAREST);
	string lvl = to_string(level);
	string levelfile = string("levels/level0").append(lvl.append(".txt"));;
	map = TileMap::createTileMap(levelfile, glm::vec2(SCREEN_X, SCREEN_Y), timeUp, texProgram);

	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT));
	player->setTileMap(map);

	//colorBall = player->getColor();
	//glm::vec2 texCoordsball[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.25f) };
	showlevel = Sprite::createSprite(glm::ivec2(254, 94), glm::vec2(0.5, 0.25), &levelsheet, &texProgram);
	showlevel->setNumberAnimations(5);

	showlevel->setAnimationSpeed(0, 8);
	showlevel->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	showlevel->setAnimationSpeed(1, 8);
	showlevel->addKeyframe(1, glm::vec2(0.0f, 0.25f));
	showlevel->setAnimationSpeed(2, 8);
	showlevel->addKeyframe(2, glm::vec2(0.0f, 0.5f));
	showlevel->setAnimationSpeed(3, 8);
	showlevel->addKeyframe(3, glm::vec2(0.5f, 0.0f));
	showlevel->setAnimationSpeed(4, 8);
	showlevel->addKeyframe(4, glm::vec2(0.5f, 0.25f));
	showlevel->setAnimationSpeed(5, 8);

	showlevel->changeAnimation(level-1);
	showlevel->setPosition(glm::vec2((SCREEN_WIDTH / 2) - 127, 128));

	colorBall = player->getColor();
	//glm::vec2 texCoordsball[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.25f) };
	ball = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 0.125), &ballsheet, &texProgram);
	ball->setNumberAnimations(8);

	ball->setAnimationSpeed(0, 8);
	ball->addKeyframe(0, glm::vec2(0.0f, 0.f));
	ball->setAnimationSpeed(1, 8);
	ball->addKeyframe(1, glm::vec2(0.0f, 0.125f));
	ball->setAnimationSpeed(2, 8);
	ball->addKeyframe(2, glm::vec2(0.0f, 0.25f));
	ball->setAnimationSpeed(3, 8);
	ball->addKeyframe(3, glm::vec2(0.0f, 0.375f));
	ball->setAnimationSpeed(4, 8);
	ball->addKeyframe(4, glm::vec2(0.0f, 0.5f));
	ball->setAnimationSpeed(5, 8);
	ball->addKeyframe(5, glm::vec2(0.0f, 0.625f));
	ball->setAnimationSpeed(6, 8);
	ball->addKeyframe(6, glm::vec2(0.0f, 0.75f));
	ball->setAnimationSpeed(7, 8);
	ball->addKeyframe(7, glm::vec2(0.0f, 0.875f));
		
	ball->changeAnimation(colorBall - 1);
	ball->setPosition(glm::vec2((SCREEN_WIDTH / 2) + xBall - MARGIN_SCREEN, SCREEN_HEIGHT - yBall - 48.f - 32.f - 16.f));
	glm::vec2 geomline[2] = { glm::vec2(0.f, 0.f), glm::vec2(256, 8) };
	glm::vec2 texCoordsline[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	line = TexturedQuad::createTexturedQuad(geomline, texCoordsline, texProgram);
	texLine.setMagFilter(GL_NEAREST);

	sBomb = Sprite::createSprite(glm::ivec2(32, 34), glm::vec2(1, 1), &sbombsheet, &texProgram);
	sBomb->setNumberAnimations(1);
	sBomb->setAnimationSpeed(0, 8);
	sBomb->addKeyframe(0, glm::vec2(0.f, 0.f));
	sBomb->changeAnimation(0);
	sBomb->setPosition(glm::vec2(201, 32.f));

	sArrow = Sprite::createSprite(glm::ivec2(16, 64), glm::vec2(1, 1), &sarrowsheet, &texProgram);
	sArrow->setNumberAnimations(1);
	sArrow->setAnimationSpeed(0, 8);
	sArrow->addKeyframe(0, glm::vec2(0.f, 0.f));
	sArrow->changeAnimation(0);
	sArrow->setPosition(glm::vec2(201, 32.f));

	puntsprite = vector<Sprite*>(8);

	sContArrow = Sprite::createSprite(glm::ivec2(48, 16), glm::vec2(0.5, 0.25), &specialContsheet, &texProgram);
	sContBomb = Sprite::createSprite(glm::ivec2(48, 16), glm::vec2(0.5, 0.25), &specialContsheet, &texProgram);

	sContArrow->setNumberAnimations(3);
	sContArrow->setAnimationSpeed(0, 8);
	sContArrow->addKeyframe(0, glm::vec2(0.5f, 0.f));

	sContArrow->setAnimationSpeed(1, 8);
	sContArrow->addKeyframe(1, glm::vec2(0.5f, 0.25f));

	sContArrow->setAnimationSpeed(2, 8);
	sContArrow->addKeyframe(2, glm::vec2(0.5f, 0.5f));

	sContBomb->setNumberAnimations(3);
	sContBomb->setAnimationSpeed(0, 8);
	sContBomb->addKeyframe(0, glm::vec2(0.0f, 0.f));

	sContBomb->setAnimationSpeed(1, 8);
	sContBomb->addKeyframe(1, glm::vec2(0.0f, 0.25f));

	sContBomb->setAnimationSpeed(2, 8);
	sContBomb->addKeyframe(2, glm::vec2(0.0f, 0.5f));


	sContArrow->changeAnimation(narrow);	
	sContArrow->setPosition(glm::vec2(SCREEN_WIDTH / 2 + 4* 32.f + 16.f, 32.f));

	sContBomb->changeAnimation(nbomb);
	sContBomb->setPosition(glm::vec2(SCREEN_WIDTH / 2 + 4 * 32.f + 16.f +56.f, 32.f));


	puntsheet.loadFromFile("images/puntuacion.png", TEXTURE_PIXEL_FORMAT_RGBA);
	puntuation = Game::instance().getPuntuation();
	int aux;
	for (int i = puntsprite.size() - 1; i >= 0; --i) {
		puntsprite[i] = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 1), &puntsheet, &texProgram);
		puntsprite[i]->setNumberAnimations(10);
		puntsprite[i]->setAnimationSpeed(0, 8);
		puntsprite[i]->addKeyframe(0, glm::vec2(0.f, 0.f));

		puntsprite[i]->setAnimationSpeed(1, 8);
		puntsprite[i]->addKeyframe(1, glm::vec2(1.f / 10.f, 0.f));

		puntsprite[i]->setAnimationSpeed(2, 8);
		puntsprite[i]->addKeyframe(2, glm::vec2(2.f / 10.f, 0.f));

		puntsprite[i]->setAnimationSpeed(3, 8);
		puntsprite[i]->addKeyframe(3, glm::vec2(3.f / 10.f, 0.f));

		puntsprite[i]->setAnimationSpeed(4, 8);
		puntsprite[i]->addKeyframe(4, glm::vec2(4.f / 10.f, 0.f));

		puntsprite[i]->setAnimationSpeed(5, 8);
		puntsprite[i]->addKeyframe(5, glm::vec2(5.f / 10.f, 0.f));

		puntsprite[i]->setAnimationSpeed(6, 8);
		puntsprite[i]->addKeyframe(6, glm::vec2(6.f / 10.f, 0.f));

		puntsprite[i]->setAnimationSpeed(7, 8);
		puntsprite[i]->addKeyframe(7, glm::vec2(7.f / 10.f, 0.f));

		puntsprite[i]->setAnimationSpeed(8, 8);
		puntsprite[i]->addKeyframe(8, glm::vec2(8.f / 10.f, 0.f));

		puntsprite[i]->setAnimationSpeed(9, 8);
		puntsprite[i]->addKeyframe(9, glm::vec2(9.f / 10.f, 0.f));

		aux = puntuation % 10;
		puntuation /= 10;
		puntsprite[i]->changeAnimation(aux);
		puntsprite[i]->setPosition(glm::vec2(64.f + 16.f*i,32.f));
	}


	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 1.f);
	currentTime = 0.0f;
}

void Scene::treatRoof(ShaderProgram &shaderProgram){
	int k = 0;

	int i = roof.size();
	int j = timeUp;

	while (k < j){
		if (k >= i){
			if (k < j){
				if (i == 0){
					roof.push_back(Sprite::createSprite(glm::ivec2(8 * 32, 28), glm::vec2(0.5, 0.5), &roofsheet, &shaderProgram));
					roof[k]->setNumberAnimations(1);
					roof[k]->setAnimationSpeed(0, 8);
					roof[k]->addKeyframe(0, glm::vec2(0.5f, 0.f));
					roof[k]->changeAnimation(0);
				}
				else {
					roof.push_back(Sprite::createSprite(glm::ivec2(8 * 32, 28), glm::vec2(0.5, 0.5), &roofsheet, &shaderProgram));
					roof[k]->setNumberAnimations(1);
					roof[k]->setAnimationSpeed(0, 8);
					roof[k]->addKeyframe(0, glm::vec2(0.f, 0.5f * (k % 2)));
					roof[k]->changeAnimation(0);
				}
			}
		}
		float y = (1+k) * 28.f;
		roof[k]->setPosition(glm::vec2(SCREEN_X, ( (j*28.f - y) + SCREEN_Y)));
		++k;
	}
}

void Scene::treatFall(ShaderProgram &shaderProgram){
	if (falling != -1){
		for (int i = 0; i < fallBalls.size(); ++i){
			glm::vec2 pos = fallBalls[i]->getPosition();
			fallBalls[i]->setPosition(glm::vec2(pos.x, pos.y + 8.f));
			if (pos.y > SCREEN_HEIGHT - 64.f){
				fallBalls[i]->setPosition(glm::vec2(pos.x, SCREEN_HEIGHT + 32.f));
			}
		}
		++falling;
		if (falling == 32){
			falling = -1;
			for (int i = 0; i < fallBalls.size(); ++i){
				fallBalls[i]->free();
			}
			fallBalls.clear();
		}
	}
}

void Scene::update(int deltaTime)
{
	points = 0;
	currentTime += deltaTime;
	player->update(deltaTime);
	restart = player->getRestart();
	throwball = player->getThrowBall();
	if (!busy)anglethrow = player->getTarget();
	treatFall(texProgram);
	if (!busy){
		treatRoof(texProgram);
		int thrown = player->getBallsThrown();
		timeUp = floor(thrown / 8.f);

		map->setIncrFloor(timeUp);
	}
	if (Game::instance().getKey(0x11)){
	}
	if ((Game::instance().getSpecialKey(112) && (Game::instance().getKey('w') || Game::instance().getKey('W'))) && !(instawin || instalose)){ //TECLA W TO WIN 
		instawin = true;
		stageClear = true;
		map->setStageClear(true);
		player->setStageClear(true);
	}
	if ((Game::instance().getSpecialKey(112) && (Game::instance().getKey('e') || Game::instance().getKey('E'))) && !(instawin || instalose)){ //TECLA E TO LOSE
		instalose = true;
		gameOver = true;
		map->setGameOver(true);
		player->setGameOver(true);
	}
	if (!busy && (Game::instance().getKey('b') || Game::instance().getKey('B')) && !(isbomb || isarrow)){ //TECLA b TO bomb
		if (nbomb > 0){
			isbomb = true;
		}
	}
	if (!busy && (Game::instance().getKey('a') || Game::instance().getKey('A')) && !(isbomb || isarrow)){ //TECLA a TO arrow
		if (narrow > 0){
			isarrow = true;
		}
	}
	if (!gameOver && !stageClear){

		if (throwball){
			//colorBall = 1;
			anglethrow = player->getTarget();
			spdx = sin(anglethrow)*SPEED_MODULE;
			spdy = cos(anglethrow)*SPEED_MODULE;
			throwball = false;
			busy = true;
			if (isbomb) {
				nbomb -= 1;
				sContBomb->changeAnimation(nbomb);
			}
			if (isarrow) {
				soundArr.play();
				narrow -= 1;
				sContArrow->changeAnimation(narrow);
			}
			player->setBusy(busy);
		}

		if (busy){
				if (!(isbomb || isarrow)){
					xBall += spdx;
					yBall += spdy;
					if (xBall <= -128 + 16 || xBall >= 128 - 16){
						xBall -= 2 * spdx;
						spdx = -spdx;
					}
					float xBallScr = MAP_X / 2 + xBall;
					float yBallScr = MAP_Y - yBall;
					glm::vec2 tile;
					if (yBallScr - 16 - timeUp * (32.f - 32.f / 8.f) <= 0.f){
						xBall = xBall + MAP_X / 2;
						yBall = MAP_Y - yBall;
						tile = ajustPosition(xBall, yBall);
						xBall = (xBall)-MAP_X / 2;
						yBall = MAP_Y - yBall;
						busy = false;
						player->setBusy(busy);
					}
					else {
						bool set = false;
						while (set == false){
							xBallScr = MAP_X / 2 + xBall;
							yBallScr = MAP_Y - yBall;
							if (map->collision(glm::vec2(xBallScr, yBallScr), 16, glm::vec2(spdx, spdy))){
								float xBallA = xBall + MAP_X / 2;
								float yBallA = MAP_Y - yBall;
								tile = ajustPosition(xBallA, yBallA);
								if (map->getColor(tile) == 0){
									xBall = (xBallA)-MAP_X / 2;
									yBall = MAP_Y - yBallA;
									busy = false;
									player->setBusy(busy);
									set = true;
								}
								else {
									xBall -= spdx / 8;
									yBall -= spdy / 8;
								}
							}
							else{
								set = true;
							}
						}
					}
					if (gameOver){
						map->setGameOver(true);
						player->setGameOver(true);
					}
					if (!busy){

						bool ret = map->addBall(tile, colorBall, texProgram);
						gameOver = map->getGameOver();
						if (!(isbomb || isarrow)){
							if (!gameOver){

								if (!ret) {
									soundSpr.play();
									player->sparkleLand(tile, colorBall, timeUp, texProgram);
								}
								else {
									soundExp.play();
									stack<glm::ivec2> explote = map->getExplode();
									stack<glm::ivec2> fallen = map->getFall();
									stack<int> colorFallen = map->getColorFall();
									player->explodeLand(explote, colorBall, timeUp, texProgram);
									points += explote.size() * 10;
									if (fallen.size() > 0){
										points += pow(2, fallen.size()) * 10;
										}
									if (fallen.size() > 0){
										falling = 0;
										fallLand(fallen, colorFallen, timeUp, texProgram);
									}

								}
								colorBall = player->getColor();
								xBall = 0;
								yBall = 0;
							}
							player->changeColor();
							stageClear = map->getStageClear();
						}
					}
					if (stageClear){
						if (currentTime <= 60000){
							if (currentTime <= 5000) points += 50000;
							else {
								float time = 60000 - (currentTime - 5000);
								points += floor(50000 * (time / 55000));
							}
						}
						player->setStageClear(stageClear);
						map->free();
						map->render();
					}
					if (gameOver){
						map->setGameOver(true);
						player->setGameOver(gameOver);
						map->processGameOver(texProgram);
						map->render();
					}
				}
				// TODO
				if (isbomb){
					xBall += spdx;
					yBall += spdy;
					if (xBall <= -128 + 16 || xBall >= 128 - 16){
						xBall -= 2 * spdx;
						spdx = -spdx;
					}
					float xBallScr = MAP_X / 2 + xBall;
					float yBallScr = MAP_Y - yBall;
					glm::vec2 tile;
					if (yBallScr - 16 - timeUp * (32.f - 32.f / 8.f) <= 0.f){
						xBall = xBall + MAP_X / 2;
						yBall = MAP_Y - yBall;
						tile = ajustPosition(xBall, yBall);
						xBall = (xBall)-MAP_X / 2;
						yBall = MAP_Y - yBall;
						busy = false;
						player->setBusy(busy);
					}
					else {
						bool set = false;
						while (set == false){
							xBallScr = MAP_X / 2 + xBall;
							yBallScr = MAP_Y - yBall;
							if (map->collision(glm::vec2(xBallScr, yBallScr), 16, glm::vec2(spdx, spdy))){
								float xBallA = xBall + MAP_X / 2;
								float yBallA = MAP_Y - yBall;
								tile = ajustPosition(xBallA, yBallA);
								if (map->getColor(tile) == 0){
									xBall = (xBallA)-MAP_X / 2;
									yBall = MAP_Y - yBallA;
									busy = false;
									player->setBusy(busy);
									set = true;
								}
								else {
									xBall -= spdx / 8;
									yBall -= spdy / 8;
								}
							}
							else{
								set = true;
							}
						}
					}
					if (gameOver){
						map->setGameOver(true);
						player->setGameOver(true);
					}
					if (!busy){

						bool ret = map->addBomb(tile, texProgram);
						if (!gameOver){
							player->exploteBomb(tile, timeUp, texProgram);
							if (ret) {
								soundExp.play();
								stack<glm::ivec2> explote = map->getExplode();
								stack<glm::ivec2> fallen = map->getFall();
								stack<int> colorFallen = map->getColorFall();
								player->explodeLand(explote, colorBall, timeUp, texProgram);
								points += explote.size() * 10;
								if (fallen.size() > 0){
									points += pow(2, fallen.size()) * 10;
								}
								if (fallen.size() > 0){
									falling = 0;
									fallLand(fallen, colorFallen, timeUp, texProgram);
								}
								isbomb = false;

							}
							soundBomb.play();
							colorBall = player->getColor();
							xBall = 0;
							yBall = 0;
						}
						player->changeColor();
						stageClear = map->getStageClear();

					}
					if (stageClear){
						if (currentTime <= 60000){
							if (currentTime <= 5000) points += 50000;
							else {
								float time = 60000 - (currentTime - 5000);
								points += floor(50000 * (time / 55000));
							}
						}
						player->setStageClear(stageClear);
						map->free();
						map->render();
					}
					if (gameOver){
						map->setGameOver(true);
						player->setGameOver(gameOver);
						map->processGameOver(texProgram);
						map->render();
					}
				}
				if (isarrow){
					xBall += spdx;
					yBall += spdy;
					float xBallScr = MAP_X / 2 + xBall;
					float yBallScr = MAP_Y - yBall;
					float xhead = xBallScr + spdx;
					float yhead = yBallScr - spdy;
					glm::vec2 tile;
					if (yhead - 8 - timeUp * (32.f - 32.f / 8.f) <= 0.f || (xBall+spdx <= -128 + 8 || xBall+spdx >= 128 - 8)){
						arrowLand(pierced,timeUp, texProgram);
						xBall = 0;
						yBall = 0;
						busy = false;
						player->setBusy(busy);

						isarrow = false;
					}
					else {
						bool set = false;
						xBallScr = MAP_X / 2 + xBall;
						yBallScr = MAP_Y - yBall;
						stack<glm::ivec2> exploteaux = map->collisionArrow(glm::vec2(xhead, yhead));
						while (!exploteaux.empty()){
							glm::vec2 aux = exploteaux.top();
							exploteaux.pop();
							pierced.push_back(make_pair(aux, map->getColor(aux)));
						}
					}
					if (gameOver){
						map->setGameOver(true);
						player->setGameOver(true);
					}
					if (!busy){
						if (!gameOver){
							if (pierced.size() == 0) {
								//player->sparkleLand(tile, colorBall, timeUp, texProgram);
							}
							else {
								soundExp.play();
								stack<glm::ivec2> explote = map->getExplode();
								stack<int> colorExplote = map->getColorExplodeArrow();
								stack<glm::ivec2> fallen = map->getFall();
								stack<int> colorFallen = map->getColorFall();
								player->explodeArrow(explote, colorExplote, timeUp, texProgram);
								points += explote.size() * 10;
								if (fallen.size() > 0){
									points += pow(2, fallen.size()) * 10;
								}
								if (fallen.size() > 0){
									falling = 0;
									fallLand(fallen, colorFallen, timeUp, texProgram);
								}
								pierced.clear();
							}
							colorBall = player->getColor();
							xBall = 0;
							yBall = 0;
						}
						player->changeColor();
						stageClear = map->getStageClear();
					}
					if (stageClear){
						if (currentTime <= 60000){
							if (currentTime <= 5000) points += 50000;
							else {
								float time = 60000 - (currentTime - 5000);
								points += floor(50000 * (time / 55000));
							}
						}
						player->setStageClear(stageClear);
						map->free();
						map->render();
					}
					if (gameOver){
						map->setGameOver(true);
						player->setGameOver(gameOver);
						map->processGameOver(texProgram);
						map->render();
					}
			}
		}
	}
	if (gameOver || stageClear){
		if (!ended){
			
			end = Sprite::createSprite(glm::ivec2(250, 64), glm::vec2(0.5f, 0.5f), &endsheet, &texProgram);
			end->setNumberAnimations(1);

			toCont = Sprite::createSprite(glm::ivec2(464, 16), glm::vec2(1.f, 0.5f), &toContsheet, &texProgram);
			toCont->setNumberAnimations(1);

			toCont->setAnimationSpeed(0, 4);
			toCont->addKeyframe(0, glm::vec2(0.0f, .0f));
			toCont->addKeyframe(0, glm::vec2(0.0f, .0f));
			toCont->addKeyframe(0, glm::vec2(0.0f, .5f));

			if (gameOver){
				soundLose.play();
				end->setAnimationSpeed(0, 8);
				end->addKeyframe(0, glm::vec2(0.0f, .5f));
				end->addKeyframe(0, glm::vec2(0.0f, .5f));
				end->addKeyframe(0, glm::vec2(0.5f, .5f));
			}
			if (stageClear){
				soundWin.play();
				end->setAnimationSpeed(0, 8);
				end->addKeyframe(0, glm::vec2(0.0f, .0f));
				end->addKeyframe(0, glm::vec2(0.0f, .0f));
				end->addKeyframe(0, glm::vec2(0.5f, .0f));
			}

			toCont->setPosition(glm::vec2(SCREEN_WIDTH / 2 - 232, SCREEN_HEIGHT / 2 - 32 + 64 + 8));
			end->setPosition(glm::vec2(SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT / 2 - 32));
			toCont->changeAnimation(0);
			end->changeAnimation(0);
			ended = true;
		}
		else {
			end->update(deltaTime);
			toCont->update(deltaTime);
		}
	}
	if (restart){
		if (stageClear){
			level += 1;
			if (level > 5){
				endgame = true;
			}
			else {
				init();
			}
		}
		restart = false;
		if (gameOver) {
			if (credits == 0) {
				endgame = true;
			}
			else{
				--credits;
				init();
			}
		}		
	}
	puntuation = Game::instance().getPuntuation() + points;
	int aux;
	for (int i = puntsprite.size() - 1; i >= 0; --i) {
		aux = puntuation % 10;
		puntsprite[i]->changeAnimation(aux);
		if (puntuation > 9){
			puntuation /= 10;
		}
		else {
			puntuation = 0;
		}
	}
}

glm::vec2 Scene::ajustPosition(float &x, float &y){
	int ytile = (y) / (32 - 32/8) - timeUp;

	int xtile;
	xtile = ((x + ((-16)*(ytile%2))) / 32);
	x = 16 + 32 * xtile + 16*(ytile%2);		
	y = 16 + (ytile * 32) - (ytile*32 / 8);

	if (ytile+timeUp >= 11) gameOver = true;
	return glm::vec2(xtile, ytile);
}


void Scene::fallLand(stack<glm::ivec2> tiles,stack<int>colorFallen, int incrFloor, ShaderProgram &shaderProgram){

	int i = 0;
	while (!tiles.empty()){
		glm::ivec2 tile = tiles.top();
		int color = colorFallen.top() - 1 ;
		tiles.pop();
		colorFallen.pop();
		fallBalls.push_back(Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 0.125), &fallsheet, &shaderProgram));
		fallBalls[i]->setNumberAnimations(1);
		fallBalls[i]->setAnimationSpeed(0, 8);
		fallBalls[i]->addKeyframe(0, glm::vec2(0.0f, color*.125));
		fallBalls[i]->changeAnimation(0);
		float x = 16.f + 32.f * tile.x + 16.f * ((tile.y) % 2);
		float y = 16.f + (tile.y * 32.f) - (tile.y * 32.f / 8.f) + incrFloor * (32.f - 32.f / 8);
		fallBalls[i]->setPosition(glm::vec2((x + 192 - 16), (y + 64 - 16)));
		++i;
	}
	falling = 0;
}


void Scene::arrowLand(vector<pair<glm::ivec2, int>> pierced, int incrFloor, ShaderProgram &texProgram){
	map->clearArrow(pierced, incrFloor, texProgram);
}

void Scene::render()
{
	glm::mat4 modelview;
	
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(texBackground);

	
	if (!stageClear){
		map->render();
	}
	
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3((SCREEN_WIDTH / 2) - 128.f, SCREEN_HEIGHT - 100.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	line->render(texLine);
	
	for (int i = 0; i < fallBalls.size(); ++i){
		fallBalls[i]->render();
	}
	player->render1();


	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(SCREEN_X, SCREEN_Y + (32.f * timeUp), 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	for (int i = 0; i < roof.size(); ++i){
		roof[i]->render();
	}

	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT - 64, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	tube->render(texTube);

	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3((SCREEN_WIDTH/2)-22.f, SCREEN_HEIGHT-112.f - 32.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(22.f, 64.f, 0.f));
	float angle = player->getTarget();
	modelview = glm::rotate(modelview, angle, glm::vec3(0.f, 0.f, 1.f));
	modelview = glm::translate(modelview, glm::vec3(-22.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	arrow->render(texArrow);
	player->render2();

	glm::vec2 geomball[2] = { glm::vec2(0.f, 0.f), glm::vec2(32.f, 32.f) };
	if (!gameOver) colorBall = player->getColor();
	else colorBall = 6;
	ball->changeAnimation(colorBall - 1);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	if (!(isbomb || isarrow)){
		ball->setPosition(glm::vec2((SCREEN_WIDTH / 2) + xBall - MARGIN_SCREEN, SCREEN_HEIGHT - yBall - 48.f - 32.f - 16.f));
		if (!gameOver){
			ball->render();
		}
	}
	if (isarrow){
		sArrow->setPosition(glm::vec2((SCREEN_WIDTH / 2) + xBall - MARGIN_SCREEN + 8.f, SCREEN_HEIGHT - yBall - 48.f - 32.f - 34.f));
		if (!throwball)
		sArrow->renderAngle(anglethrow);
	}
	if (isbomb){
		sBomb->setPosition(glm::vec2((SCREEN_WIDTH / 2) + xBall - MARGIN_SCREEN, SCREEN_HEIGHT - yBall - 48.f - 32.f - 16.f));
		sBomb->render();
	}

	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(0, SCREEN_HEIGHT - 32.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	bottomBar->render(texBottomBar);

	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(SCREEN_WIDTH/2 - 64.f, SCREEN_HEIGHT - 32.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	round->render(texRound);

	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(SCREEN_WIDTH -120-16, SCREEN_HEIGHT - 32.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	credit->render(texCredit);

	glm::vec2 geomLevel[2] = { glm::vec2(0.f, 0.f), glm::vec2(32.f, 32.f) };
	showlevel->changeAnimation(level - 1);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	for (int i = 0; i < puntsprite.size(); ++i){
		puntsprite[i]->render();
	}

	sContArrow->render();
	sContBomb->render();

	if (currentTime < 2000.f){
		showlevel->render();
	}

	if (gameOver || stageClear){
		end->render();
		toCont->render();
	}
}

void Scene::setGameOver(bool over){
	gameOver = over;
}

void Scene::setEndgame(bool end){
	endgame = end;
}

void Scene::setLevel(int lvl){
	level = lvl;
}

void Scene::setCredits(int crd){
	credits = crd;
}

void Scene::setPoints(int point){
	points = point;
}

int Scene::getPoints(){
	return points;
}

int Scene::getRound(){
	return level;
}


void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



