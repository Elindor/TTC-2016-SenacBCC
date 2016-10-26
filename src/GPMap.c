#include "GPMap.h"
#include "json.h" // https://github.com/udp/json-parser

#define rand_a 48271
#define rand_m 2147483647
#define rand_q (rand_m / rand_a)
#define rand_r (rand_m % rand_a)

static long int randSeed = 1;
static GPMap *map;

static void print_depth_shift(int depth)
{
    int j;
    for (j=0; j < depth; j++) {
        printf(" ");
    }
}
static void process_value(json_value* value, int depth, char* name);

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
        process_value(value->u.object.values[x].value, depth+1, value->u.object.values[x].name);
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
        process_value(value->u.array.values[x], depth, NULL);
    }
}

static void process_value(json_value* value, int depth, char* name)
{
//    int j;
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
            if(name == NULL)
                return;
            if(!strcmp (name,"Width")){
                map->width = 0 + (int)value->u.integer;
                printf("Acquired width: %d\n", map->width);
            }
            else if(!strcmp (name,"Height")){
                printf("Acquired height: %lld\n", value->u.integer);
                map->height = (int)value->u.integer;
            }
            else if(!strcmp (name,"GenerationType")){
                printf("Acquired GenerationType: %lld\n", value->u.integer);
                map->generation = (int)value->u.integer;
            }
            else if(!strcmp (name,"MinimalContent")){
                printf("Acquired minimal: %lld\n", value->u.integer);
                map->minimalContent = (int)value->u.integer;
            }
            else if(!strcmp (name,"MaximumContent")){
                printf("Acquired maximum: %lld\n", value->u.integer);
                map->maximumContent = (int)value->u.integer;
            }
            else
            printf("int: %10" PRId64  "\n", value->u.integer);

            break;
        case json_double:
            printf("double: %f\n", value->u.dbl);
            break;
        case json_string:
            if(name == NULL)
                return;
            if(!strcmp (name,"ConditionSet")){
                printf("Acquired ConditionSet: %s\n", value->u.string.ptr);
                map->conditionFilePath = getCopyFromString(value->u.string.ptr);
            }
            else if(!strcmp (name,"ContentSet")){
                printf("Acquired ContentSet: %s\n", value->u.string.ptr);
                map->contentPathSet = getCopyFromString(value->u.string.ptr);
            }
            else if (!strcmp(name,"mapName")){
                printf("Acquired Map Name: %s\n", value->u.string.ptr);
                map->name = getCopyFromString(value->u.string.ptr);
            }
            else printf("string: %s\n", value->u.string.ptr);
            break;
        case json_boolean:
            printf("bool: %d\n", value->u.boolean);
            break;
        default:
            break;
    }
}



enum GPGeneration_Type {GPGenerationType_Automata = 0, GPGenerationType_Genetic, GPGenerationType_Hybrid_Genetic, GPGenerationType_Hybrid_Automata};

GPMap* generateMap(char*mapCorePath){

	map = malloc(sizeof(GPMap));

	// Busca Informação Básica

    FILE *coreFile;
    struct stat filestatus;
    int file_size;
    char* file_contents;

    if ( stat(mapCorePath, &filestatus) != 0) {
        fprintf(stderr, "File %s not found\n", mapCorePath);
        return NULL;
    }
    file_size = (int)filestatus.st_size;
    file_contents = (char*)malloc(filestatus.st_size);
    if ( file_contents == NULL) {
        fprintf(stderr, "Memory error: unable to allocate %d bytes\n", file_size);
        return NULL;
    }


    coreFile = fopen(mapCorePath, "rb");
    if (coreFile == NULL) {
        fprintf(stderr, "Unable to open %s\n", mapCorePath);
        fclose(coreFile);
        free(file_contents);
        return NULL;
    }
    if ( fread(file_contents, file_size, 1, coreFile) != 1 ) {
                if (feof(coreFile)) {
                  puts ("End-of-File reached.");
                  printf ("Total number of bytes read: %d\n",file_size);
                }
        fprintf(stderr, "Unable to read content of %s\n", mapCorePath);
        fclose(coreFile);
        free(file_contents);
        return NULL;
    }
    fclose(coreFile);


    printf("%s\n", file_contents);


    json_char* json;
    json_value* value;
    json = (json_char*)file_contents;
    value = json_parse(json,file_size);

    if (value == NULL) {
        fprintf(stderr, "Unable to parse data in %s; JSON structure is probably not valid. Try dubbuging at http://jsonlint.com \n", mapCorePath);
        free(file_contents);
        exit(1);
    }

    process_value(value, 0, NULL);

    json_value_free(value);
    free(file_contents);



	// Gera Mapa
	map->grid = (GMTile***)malloc(sizeof(GMTile**) * map->width);
    int i, j;
    printf("%d\n", map->width);
    for(i = 0; i < map->height; i ++){
        map->grid[i] = (GMTile**)malloc(sizeof(GMTile*) * map->height);
        for(j = 0; j < map->height; j ++){
            GMTile *tiler = malloc(sizeof(GMTile));
            tiler->id = 0;
            tiler->type = 1;
            tiler->name = "name";
            map->grid[i][j] = tiler;

        }

    }

    outputCurrentMapToStream(map);

	// Processa Mapa
	switch(map->generation){
        case GPGenerationType_Genetic:{

            generateGeneticMap(map);

            break;

        }
		case GPGenerationType_Automata:
            printf("%d\n", map->width);

            generateAutomataMap(map);
            break;
		case GPGenerationType_Hybrid_Genetic:

		break;
		case GPGenerationType_Hybrid_Automata:

		break;

	}

    return map;
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
    		GMTile *t = getTile(map, i, j);
	        fprintf(file,"%2d ",t->id); /* write */
    	}
    	fprintf(file, "\n ");
    }

    fprintf(file, "\n-----------------------------\n     Printing Finished     \n-----------------------------\n");
    fclose(file);                       /* close the file pointer */

    return;

}

void outputCurrentMapToStream(GPMap *map){
    int i, j;

    printf("\n-----------------------------\n     Printing Started     \n-----------------------------\n");
    printf("Map Name: %s\nSized with width %d and height %d\n\n Printing grid \n\n", map->name, map->width, map->height);

    for (i = 0; i < map->width; i++){
        for(j = 0; j < map->height; j++){
            GMTile *t = getTile(map, i, j);
            printf("%2d ",t->id); /* write */
        }
        printf("\n ");
    }

    printf("\n-----------------------------\n     Printing Finished     \n-----------------------------\n");

    return;

}

GMTile *getTile (GPMap *map, int xPosition, int yPosition){
    GMTile *tile = map->grid[xPosition][yPosition];
    return tile;
}

char* getCopyFromString(char* str){
    char* ptr = str;

    int orig_str_size = 0;
    char* bkup_copy = NULL;

    while (*ptr++ != '\0')
        orig_str_size++;

    bkup_copy = (char*) malloc((orig_str_size+1) * sizeof(char));
    bkup_copy[orig_str_size] = '\0';

    ptr = &str[0];
    int idx = 0;
    while (*ptr != '\0')
        bkup_copy[idx++] = *ptr++;


    return bkup_copy;
}

int PMrand(){
    long int hi = randSeed / rand_q;
    long int lo = (randSeed * rand()) % rand_q;
    long int test = rand_a * lo - rand_r * hi;
    if(test > 0)
        randSeed = test;
    else	randSeed = test + rand_m;
    return (int)randSeed;
}
