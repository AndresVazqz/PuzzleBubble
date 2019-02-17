#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include <SFML\Audio.hpp>

class Credits
{
public:
	Credits();
	~Credits();
	void init(bool win, int puntuation, int );
	void update(int deltaTime);
	void render();
	bool getStart(){ return start; };
	void setStart(bool strt);

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	TexturedQuad *background;
	Texture texBackground;
	TexturedQuad *roundFinal;
	Texture texRoundFinal;
	TexturedQuad *puntuationFinal;
	Texture texPuntuationFinal;
	Texture texNames;
	TexturedQuad *names;
	Texture texWinFloor;
	TexturedQuad *winFloor;
	sf::Music music;
	Texture actionsheet;
	Sprite *ernestoaction;
	Sprite *felipeaction;
	Sprite *roundNum;

	Texture backlosesheet;
	Sprite *backlose;
	Texture oversheet;
	Sprite *over;
	Texture bobblewin2sheet;
	int points;
	Texture puntfinalsheet;
	bool start, win;
	vector<Sprite*> puntsprite;

	vector<Sprite*> bubs;
	vector<glm::ivec4> bubsvar;

	sf::Sound soundLose;
	sf::SoundBuffer bufferLose;

};


#endif // _CREDITS_INCLUDE

