#include "GPMap.h"

enum GPGeneration_Type(GPGenerationType_Automata = 0, GPGenerationType_Genetic, GPGenerationType_Hybrid_Genetic, GPGenerationType_Hybrid_Automata);

GPMap* generateMap(char*mapCorePath){

	GPMap *map = malloc(sizeof(map));

	// Busca Informação Básica

	map.width = 50;
	map.height = 50;




	// Gera Mapa
	map.grid = malloc(sizeof(tile) * width * height);
	int i, j;
	for(i = 0; i < map.width; i ++)
		for(j = 0; j < map.height; j ++){
			map.grid[width][height] = malloc(sizeof(tile));
		}


	// Processa Mapa
	switch(generationType){
		case GPGenerationType_Genetic:

		break;
		case GPGenerationType_Automata:

		break;
		case GPGenerationType_Hybrid_Genetic:

		break;
		case GPGenerationType_Hybrid_Automata:

		break;

	}

}

int releaseMap(GPMap *map){

	int i, j;
	for(i = 0; i < map.width; i ++)
		for(j = 0; j < map.height; j ++){
			free(map.grid[width][height]);
		}
	free(map.grid);
	free(map);
	return 1;
}


void outputCurrentMapToFile(GPMap *map, char* filename){

	FILE *fout;

    int i, j;
    char[32] filePath;
    sprintf(filePath, "%s.txt"); 
    fout = fopen(filePath,"a+");      /* open the file in append mode */

    fprintf(file, "\n-----------------------------\n     Printing Started     \n-----------------------------\n");
    fprintf(file, "Map Name: %s\n Sized with width %d and height %d\n\n Printing grid \n\n", map.name, map.width, map.height);

    for (i = 0; i < map.width; i++){
    	for(j = 0; j < map.height; j++){
    		tile *t = getTile(map, i, j);
	        fprintf(file,"%2d ",tile.id); /* write */ 
    	}
    	fprintf(file, "\n ");
    }

    fprintf(file, "\n-----------------------------\n     Printing Finished     \n-----------------------------\n");
    fclose(file);                       /* close the file pointer */ 

    return 0; 

}



tile* getTile(GPMap*map, int xPosition, int yPosition){

	return map.grid[xPosition][yPosition];
}