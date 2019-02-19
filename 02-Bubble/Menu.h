#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include <SFML\Audio.hpp>

class Menu
{
public:
	Menu();
	~Menu();
	void init();
	void update(int deltaTime);
	void render();
	bool getStart(){ return start; };
	bool getH(){ return help; };
	void setStart(bool strt);
	void setH(bool strt);
	sf::Music music;
 
private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime, xBall, yBall;
	glm::mat4 projection;
	TexturedQuad *background, *icon;
	Texture texBackground, texIcon;
	Texture toContsheet;
	Sprite *toCont;
	

	bool start, help;

};


#endif // _MENU_INCLUDE

