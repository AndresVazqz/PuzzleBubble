#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Manual.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <SFML\Audio.hpp>


void Manual::init(){
	initShaders();
	start = false;


	manualsheet.loadFromFile("images/manual.png", TEXTURE_PIXEL_FORMAT_RGBA);
	manual = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), &manualsheet, &texProgram);
	manual->setNumberAnimations(1);

	manual->setAnimationSpeed(0, 8);
	manual->addKeyframe(0, glm::vec2(.0f, 0.f));
	manual->changeAnimation(0);
	manual->setPosition(glm::vec2(0.f, 0.f));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	if (music.openFromFile("music/helpsound.wav")){
		music.play();
		music.setLoop(true);
	}
	else{

		cout << "No se ha podido cargar la banda sonora" << endl;
	}
}

void Manual::update(int deltaTime)
{
	currentTime += deltaTime;
	manual->update(deltaTime);
	if (Game::instance().getKey(13))
	{
		start = true;
		music.setVolume(0);
	}
}

void Manual::setStart(bool strt)
{
	start = strt;
}

void Manual::render()
{
	glm::mat4 modelview;
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	manual->render();
}

void Manual::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

Manual::Manual()
{
}


Manual::~Manual()
{
}
