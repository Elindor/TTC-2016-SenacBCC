#include "GPMap.h"


static void print_depth_shift(int depth)
{
    int j;
    for (j=0; j < depth; j++) {
        printf(" ");
    }
}

static void process_value(json_value* value, int depth);

static void process_object(json_value* value, int depth)
{
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++) {
        print_depth_shift(depth);
        printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
        process_value(value->u.object.values[x].value, depth+1);
    }
}

static void process_array(json_value* value, int depth)
{
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.array.length;
    printf("array\n");
    for (x = 0; x < length; x++) {
        process_value(value->u.array.values[x], depth);
    }
}

static void process_value(json_value* value, int depth)
{
    int j;
    if (value == NULL) {
        printf("NULL value\n");
        return;
    }
    if (value->type != json_object) {
        print_depth_shift(depth);
    }
    switch (value->type) {
        case json_none:
            printf("none\n");
            break;
        case json_object:
            process_object(value, depth+1);
            break;
        case json_array:
            process_array(value, depth+1);
            break;
        case json_integer:
            printf("int: %10" PRId64 "\n", value->u.integer);
            break;
        case json_double:
            printf("double: %f\n", value->u.dbl);
            break;
        case json_string:
            printf("string: %s\n", value->u.string.ptr);
            break;
        case json_boolean:
            printf("bool: %d\n", value->u.boolean);
            break;
    }
}


enum GPGeneration_Type {GPGenerationType_Automata = 0, GPGenerationType_Genetic, GPGenerationType_Hybrid_Genetic, GPGenerationType_Hybrid_Automata};

GPMap* generateMap(char*mapCorePath){

	GPMap *map = malloc(sizeof(map));

	// Busca Informação Básica

	map->width = 50;
	map->height = 50;

    FILE *coreFile;
    struct stat filestatus;
    int file_size;
    char* file_contents;
    
    if ( stat(mapCorePath, &filestatus) != 0) {
        fprintf(stderr, "File %s not found\n", mapCorePath);
        return NULL;
    }
    file_size = filestatus.st_size;
    file_contents = (char*)malloc(filestatus.st_size);
    if ( file_contents == NULL) {
        fprintf(stderr, "Memory error: unable to allocate %d bytes\n", file_size);
        return NULL;
    }
    
    
    coreFile = fopen(mapCorePath, "rt");
    if (coreFile == NULL) {
        fprintf(stderr, "Unable to open %s\n", mapCorePath);
        fclose(coreFile);
        free(file_contents);
        return NULL;
    }
    if ( fread(file_contents, file_size, 1, coreFile) != 1 ) {
        fprintf(stderr, "Unable to read content of %s\n", mapCorePath);
        fclose(coreFile);
        free(file_contents);
        return NULL;
    }
    fclose(NULL);
    
    
    printf("%s\n", file_contents);

    
    json_char* json;
    json_value* value;
    json = (json_char*)file_contents;
    value = json_parse(json,file_size);
    printf("here %ld\n", sizeof(value));

    
    
    process_value(value, 0);
    
    json_value_free(value);
    free(file_contents);



	// Gera Mapa
	map->grid = malloc(sizeof(GMTile*) * map->width);
    int i;
    for(i = 0; i < map->width; i ++)
        map->grid[map->width] = malloc(sizeof(GMTile) * map->height);



	// Processa Mapa
	switch(1){
        case GPGenerationType_Genetic:{
            int condition = 0; // 0 ou 1 se estar atendendo todas as condições
            
            
            //Gerar grupo de salas -- (mapa, arquivo de salas)
            //Fixar salas Especiais no mapa
            
            
            //Pegar condições gerais -- acho que gerer um struct com ela deve ajudar
//            GMRoom RoomNow;   // Falta malloc
//            char selectedGene;
//            while(!condition){
            
                
                //Inicio da geração -- loop enquanto n atingir a condiçao ou o limite
//                RoomNow = map[x][y];
//                selectedGene = selectdoor(RoomNow);
//                if(selectedGene != "0"){
//                    conjunctPossibility = GerateListPossibylit(geneSelecionade, AllRooms);
//                    conjunctPossibility = RemoveListImpossibylit(RoomNow, conjunctPossibility);
//                    
//                }
                // preferencia a garantir rota, localiza melhor caminho
                // com a rota preferencial seleciona a porta(filha) e isola os canditados
                //  ---  map[N/S][L/W] = genetic( filha, canditados) -- retorna sala com a porta conexão preencida
                //Isolar grupo de salas posiveis
                //Remover todas as incomaptibilidades
                // "sortear" sala
//            }
            // Teste de resultado
            break;

        }
		case GPGenerationType_Automata:

		break;
		case GPGenerationType_Hybrid_Genetic:

		break;
		case GPGenerationType_Hybrid_Automata:

		break;

	}
    
    return map;
}

char selectdoor(GMRoom *atual){
	int i;
	char chosen = '0';
	for( i = 0; i < atual->Quantdoors; i++ ){
        if(atual->doors[i].idNextRoom == -1){            // DENIS, tava null, não se usa null pra int em C. Use um valor negativo
			switch(atual->doors[i].gene)
			{
				case 'N':
				case 'n':
					chosen = 'S';
					break;
				case 'S':
				case 's':
					chosen = 'N';
					break;
				case 'L':
				case 'l':
					chosen = 'O';
					break;
				case 'O':
				case 'o':
					chosen = 'L';
					break;
			}
			return chosen;
        }
	}

	return '0';
}


int releaseMap(GPMap *map){

	int i, j;
	for(i = 0; i < map->width; i ++)
		for(j = 0; j < map->height; j ++){
			free(map->grid[map->width]);
		}
	free(map->grid);
	free(map);
	return 1;
}

void outputCurrentMapToFile(GPMap *map, char* filename){

	FILE *file;

    int i, j;
    char filePath[32];
    sprintf(filePath, "%s.txt", filename);
    file = fopen(filePath,"a+");      /* open the file in append mode */

    fprintf(file, "\n-----------------------------\n     Printing Started     \n-----------------------------\n");
    fprintf(file, "Map Name: %s\n Sized with width %d and height %d\n\n Printing grid \n\n", map->name, map->width, map->height);

    for (i = 0; i < map->width; i++){
    	for(j = 0; j < map->height; j++){
    		GMTile t = getTile(map, i, j);
	        fprintf(file,"%2d ",t.id); /* write */
    	}
    	fprintf(file, "\n ");
    }

    fprintf(file, "\n-----------------------------\n     Printing Finished     \n-----------------------------\n");
    fclose(file);                       /* close the file pointer */ 

    return;

}



GMTile getTile (GPMap* map, int xPosition, int yPosition){
    GMTile tile = map->grid[xPosition][yPosition];
    return tile;
}


//GMRoom getRoom(GPMap* map, int xPosition, int yPosition){ Essa é uma TILE, vc precisa de uma estrutura auxiliar de conteudo pra buscar uma sala. Faz um array de salas comum.
//    GMRoom room = map->grid[xPosition][yPosition];
//
//    return;
//}


