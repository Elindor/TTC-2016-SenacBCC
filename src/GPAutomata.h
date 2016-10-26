#ifndef GPAutomata
#define GPAutomata

#include "GPMap.h"




typedef struct _Scatter{
    int id;
    int chance;
    struct _Scatter *next;
}GMScatterChance;

typedef struct _tiletype{
    int id;
    char *name;
    char *imageDir;
    int type;
    int totalChance;
    GMScatterChance *scatter;
    struct _tiletype *next;
}GMTileType;

typedef struct _automataContent{
    GMTileType *first;
}GMAutomataContent;

typedef struct _TileList{
    GMTile *tile;
    int x;
    int y;
    struct _TileList *next;
}GPTileList;


GPMap* generateAutomataMap(GPMap *map);
void addTileList(GPTileList *l, GMTile* t, int x, int y);
void spread(GMTile *tile, GMTile *nextTile);


#endif
