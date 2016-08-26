#include <stdio.h>
#include <string.h>





typedef enum(
	GPTile_Walkable = 0, GPTile_Block, GPTile_Wall
)GPTile_Type;

typedef struct _tile{
	int id;
	char *name;
	char *imageDir;
	int type;
}tile;

typedef struct _map{
	char *name;
	char *coreFilePath;
	char *conditionFilePath;
	char *contentPathSet;
	int height;
	int width;
	tile **grid;
}GPMap;




