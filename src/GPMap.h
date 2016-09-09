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

typedef struct _door{
	int idRoomX;
	int idRoomY;
	int idDoor;
	char gene;
	
	int idNextRoom = NULL;
	int idNextdoor = NULL;
}door;

typedef _crossover{
	
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
	tile **mapIntern;
	char *gene;	// N L S O
	

	int Quantdoors;
	door *doors;
	//compatibilidade
	crossover *incompative;
	crossover *compativeExit;
	crossover *incompativeEntrad;
}room;

typedef struct _map{
	char *name;
	char *coreFilePath;
	char *conditionFilePath;
	char *contentPathSet;
	int height;
	int width;
	tile **grid;
}GPMap;



