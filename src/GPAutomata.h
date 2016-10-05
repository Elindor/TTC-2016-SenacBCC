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

GPMap* generateAutomataMap(GPMap *map);

#endif