#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Menu.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <SFML\Audio.hpp>
#define SCREEN_X 96*2
#define SCREEN_Y 32*2
#define MARGIN_SCREEN 16.f
#define MAP_X 32*8
#define MAP_Y 32*11
#define INIT_PLAYER_X_TILES 6
#define INIT_PLAYER_Y_TILES 2

#define SPEED_MODULE 12


void Menu::init()
{
	initShaders();
	start = false;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texBackground.loadFromFile("images/backgroundmenu.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	texBackground.setMagFilter(GL_NEAREST);

	glm::vec2 geomIcon[2] = { glm::vec2(0.f, 0.f), glm::vec2(384, 284) };
	glm::vec2 texCoordsIcon[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	icon = TexturedQuad::createTexturedQuad(geomIcon, texCoordsIcon, texProgram);
	texIcon.loadFromFile("images/puzzleBobble.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texIcon.setMagFilter(GL_NEAREST);

	toContsheet.loadFromFile("images/tocontinue.png", TEXTURE_PIXEL_FORMAT_RGBA);

	toCont = Sprite::createSprite(glm::ivec2(464, 16), glm::vec2(1.f, 0.5f), &toContsheet, &texProgram);
	toCont->setNumberAnimations(1);

	toCont->setAnimationSpeed(0, 4);
	toCont->addKeyframe(0, glm::vec2(0.0f, .0f));
	toCont->addKeyframe(0, glm::vec2(0.0f, .0f));
	toCont->addKeyframe(0, glm::vec2(0.0f, .5f));

	toCont->setPosition(glm::vec2(SCREEN_WIDTH / 2 - 232, SCREEN_HEIGHT / 2 + 64 + 64 + 8));
	toCont->changeAnimation(0);
	
	if(music.openFromFile("music/soundtrack.wav")){
		music.play();
		music.setLoop(true);
	}
	else{
		
		cout << "No se ha podido cargar la banda sonora" << endl;
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	if (Game::instance().getKey(13) && currentTime > 1000)
	{
		start = true;

		music.setVolume(30);
	}
	if ((Game::instance().getKey('h') || Game::instance().getKey('H')) && currentTime > 1000)
	{
		help = true;
		music.stop();
	}
	toCont->update(deltaTime);
}

void Menu::setStart(bool strt)
{
	start = strt;
}

void Menu::setH(bool strt)
{
	help = strt;
}

void Menu::render()
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

	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3((SCREEN_WIDTH / 2) - 192, SCREEN_HEIGHT/2 - 160, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	icon->render(texIcon);

	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3((SCREEN_WIDTH / 2) - 223, SCREEN_HEIGHT / 2 + 140, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	toCont->render();
}

void Menu::initShaders()
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


Menu::Menu()
{
}


Menu::~Menu()
{
}
