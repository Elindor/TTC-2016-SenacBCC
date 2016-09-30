#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "json.h" // https://github.com/udp/json-parser





typedef enum{
	GPTile_Walkable = 0, GPTile_Block, GPTile_Wall
}GPTile_Type;

typedef struct _tile{
	int id;
	char *name;
	char *imageDir;
	int type;
}GMTile;

typedef struct _door{
	int idRoomX;
	int idRoomY;
	int idDoor;
	char gene;
	int idNextRoom;
	int idNextdoor;
}GMDoor;

typedef struct _crossover{

	int id; // não necessita dos dois
	char *genes;
	int chance; // na incompatibilidade é ignorado


}crossover;

typedef struct _room{
	int id;
	char *name;
	int height;
	int width;
	int chance;
	GMTile **mapIntern;
	char *gene;	// N L S O


	int Quantdoors;
	GMDoor *doors;
	//compatibilidade
	crossover *incompative;
	crossover *compativeExit;
	crossover *incompativeEntrad;
}GMRoom;

typedef struct _map{
	char *name;
	char *coreFilePath;
	char *conditionFilePath;
	char *contentPathSet;
	int height;
	int width;
    int maximumContent;
    int minimalContent;
	GMTile **grid;
}GPMap;

GPMap* generateMap(char*mapCorePath);
int releaseMap(GPMap *map);
void outputCurrentMapToFile(GPMap *map, char* filename);
GMTile getTile (GPMap* map, int xPosition, int yPosition);


