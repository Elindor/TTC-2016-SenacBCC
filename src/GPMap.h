#ifndef GPMapBlock
#define GPMapBlock


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "json.h" // https://github.com/udp/json-parser







typedef enum{
	GPTile_Walkable = 0, GPTile_Block, GPTile_Wall
}GPTile_Type;

typedef struct _tile{
	int id;
	char *name;
	char *imageDir;
	int type;
    int token;
}GMTile;

typedef struct _Sroom{
	int id;
	int pointX;
	int pointY;
	struct _Sroom *next;
}GMSRoom;

typedef struct _map{
	char *name;
	char *coreFilePath;
	char *conditionFilePath;
	char *contentPathSet;
	int height;
	int width;
    int maximumContent;
    int minimalContent;
    int generation;
	GMTile ***grid;
}GPMap;

#include "GPAutomata.h"
#include "GPMap_Genetic.h"


char* getCopyFromString(char* str);
GPMap* generateMap(char*mapCorePath);
int releaseMap(GPMap  *map);
void outputCurrentMapToFile(GPMap  *map, char* filename);
void outputCurrentMapToStream(GPMap  *map);
GMTile *getTile (GPMap *map, int xPosition, int yPosition);
int PMrand ();

#endif
