#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "TexturedQuad.h"
#include <SFML\Audio.hpp>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	int nbomb, narrow;
	void init();
	void update(int deltaTime);
	void render();
	void setGameOver(bool over);
	void setEndgame(bool end);
	void setLevel(int lvl);
	void setCredits(int crd);
	void setPoints(int point);
	int getPoints();
	int getRound();
	void treatFall(ShaderProgram &shaderProgram);
	void treatRoof(ShaderProgram &shaderProgram);
	void fallLand(stack<glm::ivec2> fallen, stack<int> colorFallen, int incrFloor, ShaderProgram &shaderProgram);
	void arrowLand(vector<pair<glm::ivec2, int>> pierced, int incrFloor, ShaderProgram &shaderProgram);

	glm::vec2 ajustPosition(float &x,float &y);
	bool getEndgame(){
		return endgame;
	}
	bool getStageClear(){
		return stageClear;
	}
	bool getGameOver(){
		return gameOver;
	}

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime, xBall, yBall, anglethrow;
	glm::mat4 projection;
	TexturedQuad *arrow;
	Texture texArrow;
	Sprite *ball;
	Texture ballsheet;
	Sprite *showlevel;
	Texture levelsheet;
	TexturedQuad *bottomBar;
	Texture texBottomBar;
	TexturedQuad *credit;
	Texture texCredit;

	TexturedQuad *line;
	Texture texLine;
	TexturedQuad *tube;
	Texture texTube;
	TexturedQuad *background;
	Texture texBackground;
	TexturedQuad *round;
	Texture texRound;
	Texture puntsheet;
	Texture roofsheet;
	vector<Sprite*> roof;
	vector<Sprite*> puntsprite;
	vector<pair<glm::ivec2, int>> pierced;
	Texture endsheet;
	Sprite *end;
	Texture toContsheet;
	Sprite *toCont;
	Texture sarrowsheet;
	Texture sbombsheet;
	Sprite *sBomb;
	Sprite *sArrow;
	Texture specialContsheet;
	Sprite *sContBomb;
	Sprite *sContArrow;
	float spdx, spdy;
	bool throwball, ended;
	bool gameOver, stageClear;
	bool isarrow, isbomb;
	bool busy;
	int colorBall;
	bool restart;
	int level;
	int credits;
	int timeUp, falling;
	int points;
	int puntuation;
	bool endgame;
	bool instawin, instalose;
	Texture fallsheet;
	vector<Sprite*> fallBalls;
	sf::SoundBuffer buffer;
	sf::SoundBuffer bufferExp;
	sf::SoundBuffer bufferSpr;
	sf::SoundBuffer bufferFloor;
	sf::SoundBuffer bufferWin;
	sf::SoundBuffer bufferLose;
	sf::SoundBuffer bufferBomb;
	sf::SoundBuffer bufferArr;
	sf::Sound sound;
	sf::Sound soundExp;
	sf::Sound soundSpr;
	sf::Sound soundFloor;
	sf::Sound soundWin;
	sf::Sound soundArr;
	sf::Sound soundBomb;
	sf::Sound soundLose;
};


#endif // _SCENE_INCLUDE

