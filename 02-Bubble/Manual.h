#ifndef _MANUAL_INCLUDE
#define _MANUAL_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include <SFML\Audio.hpp>

class Manual
{
public:
	Manual();
	~Manual();
	void Manual::init();
	void update(int deltaTime);
	bool getStart(){ return start; };
	void setStart(bool strt);
	void render();

private:
	void initShaders();

private:
	bool start;
	float currentTime;
	ShaderProgram texProgram;
	glm::mat4 projection;
	Texture manualsheet;
	Sprite *manual;
	sf::Music music;

};

#endif