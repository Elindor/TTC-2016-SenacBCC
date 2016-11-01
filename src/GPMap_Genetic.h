
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
	char genes[10];
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
	char name[100];
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

typedef struct _RoomList{
    GMRoom *room;
    int chance;
    struct _RoomList *next;
}GPRoomList;




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
void printListTemp();
int length();

GPMap* generateGeneticMap(GPMap *mapa);
GMRoom* seach(int key,GGnode*head);
GPRoomList* GerateListPosibylit(char *gene, GGnode *alpha);

GPRoomList* SeachCopatibility(GMRoom *atual, GPRoomList *head);
GPRoomList* deleteid(int key, GPRoomList *head);
GPRoomList* SeachIncopatibility(char *gene, GPRoomList *head);
GPRoomList* SeachIncopatibilityid(int salaid, GPRoomList *head);
GPRoomList* SeachIncopatibilityExit(GMRoom *atual, GPRoomList *head);

GPRoomList* DeleteEG(char *key, GPRoomList *head);
GPRoomList* DeleteAllG(char *key, GPRoomList *head);
GPRoomList* DeleteOffG(char *key, GPRoomList *head);
GPRoomList*  DeleteVoidDoors(GPGenetic_Map *mapa, int x, int y,GmPonto ponto, GPRoomList *head);

void fixdoor(GPGenetic_Map *mapa, int x, int y, GmPonto ponto);
GmPonto discartmap(GPGenetic_Map *mapa, GmPonto ponto);
void printDoorList(int x, int y);

void FixRoom(GPGenetic_Map *map, GmPonto ponto, int fonte);

void print_genetic_grid(GPGenetic_Map *map);


GmPonto seachNextRoom(GPGenetic_Map *mapa,GmPonto ponto);
int Sort_Rooom(GPRoomList *head);
int ocuppedspace(GPGenetic_Map *mapa);
GmPonto varremapapor(GPGenetic_Map *mapa, GmPonto ponto);
GmPonto selectdoor(GMRoom *atual,int x, int y);

#endif

