#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <queue>
#include "TileMap.h"


using namespace std;


#define INIT_X 190
#define INIT_Y 62


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, int timeUp, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, timeUp, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, int timeUp, ShaderProgram &program)
{
	mMinCoords = minCoords;
	gameOver = false;
	stageClear = false;
	incrFloor = timeUp;
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	balls = new int[9];
	for (int i = 0; i <= 8; ++i){
		balls[i] = 0;
	}
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> maxLine;
	mapSize.y = 13;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	map = new int[mapSize.x * mapSize.y];
	mapCheck = new bool[mapSize.x * mapSize.y];
	mapFall = new bool[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < (mapSize.x); i++)
		{
			fin.get(tile);
			if (tile == ' '){
				map[(j)*mapSize.x + i] = 0;
			}
			else{
				int ballType = tile - int('0');
				balls[ballType] += 1;
				balls[0] += 1;
				map[(j)*mapSize.x + i] = ballType;
			}
			if (j >= maxLine) map[(j)*mapSize.x + i] = 0;
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	int antMaxLine = maxLine;
	maxLine = 0;
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<(mapSize.x); i++)
		{
			if (!(gameOver || antMaxLine >= 11 - incrFloor )){
				tile = map[j * (mapSize.x) + i];
				if (j > maxLine && tile != 0) maxLine = j;
			}
			else {
				gameOver = true;
				if (map[j * (mapSize.x) + i] != 0){
					tile = 6;
				}
				else tile = 0;
			}
			if(tile != 0)
			{
				nTiles++;
				if (j % 2 == 0){
					posTile = glm::vec2(minCoords.x + i * tileSize, (minCoords.y + j * (tileSize - tileSize/8)) + (incrFloor*(32.f - 32.f/8.f)));
				}
				else {
					posTile = glm::vec2(minCoords.x + tileSize / 2 + i * tileSize, (minCoords.y + j * (tileSize - tileSize / 8)) + (incrFloor*(32.f - 32.f / 8.f)));
				}
				texCoordTile[0] = glm::vec2(0,(tile-1)*tileTexSize.y);
				texCoordTile[1] = glm::vec2(1, (tile)*tileTexSize.y);
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

bool TileMap::collision(glm::vec2 center, const int radius, glm::vec2 &speed) const
{
	int xp0, xp1, xi0, xi1, y, y0, y1;

	
	y1 = ((center[1] + radius) / (tileSize - tileSize / 8))-incrFloor;
	y0 = ((center[1] - radius) / (tileSize - tileSize / 8))-incrFloor;
	xp1 = ((center[0] + radius) / tileSize);
	xp0 = ((center[0] - radius) / tileSize);
	xi1 = (((center[0] + radius) - (tileSize / 2)) / tileSize);
	xi0 = (((center[0] - radius) - (tileSize / 2)) / tileSize);
	if (y0 < 0) y0 = 0;
	if (y1 < 0) y1 = 0;
	if (xp0 < 0) xp0 = 0;
	if (xp1 < 0) xp1 = 0;
	if (xi0 < 0) xi0 = 0;
	if (xi1 < 0) xi1 = 0;
	if (y0  > 12) y0  = 12;
	if (y1  > 12) y1  = 12;
	if (xp0 > 7) xp0 = 7;
	if (xp1 > 7) xp1 = 7;
	if (xi0 > 6) xi0 = 6;
	if (xi1 > 6) xi1 = 6;

	for (int y = y0; y <= y1; y++){

			
		if (y % 2 == 0){
			for (int x = xp0; x <= xp1; x++){
				int position = y*(mapSize.x) + x;
				if (map[position] != 0){

					float xcentr = tileSize / 2 + tileSize * x + tileSize / 2 * (y % 2);
					float ycentr = tileSize / 2 + (y * tileSize) - (y * tileSize / 8);
					glm::vec2 centro2 = glm::vec2(xcentr, ycentr + (incrFloor*28.f));
					float dist_centro = glm::distance(center, centro2);
					if (dist_centro <= 28)
						return true;
				}
			}
		}
		else{
			for (int x = xi0; x <= xi1; x++){
				int position = y*(mapSize.x) + x;
				if (map[position] != 0){
						
					float xcentr = tileSize / 2 + tileSize * x + tileSize/2 * (y % 2);
					float ycentr = tileSize/2 + (y * tileSize) - (y * tileSize / 8);
					glm::vec2 centro2 = glm::vec2(xcentr, ycentr + (incrFloor*(28.f)));
					float dist_centro = glm::distance(center, centro2);

					if (dist_centro <= 28)
						return true;
				}
			}
		}
	}


	return false;
}

stack<glm::ivec2> TileMap::collisionArrow(glm::vec2 center)
{
	int xp0, xp1, xi0, xi1, y, y0, y1;

	stack<glm::ivec2> s;
	y1 = ((center[1] - 19) / (tileSize - tileSize / 8)) - incrFloor;
	y0 = ((center[1] - 15) / (tileSize - tileSize / 8)) - incrFloor;
	xp1 = ((center[0] + 6) / tileSize);
	xp0 = ((center[0] - 6) / tileSize);
	xi1 = (((center[0] + 6) - (tileSize / 2)) / tileSize);
	xi0 = (((center[0] - 6) - (tileSize / 2)) / tileSize);
	if (y0 < 0) y0 = 0;
	if (y1 < 0) y1 = 0;
	if (xp0 < 0) xp0 = 0;
	if (xp1 < 0) xp1 = 0;
	if (xi0 < 0) xi0 = 0;
	if (xi1 < 0) xi1 = 0;
	if (y0  > 12) y0 = 12;
	if (y1  > 12) y1 = 12;
	if (xp0 > 7) xp0 = 7;
	if (xp1 > 7) xp1 = 7;
	if (xi0 > 6) xi0 = 6;
	if (xi1 > 6) xi1 = 6;

	for (int y = y0; y <= y1; y++){


		if (y % 2 == 0){
			for (int x = xp0; x <= xp1; x++){
				int position = y*(mapSize.x) + x;
				if (map[position] != 0){

					float xcentr = tileSize / 2 + tileSize * x + tileSize / 2 * (y % 2);
					float ycentr = tileSize / 2 + (y * tileSize) - (y * tileSize / 8);
					glm::vec2 centro2 = glm::vec2(xcentr, ycentr + (incrFloor*28.f));
					float dist_centro = glm::distance(center, centro2);
					if (dist_centro <= 18){
						s.push(glm::ivec2(x, y));
					}
				}
			}
		}
		else{
			for (int x = xi0; x <= xi1; x++){
				int position = y*(mapSize.x) + x;
				if (map[position] != 0){

					float xcentr = tileSize / 2 + tileSize * x + tileSize / 2 * (y % 2);
					float ycentr = tileSize / 2 + (y * tileSize) - (y * tileSize / 8);
					glm::vec2 centro2 = glm::vec2(xcentr, ycentr + (incrFloor*(28.f)));
					float dist_centro = glm::distance(center, centro2);
					if (dist_centro <= 18){
						s.push(glm::ivec2(x, y));
					}
				}
			}
		}
	}


	return s;
}


bool TileMap::addBall(glm::ivec2 tile, int color, ShaderProgram &program){
	int position = tile.y*(mapSize.x) + tile.x;
	map[position] = color;
	balls[0] += 1;
	balls[color] += 1;
	if (maxLine >= 11 - incrFloor){
		gameOver = true;
	}
	bool ret = false;
	if (!gameOver){
		ret = checkDelete(tile, color);
	}
	if (balls[0] > 0){
		prepareArrays(mMinCoords, program);
	}
	else stageClear = true;
	return ret;
}


bool TileMap::addBomb(glm::ivec2 tile, ShaderProgram &program){
	int position = tile.y*(mapSize.x) + tile.x;
	map[position] = 0;
	bool ret = false;
	if (!gameOver){
		ret = checkBomb(tile);
	}
	if (balls[0] > 0){
		prepareArrays(mMinCoords, program);
	}
	else stageClear = true;
	return ret;
}


bool TileMap::checkBomb(glm::vec2 tile){
	bool ret = false;
	int tileX = tile.x;
	int tileY = tile.y;
		mapCheck[tileY*mapSize.x + tileX] = true;
		if (tileY % 2 == 0){
			if ((tileY - 1 >= 0) && (tileX - 1 >= 0)){
				int pos = (tileY - 1)*mapSize.x + (tileX - 1);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX - 1, tileY - 1));
				}
			}
			if ((tileY - 1 >= 0) && (tileX <= 6)){
				int pos = (tileY - 1)*mapSize.x + (tileX);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX, tileY - 1));
				}
			}
			if (tileX - 1 >= 0){
				int pos = tileY*mapSize.x + (tileX - 1);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX - 1, tileY));
				}
			}
			if ((tileX <= 6)){
				int pos = tileY*mapSize.x + (tileX + 1);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX + 1, tileY));
				}
			}
			if ((tileY + 1 <= maxLine) && (tileX - 1 >= 0)){
				int pos = (tileY + 1)*mapSize.x + (tileX - 1);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX - 1, tileY + 1));
				}
			}
			if ((tileY + 1 <= maxLine) && (tileX <= 6)){
				int pos = (tileY + 1)*mapSize.x + (tileX);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX, tileY + 1));
				}
			}
		}
		else{
			if ((tileY - 1 >= 0)){
				int pos = (tileY - 1)*mapSize.x + (tileX);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX, tileY - 1));
				}
			}
			if ((tileY - 1 >= 0) && (tileX + 1 <= 7)){
				int pos = (tileY - 1)*mapSize.x + (tileX + 1);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX + 1, tileY - 1));
				}
			}
			if (tileX - 1 >= 0){
				int pos = tileY*mapSize.x + (tileX - 1);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX - 1, tileY));
				}
			}
			if ((tileX + 1 <= 6)){
				int pos = tileY*mapSize.x + (tileX + 1);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX + 1, tileY));
				}
			}
			if ((tileY + 1 <= maxLine)){
				int pos = (tileY + 1)*mapSize.x + (tileX);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX, tileY + 1));
				}
			}
			if ((tileY + 1 <= maxLine) && (tileX + 1 <= 7)){
				int pos = (tileY + 1)*mapSize.x + (tileX + 1);
				if (map[pos] != 0){
					ballsExpl.push(glm::vec2(tileX + 1, tileY + 1));
				}
			}
		}
	if (!ballsExpl.empty()){
		ret = true;
		cleanBalls(ballsExpl);
		cleanMap();
		cleanBalls(ballsFall);
	}
	else {
		while (!ballsExpl.empty()) ballsExpl.pop();
	}

	return ret;
}

void TileMap::processGameOver(ShaderProgram &program){
	if (gameOver){
		prepareArrays(mMinCoords, program);
	}

}

int TileMap::getBalls(int type){
	int ball = balls[type];
	return ball;
}

int TileMap::getColor(const glm::ivec2 &tile){
	int ball = map[(tile.y)*(mapSize.x) + tile.x];
	return ball;
}

void TileMap::setGameOver(bool Over){
	gameOver = Over;
}

void TileMap::setStageClear(bool clear){
	stageClear = clear;
}

bool TileMap::checkDelete(glm::vec2 tile, int color){
	bool ret = false;
	queue<glm::vec2> s;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < (mapSize.x); i++)
		{
			mapCheck[j*mapSize.x + i] = false;
		}
	}
	s.push(tile);
	do{
		glm::vec2 tileCheck = s.front();
		s.pop();
		int tileX = tileCheck.x;
		int tileY = tileCheck.y;
		if (!mapCheck[tileY*mapSize.x + tileX] && map[tileY*mapSize.x + tileX]!= 0){
			ballsExpl.push(glm::vec2(tileX, tileY));
			mapCheck[tileY*mapSize.x + tileX] = true;
			if (tileY % 2 == 0){
				if ((tileY - 1 >= 0) && (tileX - 1 >= 0)){
					int pos = (tileY - 1)*mapSize.x + (tileX - 1);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX - 1, tileY - 1));
					}
					//Y-1 X-1
				}
				if ((tileY - 1 >= 0) && (tileX <= 6)){
					int pos = (tileY - 1)*mapSize.x + (tileX);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX, tileY - 1));
					}
					//Y-1 X
				}
				if (tileX - 1 >= 0){
					int pos = tileY*mapSize.x + (tileX - 1);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX - 1, tileY));
					}
					//Y X-1
				}
				if ((tileX <= 6)){
					int pos = tileY*mapSize.x + (tileX + 1);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX + 1, tileY));
					}
					//Y X+1
				}
				if ((tileY + 1 <= maxLine) && (tileX - 1 >= 0)){
					int pos = (tileY + 1)*mapSize.x + (tileX - 1);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX - 1, tileY + 1));
					}
					//Y+1 X-1
				}
				if ((tileY + 1 <= maxLine) && (tileX <= 6)){
					int pos = (tileY + 1)*mapSize.x + (tileX);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX, tileY + 1));
					}
					//Y+1 X
				}
			}
			else{
				if ((tileY - 1 >= 0)){
					int pos = (tileY - 1)*mapSize.x + (tileX);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX, tileY - 1));
					}
					//Y-1 X
				}
				if ((tileY - 1 >= 0) && (tileX + 1 <= 7)){
					int pos = (tileY - 1)*mapSize.x + (tileX + 1);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX + 1, tileY - 1));
					}
					//Y-1 X+1
				}
				if (tileX - 1 >= 0){
					int pos = tileY*mapSize.x + (tileX - 1);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX - 1, tileY));
					}
					//Y X-1
				}
				if ((tileX + 1 <= 6)){
					int pos = tileY*mapSize.x + (tileX + 1);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX + 1, tileY));
					}
					//Y X+1
				}
				if ((tileY + 1 <= maxLine)){
					int pos = (tileY + 1)*mapSize.x + (tileX);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX, tileY + 1));
					}
					//Y+1 X
				}
				if ((tileY + 1 <= maxLine) && (tileX + 1 <= 7)){
					int pos = (tileY + 1)*mapSize.x + (tileX + 1);
					if (map[pos] == color && !mapCheck[pos]){
						s.push(glm::vec2(tileX + 1, tileY + 1));
					}
					//Y+1 X+1
				}
			}
		}
	} while (!s.empty());
	if (!ballsExpl.empty() && ballsExpl.size() >= 3){
		ret = true;cleanBalls(ballsExpl);
		cleanMap();
		cleanBalls(ballsFall);		
	}
	else {
		while (!ballsExpl.empty()) ballsExpl.pop();
	}

	return ret;
}

void TileMap::cleanBalls(stack<glm::ivec2> s){
	
	while (!s.empty()){
		glm::vec2 tileCheck = s.top();
		s.pop();
		int tileX = tileCheck.x;
		int tileY = tileCheck.y;
		int pos = tileY*mapSize.x + (tileX);
		if (map[pos] != 0){
			balls[map[pos]] = balls[map[pos]] - 1;
			balls[0] = balls[0] - 1;
			map[pos] = 0;
		}
	}
}

void TileMap::clearArrow(vector<pair<glm::ivec2, int>> pierced, int incrFloor, ShaderProgram &program){
	for (int i = 0; i < pierced.size(); ++i){
		ballsExpl.push(pierced[i].first);
		colorExplote.push(pierced[i].second);
	}
	cleanBalls(ballsExpl);
	cleanMap();
	cleanBalls(ballsFall); 
	if (balls[0] > 0){
		prepareArrays(mMinCoords, program);
	}
	else stageClear = true;
}

void TileMap::cleanMap(){
	queue<glm::vec2> s;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < (mapSize.x); i++)
		{
			mapFall[j*mapSize.x + i] = false;
		}
	}
	for (int k = 0; k < 8; ++k){
		if (map[k] != 0){
			s.push(glm::vec2(k, 0));
			do{
				glm::vec2 tileCheck = s.front();
				s.pop();
				int tileX = tileCheck.x;
				int tileY = tileCheck.y;
				mapFall[tileY*mapSize.x + tileX] = true;
				if (tileY % 2 == 0){
					if ((tileY - 1 >= 0) && (tileX - 1 >= 0)){
						int pos = (tileY - 1)*mapSize.x + (tileX - 1);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX - 1, tileY - 1));
						}
					}
					if ((tileY - 1 >= 0) && (tileX <= 6)){
						int pos = (tileY - 1)*mapSize.x + (tileX);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX, tileY - 1));
						}
					}
					if (tileX - 1 >= 0){
						int pos = tileY*mapSize.x + (tileX - 1);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX - 1, tileY));
						}
						//Y X-1
					}
					if ((tileX <= 6)){
						int pos = tileY*mapSize.x + (tileX + 1);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX + 1, tileY));
						}
						//Y X+1
					}
					if ((tileY + 1 <= maxLine) && (tileX - 1 >= 0)){
						int pos = (tileY + 1)*mapSize.x + (tileX - 1);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX - 1, tileY + 1));
						}
						//Y+1 X-1
					}
					if ((tileY + 1 <= maxLine) && (tileX <= 6)){
						int pos = (tileY + 1)*mapSize.x + (tileX);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX, tileY + 1));
						}
						//Y+1 X
					}
				}
				else{
					if ((tileY - 1 >= 0)){
						int pos = (tileY - 1)*mapSize.x + (tileX);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX, tileY - 1));
						}
						//Y-1 X
					}
					if ((tileY - 1 >= 0) && (tileX + 1 <= 7)){
						int pos = (tileY - 1)*mapSize.x + (tileX + 1);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX + 1, tileY - 1));
						}
						//Y-1 X+1
					}
					if (tileX - 1 >= 0){
						int pos = tileY*mapSize.x + (tileX - 1);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX - 1, tileY));
						}
						//Y X-1
					}
					if ((tileX + 1 <= 6)){
						int pos = tileY*mapSize.x + (tileX + 1);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX + 1, tileY));
						}
						//Y X+1
					}
					if ((tileY + 1 <= maxLine)){
						int pos = (tileY + 1)*mapSize.x + (tileX);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX, tileY + 1));
						}
						//Y+1 X
					}
					if ((tileY + 1 <= maxLine) && (tileX + 1 <= 7)){
						int pos = (tileY + 1)*mapSize.x + (tileX + 1);
						if (map[pos] != 0 && !mapFall[pos]){
							s.push(glm::vec2(tileX + 1, tileY + 1));
						}
						//Y+1 X+1
					}
				}
			} while (!s.empty());
		}
	}

	

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < (mapSize.x); i++)
		{
			int pos = j*mapSize.x + i;
			if (!mapFall[pos] && map[pos] != 0){
				ballsFall.push(glm::vec2(i, j));
				colorFall.push(map[pos]);
			}
		}
	}
	
}

stack<glm::ivec2> TileMap::getExplode(){
	stack<glm::ivec2> aux;
	while (!ballsExpl.empty()){
		glm::ivec2 aaa = ballsExpl.top();
		ballsExpl.pop();
		aux.push(aaa);
	}
	
	return aux;
}

void TileMap::setIncrFloor(int timeUp){
	incrFloor = timeUp;
}

stack<glm::ivec2> TileMap::getFall(){ 
	stack<glm::ivec2> aux;
	while (!ballsFall.empty()){
		glm::ivec2 aaa = ballsFall.top();
		ballsFall.pop();
		aux.push(aaa);
	}
	return aux;
}

stack<int> TileMap::getColorFall(){
	stack<int> aux;
	while (!colorFall.empty()){
		int aaa = colorFall.top();
		colorFall.pop();
		aux.push(aaa);
	}
	return aux;
}

stack<int> TileMap::getColorExplodeArrow(){
	stack<int> aux;
	while (!colorExplote.empty()){
		int aaa = colorExplote.top();
		colorExplote.pop();
		aux.push(aaa);
	}
	return aux;
}

























