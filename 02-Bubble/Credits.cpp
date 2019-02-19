#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Credits.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <SFML\Audio.hpp>

void Credits::init(bool wina, int puntuation, int round)
{
	srand(time(NULL));

	currentTime = 0.f;
	initShaders();
	start = false;
	win = wina;
	points = puntuation;

	glm::vec2 geomRoundFinal[2] = { glm::vec2(0.f, 0.f), glm::vec2(160, 32) };
	glm::vec2 texCoordsRoundFinal[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	roundFinal = TexturedQuad::createTexturedQuad(geomRoundFinal, texCoordsRoundFinal, texProgram);
	texRoundFinal.loadFromFile("images/roundfinal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texRoundFinal.setMagFilter(GL_NEAREST);

	glm::vec2 geomPuntuationFinal[2] = { glm::vec2(0.f, 0.f), glm::vec2(160, 32) }; 
	glm::vec2 texCoordsPuntuationFinal[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	puntuationFinal = TexturedQuad::createTexturedQuad(geomPuntuationFinal, texCoordsPuntuationFinal, texProgram);
	texPuntuationFinal.loadFromFile("images/score.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texPuntuationFinal.setMagFilter(GL_NEAREST);

	if (win){
		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		texBackground.loadFromFile("images/backgroundwin.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texBackground.setMagFilter(GL_NEAREST);

		glm::vec2 geomwinfloor[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
		glm::vec2 texCoordsWinFloor[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		winFloor = TexturedQuad::createTexturedQuad(geom, texCoordsWinFloor, texProgram);
		texWinFloor.loadFromFile("images/winfloor.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texWinFloor.setMagFilter(GL_NEAREST);


		glm::vec2 geomNames[2] = { glm::vec2(0.f, 0.f), glm::vec2(223, 32) };
		glm::vec2 texCoordsNames[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		names = TexturedQuad::createTexturedQuad(geomNames, texCoordsNames, texProgram);
		texNames.loadFromFile("images/names.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texNames.setMagFilter(GL_NEAREST);


		actionsheet.loadFromFile("images/ernestoaction.png", TEXTURE_PIXEL_FORMAT_RGBA);
		ernestoaction = Sprite::createSprite(glm::ivec2(100, 94), glm::vec2(0.2, 0.2), &actionsheet, &texProgram);
		ernestoaction->setNumberAnimations(1);

		ernestoaction->setAnimationSpeed(0, 6);


		ernestoaction->addKeyframe(0, glm::vec2(.0f, 0.6f));
		ernestoaction->addKeyframe(0, glm::vec2(.2f, 0.6f));
		ernestoaction->addKeyframe(0, glm::vec2(.4f, 0.6f));
		ernestoaction->addKeyframe(0, glm::vec2(.6f, 0.6f));

		ernestoaction->changeAnimation(0);
		ernestoaction->setPosition(glm::vec2(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 + 40));


		actionsheet.loadFromFile("images/ernestoaction.png", TEXTURE_PIXEL_FORMAT_RGBA);
		felipeaction = Sprite::createSprite(glm::ivec2(100, 94), glm::vec2(0.2, 0.2), &actionsheet, &texProgram);
		felipeaction->setNumberAnimations(1);

		felipeaction->setAnimationSpeed(0, 6);
		felipeaction->addKeyframe(0, glm::vec2(.0f, 0.0f));
		felipeaction->addKeyframe(0, glm::vec2(.2f, 0.0f));
		felipeaction->addKeyframe(0, glm::vec2(.4f, 0.0f));
		//felipeaction->addKeyframe(0, glm::vec2(.6f, 0.0f));
		//felipeaction->addKeyframe(0, glm::vec2(.8f, 0.0f));

		//felipeaction->addKeyframe(0, glm::vec2(.0f, 0.4f));
		felipeaction->addKeyframe(0, glm::vec2(.2f, 0.4f));
		felipeaction->addKeyframe(0, glm::vec2(.4f, 0.4f));
		felipeaction->addKeyframe(0, glm::vec2(.6f, 0.4f));
		felipeaction->addKeyframe(0, glm::vec2(.8f, 0.4f));

		//felipeaction->addKeyframe(0, glm::vec2(.0f, 0.8f));
		//felipeaction->addKeyframe(0, glm::vec2(.2f, 0.8f));

		//felipeaction->addKeyframe(0, glm::vec2(.0f, 0.2f));
		//felipeaction->addKeyframe(0, glm::vec2(.2f, 0.2f));
		//felipeaction->addKeyframe(0, glm::vec2(.4f, 0.2f));
		felipeaction->addKeyframe(0, glm::vec2(.6f, 0.2f));
		felipeaction->addKeyframe(0, glm::vec2(.8f, 0.2f));

		//felipeaction->addKeyframe(0, glm::vec2(.4f, 0.8f));

		felipeaction->changeAnimation(0);
		felipeaction->setPosition(glm::vec2(SCREEN_WIDTH / 2 + 130, SCREEN_HEIGHT / 2 + 40));

		bobblewin2sheet.loadFromFile("images/bobblewin2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		bubs = vector<Sprite*>(8);
		bubsvar = vector<glm::ivec4>(8);
		for (int i = 0; i < bubs.size(); ++i) {
			bubs[i] = Sprite::createSprite(glm::ivec2(98, 98), glm::vec2(0.1f, 0.125f), &bobblewin2sheet, &texProgram);
			bubs[i]->setNumberAnimations(3);

			bubs[i]->setAnimationSpeed(0, 12);
			bubs[i]->addKeyframe(0, glm::vec2(0.0f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.1f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.2f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.3f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.4f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.5f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.6f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.7f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.8f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.9f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.9f, i*0.125f));
			bubs[i]->addKeyframe(0, glm::vec2(0.9f, i*0.125f));

			bubs[i]->setAnimationSpeed(1, 12);
			bubs[i]->addKeyframe(1, glm::vec2(0.f, i*0.125f));

			bubs[i]->setAnimationSpeed(2, 12);
			bubs[i]->addKeyframe(2, glm::vec2(0.9f, i*0.125f));

			bubs[i]->changeAnimation(1);

			bubsvar[i].x = (rand() % SCREEN_WIDTH); //inicio x
			bubsvar[i].y = (rand() % (SCREEN_HEIGHT / 2 + 40 + 82)); //final y

			bubsvar[i].z = bubsvar[i].x; //posicion en las x
			bubsvar[i].w = (SCREEN_HEIGHT / 2 + 40 + 82 + (i * 20)); //posicion en las y

			bubs[i]->setPosition(glm::vec2(bubsvar[i].x, bubsvar[i].w));
		}

		if (music.openFromFile("music/soundwin.wav")){
			music.play();
			music.setLoop(true);
		}
		else{

			cout << "No se ha podido cargar la banda sonora" << endl;
		}
	}
	else {
		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		texBackground.loadFromFile("images/backgroundlose.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
		texBackground.setMagFilter(GL_NEAREST);

		glm::vec2 geomNames[2] = { glm::vec2(0.f, 0.f), glm::vec2(446, 64) };
		glm::vec2 texCoordsNames[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		names = TexturedQuad::createTexturedQuad(geomNames, texCoordsNames, texProgram);
		texNames.loadFromFile("images/names.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texNames.setMagFilter(GL_NEAREST);

		backlosesheet.loadFromFile("images/backgroundlose.png", TEXTURE_PIXEL_FORMAT_RGBA);
		backlose = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 0.125), &backlosesheet, &texProgram);
		backlose->setNumberAnimations(1);

		backlose->setAnimationSpeed(0, 8);
		backlose->addKeyframe(0, glm::vec2(0.f, .0f));
		backlose->addKeyframe(0, glm::vec2(0.f, .125f));
		backlose->addKeyframe(0, glm::vec2(0.f, .25f));
		backlose->addKeyframe(0, glm::vec2(0.f, .375f));
		backlose->addKeyframe(0, glm::vec2(0.f, .5f));
		backlose->addKeyframe(0, glm::vec2(0.f, .625f));
		backlose->addKeyframe(0, glm::vec2(0.f, .75f));
		backlose->addKeyframe(0, glm::vec2(0.f, .875f));
		backlose->addKeyframe(0, glm::vec2(0.f, .75f));
		backlose->addKeyframe(0, glm::vec2(0.f, .625f));
		backlose->addKeyframe(0, glm::vec2(0.f, .5f));
		backlose->addKeyframe(0, glm::vec2(0.f, .375f));
		backlose->addKeyframe(0, glm::vec2(0.f, .25f));
		backlose->addKeyframe(0, glm::vec2(0.f, .125f));

		backlose->changeAnimation(0);
		backlose->setPosition(glm::vec2(0, 0));

		oversheet.loadFromFile("images/gameover.png", TEXTURE_PIXEL_FORMAT_RGBA);
		over = Sprite::createSprite(glm::ivec2(374, 42), glm::vec2(1, 0.125f), &oversheet, &texProgram);
		over->setNumberAnimations(1);

		over->setAnimationSpeed(0, 16);
		over->addKeyframe(0, glm::vec2(0.f, .0f));
		over->addKeyframe(0, glm::vec2(0.f, .125f));
		over->addKeyframe(0, glm::vec2(0.f, .25f));
		over->addKeyframe(0, glm::vec2(0.f, .375f));
		over->addKeyframe(0, glm::vec2(0.f, .5f));
		over->addKeyframe(0, glm::vec2(0.f, .625f));
		over->addKeyframe(0, glm::vec2(0.f, .75f));
		over->addKeyframe(0, glm::vec2(0.f, .875f));
		over->addKeyframe(0, glm::vec2(0.f, .75f));
		over->addKeyframe(0, glm::vec2(0.f, .625f));
		over->addKeyframe(0, glm::vec2(0.f, .5f));
		over->addKeyframe(0, glm::vec2(0.f, .375f));
		over->addKeyframe(0, glm::vec2(0.f, .25f));
		over->addKeyframe(0, glm::vec2(0.f, .125f));

		over->changeAnimation(0);
		over->setPosition(glm::vec2(SCREEN_WIDTH / 2 - 187, SCREEN_HEIGHT / 2 + 40));

		roundNum = Sprite::createSprite(glm::ivec2(28, 32), glm::vec2(0.1, 1), &puntfinalsheet, &texProgram);
		roundNum->setNumberAnimations(1);

		roundNum->setAnimationSpeed(0, 8);
		int aux = round;
		roundNum->addKeyframe(0, glm::vec2(aux / 10.f, 0.f));
		roundNum->changeAnimation(0);

		roundNum->setPosition(glm::vec2(161.f, 80.f));

		if (music.openFromFile("music/soundwin.wav")){
			music.play();
			music.setLoop(true);
		}
		else{

			cout << "No se ha podido cargar la banda sonora" << endl;
		}
	}
	puntsprite = vector<Sprite*>(8);

	puntfinalsheet.loadFromFile("images/puntfinal.png", TEXTURE_PIXEL_FORMAT_RGBA);

	int aux;
	for (int i = puntsprite.size() - 1; i >= 0; --i) {
		puntsprite[i] = Sprite::createSprite(glm::ivec2(28, 32), glm::vec2(0.1, 1), &puntfinalsheet, &texProgram);
		puntsprite[i]->setNumberAnimations(1);

		puntsprite[i]->setAnimationSpeed(0, 8);
		aux = points % 10;
		puntsprite[i]->addKeyframe(0, glm::vec2(aux / 10.f, 0.f));
		points /= 10;
		puntsprite[i]->changeAnimation(0);
		if (win){
			puntsprite[i]->setPosition(glm::vec2(SCREEN_WIDTH / 2 - 30*4 + 30 * i, 32.f + 80.f));
		}
		else {
			puntsprite[i]->setPosition(glm::vec2(SCREEN_WIDTH / 2 + 64.f + 30 * i -16, 80.f));
		}
		//puntsprite[i]->setPosition(glm::vec2(SCREEN_WIDTH / 2 - 30 * (8 - i) + (30 * 4), SCREEN_HEIGHT / 2 + 32.f));
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Credits::update(int deltaTime)
{
	currentTime += deltaTime;
	if (Game::instance().getKey(13) && currentTime >= 1000){
		start = true;
		music.stop();
	}
	if (win) {
		ernestoaction->update(deltaTime);
		felipeaction->update(deltaTime);
		for (int i = 0; i < bubs.size(); ++i) {
			bubs[i]->update(deltaTime);
		}
	}
	else {
		backlose->update(deltaTime);
		over->update(deltaTime);
	}
	for (int i = 0; i < bubs.size(); ++i) {

		//x=inicio x   ---  y=final y --- z=actual x --- t=actual y

		bubsvar[i].w -= 2;
		bubsvar[i].z = sin(bubsvar[i].w / 20) * 20 + bubsvar[i].x;

		if (bubsvar[i].w <= bubsvar[i].y && bubs[i]->animation() != 0) {
			bubs[i]->changeAnimation(0);

		}
		if (bubsvar[i].w <= bubsvar[i].y - 64 && bubs[i]->animation() != 1) {

			bubs[i]->changeAnimation(1);
			bubsvar[i].x = (rand() % (SCREEN_WIDTH));
			bubsvar[i].y = (rand() % (SCREEN_HEIGHT / 2 + 40 + 82));
			bubsvar[i].w = SCREEN_HEIGHT / 2 + 40 + 82;
		}
		bubs[i]->setPosition(glm::vec2(bubsvar[i].z, bubsvar[i].w));
	}
}

void Credits::setStart(bool strt)
{
	start = strt;
}


void Credits::render()
{
	glm::mat4 modelview;
	modelview = glm::mat4(1.0f);

	if (win){
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
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		for (int i = 0; i < bubs.size(); ++i) {
			bubs[i]->render();
		}


		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(0.f, SCREEN_HEIGHT / 2 + 40 + 82, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		winFloor->render(texWinFloor);

		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		ernestoaction->render();

		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		felipeaction->render();

		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(8.f, SCREEN_HEIGHT-40.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		names->render(texNames);

		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else{
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);



		texProgram.use();

		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		backlose->render();

		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		over->render();

		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(32, 32, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		roundFinal->render(texRoundFinal);
		roundNum->render();
		//modelview = glm::mat4(1.0f);
		//modelview = glm::translate(modelview, glm::vec3((SCREEN_WIDTH / 2) - 192, SCREEN_HEIGHT / 2 - 160, 0.f));
		//texProgram.setUniformMatrix4f("modelview", modelview);
		//texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		//icon->render(texIcon);
	}

	modelview = glm::mat4(1.0f);
	if (win){
		modelview = glm::translate(modelview, glm::vec3(SCREEN_WIDTH/2 - 80, 64, 0.f));
	}
	else {
		modelview = glm::translate(modelview, glm::vec3(SCREEN_WIDTH - 32 - 160, 32, 0.f));
	}
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	puntuationFinal->render(texPuntuationFinal);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	for (int i = 0; i < puntsprite.size(); ++i){

		puntsprite[i]->render();
	}
}

void Credits::initShaders()
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


Credits::Credits()
{
}


Credits::~Credits()
{
}
