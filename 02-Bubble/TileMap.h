#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <stack>
#include <vector>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, int timeUp, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, int timeUp, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }
	int getStageClear() const { return stageClear; }
	int getGameOver() const { return gameOver; }

	stack<glm::ivec2> getExplode();
	stack<int> getColorExplodeArrow();
	stack<glm::ivec2> getFall();
	stack<int> getColorFall();

	bool collision(glm::vec2 center, const int radius, glm::vec2 &speed) const;
	stack<glm::ivec2> collisionArrow(glm::vec2 center);

	bool addBall(glm::ivec2 tile, int color, ShaderProgram &program);
	bool addBomb(glm::ivec2 tile, ShaderProgram &program);
	int getBalls(int type);
	int getColor(const glm::ivec2 &tile);
	void setIncrFloor(int timeUp);
	void processGameOver(ShaderProgram &program);
	void setGameOver(bool Over);
	void setStageClear(bool clear);
	void clearArrow(vector<pair<glm::ivec2, int>> pierced, int incrFloor, ShaderProgram &program);
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	bool checkDelete(glm::vec2 tile, int color);
	bool checkBomb(glm::vec2 tile);
	void cleanBalls(stack<glm::ivec2> s);
	void cleanMap();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	glm::vec2 mMinCoords;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	bool *mapCheck;
	bool *mapFall;
	int maxLine;
	int incrFloor;
	int posOver;
	bool gameOver,stageClear;
	int *balls;
	stack<glm::ivec2> ballsExpl, ballsFall;
	stack<int> colorFall, colorExplote;
};


#endif // _TILE_MAP_INCLUDE


