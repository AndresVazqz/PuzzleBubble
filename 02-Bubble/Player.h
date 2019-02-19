#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <SFML\Audio.hpp>

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render1();
	void render2();
	void sparkleLand(const glm::ivec2 &tile, int color, int incrFloor, ShaderProgram &shaderProgram);
	void explodeLand(stack<glm::ivec2> tiles, int color, int incrFloor, ShaderProgram &shaderProgram);

	void explodeArrow(stack<glm::ivec2> tiles, stack<int> colors, int incrFloor, ShaderProgram &shaderProgram);
	void setGameOver(bool over);
	void setTileMap(TileMap *tileMap);
	void setBusy(bool bussy);
	void setPosition(const glm::vec2 &pos);
	void setStageClear(bool clear);
	float getTarget();
	float getActualAngle();
	bool getThrowBall();
	bool getRestart();
	int getBallsThrown();
	int getColor();
	void changeColor();
	void exploteBomb(glm::ivec2 tile, int incrFloor, ShaderProgram &shaderProgram);

private:
	bool throwBall, busy, gameOver, stageClear,restart, landed, exploded, pointing, explodedBomb;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, timedrop, timedropLanded, timedropExplode, timedropBomb;
	int colorBall, downColorBall, ballsThrown;
	float targetPoint, actualAngle, lastTime; 
	Texture bagsheet;
	Texture cannonsheet;
	Texture cranksheet;
	Texture ernestosheet;
	Texture felipesheet;
	Texture arksheet;
	Texture ballssheet;
	Texture sparklesheet;
	Texture explodesheet;
	Texture pointssheet;
	Texture hurrysheet;
	Texture bombexplsheet;
	Sprite *bagup;
	Sprite *bagdown;
	Sprite *cannon;
	Sprite *crank;
	Sprite *ernesto;
	Sprite *felipe;
	Sprite *ark;
	Sprite *balls;
	Sprite *sparkle;
	Sprite *hurry;
	Sprite *explodeBomb;
	vector<Sprite*> explodeBalls;
	vector<Sprite*> pointsBalls;
	TileMap *map;
	sf::Sound soundThrow;
	sf::SoundBuffer bufferThrow;

};


#endif // _PLAYER_INCLUDE


