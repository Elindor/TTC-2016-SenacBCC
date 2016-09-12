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

typedef struct _map{
	char *name;
	char *coreFilePath;
	char *conditionFilePath;
	char *contentPathSet;
	int height;
	int width;
	GMTile **grid;
}GPMap;

GPMap* generateMap(char*mapCorePath);
int releaseMap(GPMap *map);
void outputCurrentMapToFile(GPMap *map, char* filename);
GMTile getTile (GPMap* map, int xPosition, int yPosition);

