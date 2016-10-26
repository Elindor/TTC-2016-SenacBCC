
#ifndef GPGenetic
#define GPGenetic

#include "GPMap.h"


typedef struct _Crossover{

	int id; // não necessita dos dois
	char *genes;
	int chance; // na incompatibilidade é ignorado
    struct _Crossover *next;

}GmCrossover;

typedef struct _ponto{
	char *genes;
	int y;
    int x;
}GmPonto;



typedef struct _Door{

	int idRoomX;
	int idRoomY;
	int idDoor;

	char *gene;

	int idNextRoom;
	int idNextRoomX;
	int idNextRoomY;

	int idNextdoor;


	struct _Door *next;
}GMDoor;

typedef struct _room{
	int id;
	char *name;
	int height;
	int width;
	int chance;
	GMTile **mapIntern;
	char *gene;	// N L S O

	//definicao para mapas
	int Tam;
	GMDoor *doors;

	//compatibilidade
	GmCrossover *incompative;
	GmCrossover *compativeExit;
	GmCrossover *incompativeEntrad;
	//
	int alldoorsocupped;
	struct _room *next;
}GMRoom;






typedef struct _RoomContent{
    GMRoom *first;
}GMRoomContent;





typedef struct _Node
{
    GMRoom *sala;
}GGnode;

typedef struct _mapa
{
    char *name;
	char *coreFilePath;
	char *conditionFilePath;
	char *contentPathSet;
	int height;
	int width;
    int maximumContent;
    int minimalContent;
    int generation;
    GMRoom ***grid;
}GPGenetic_Map;




void printList();
int length();

GPMap* generateGeneticMap(GPMap *mapa);
GMRoom* seach(int key);
GGnode* GerateListPosibylit(char *gene, GGnode *alpha);

GGnode* SeachCopatibility(GMRoom *atual, GGnode *head);
GGnode* deleteid(int key, GGnode *head);
GGnode* SeachIncopatibility(char *gene, GGnode *head);
GGnode* SeachIncopatibilityid(int salaid, GGnode *head);
GGnode* SeachIncopatibilityExit(GMRoom *atual, GGnode *head);

GGnode* DeleteEG(char *key, GGnode *head);
GGnode* DeleteAllG(char *key, GGnode *head);
GGnode* DeleteOffG(char *key, GGnode *head);
GGnode*  DeleteVoidDoors(GPGenetic_Map *mapa, int x, int y,GmPonto ponto, GGnode *head);

void fixdoor(GPGenetic_Map *mapa, int x, int y, GmPonto ponto);
void discartmap(GPGenetic_Map *mapa);

GmPonto seachNextRoom(GPGenetic_Map *mapa,GmPonto ponto);
int Sort_Rooom(GGnode *head);
int ocuppedspace(GPGenetic_Map *mapa);
GmPonto varremapapor(GPGenetic_Map *mapa, GmPonto ponto);
GmPonto selectdoor(GMRoom *atual,int x, int y);

GMRoom *getRoom (GPGenetic_Map *map, int xPosition, int yPosition);
#endif

