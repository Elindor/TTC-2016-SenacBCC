#include "GPAutomata.h"
GMAutomataContent *content;
int generatingTile;
int generatingScatter;

static void print_depth_shift(int depth)
{
    int j;
    for (j=0; j < depth; j++) {
        printf(" ");
    }
}

static void process_automata_content_value(json_value* value, int depth, char* name);
void addTileList(GPTileList *l, GMTile* t, int x, int y);
void spread(GMTile *tile, GMTile *nextTile);

static void process_automata_content_object(json_value* value, int depth)
{
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++) {
        print_depth_shift(depth);
        printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
        process_automata_content_value(value->u.object.values[x].value, depth+1, value->u.object.values[x].name);
    }
}

static void process_automata_content_array(json_value* value, int depth)
{
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.array.length;
    printf("array\n");
    for (x = 0; x < length; x++) {
        // Reads new array: if is tile processing, creates new tile and pushes, otherwise, creates new scatter.
        if(generatingTile){
            GMTileType *a = content->first;
            GMTileType *b = malloc(sizeof(GMTileType));
            b->next = a;
            content->first = b;
        }
        else if(generatingScatter){
            GMTileType *tile = content->first;
            GMScatterChance *a = tile->scatter;
            GMScatterChance *b = malloc(sizeof(GMScatterChance));
            b->next = a;
            tile->scatter = b;
        }

        process_automata_content_value(value->u.array.values[x], depth, NULL);
    }
}

static void process_automata_content_value(json_value* value, int depth, char* name)
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
            process_automata_content_object(value, depth+1);
            break;
        case json_array:
            if(name == NULL)
                return;
            if(!strcmp (name,"Tiles")){
                generatingTile = 1;
                process_automata_content_array(value, depth+1);
                generatingTile = 0;

            }
            if(!strcmp (name,"ScatterRules")){
                generatingScatter = 1;
                generatingTile = 0;
                process_automata_content_array(value, depth+1);
                generatingScatter = 0;
                generatingTile = 1;
            }
            break;
        case json_integer:
            if(name == NULL)
                return;
            if(!strcmp (name,"Id")){
                if(generatingTile){
                    GMTileType *t = content->first;
                    printf("Acquired tile Id: %lld\n", value->u.integer);
                    t->id = (int)value->u.integer;
                }
                if(generatingScatter){
                    GMScatterChance *t = content->first->scatter;
                    printf("Acquired scatter Id: %lld\n", value->u.integer);
                    t->id = (int)value->u.integer;
                }

            }
            else if(!strcmp (name,"Type")){
                GMTileType *t = content->first;
                printf("Acquired tile wall type: %lld\n", value->u.integer);
                t->type = (int)value->u.integer;
            }
            else if(!strcmp (name,"Chance")){
                GMScatterChance *t = content->first->scatter;
                printf("Acquired scatter chance: %lld\n", value->u.integer);
                t->chance = (int)value->u.integer;
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
            if(!strcmp (name,"Name")){
                GMTileType *t = content->first;
                printf("Acquired Tile Name: %s\n", value->u.string.ptr);
                t->name = getCopyFromString(value->u.string.ptr);

            }
            else if(!strcmp (name,"ImageDir")){
                GMTileType *t = content->first;
                printf("Acquired Tile ImageDir: %s\n", value->u.string.ptr);
                t->imageDir = getCopyFromString(value->u.string.ptr);
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



GPMap* generateAutomataMap(GPMap *map){
    // Busca Informação Básica
    char* contentFilePath = map->contentPathSet;
    FILE *contentFile;
    struct stat filestatus;
    int file_size;
    char* file_contents;

    if ( stat(contentFilePath, &filestatus) != 0) {
        fprintf(stderr, "File %s not found\n", contentFilePath);
        return NULL;
    }
    file_size = (int)filestatus.st_size;
    file_contents = (char*)malloc(filestatus.st_size);
    if ( file_contents == NULL) {
        fprintf(stderr, "Memory error: unable to allocate %d bytes\n", file_size);
        return NULL;
    }


    contentFile = fopen(contentFilePath, "rt");
    if (contentFile == NULL) {
        fprintf(stderr, "Unable to open %s\n", contentFilePath);
        fclose(contentFile);
        free(file_contents);
        return NULL;
    }
    if ( fread(file_contents, file_size, 1, contentFile) != 1 ) {
        fprintf(stderr, "Unable to read content of %s\n", contentFilePath);
        fclose(contentFile);
        free(file_contents);
        return NULL;
    }
    fclose(NULL);


    printf("%s\n", file_contents);

    printf("starting to tileA: %d %d\n", map->width, map->height);

    json_char* json;
    json_value* value;
    json = (json_char*)file_contents;
    value = json_parse(json,file_size);
    printf("starting to tileB: %d %d\n", map->width, map->height);

    if (value == NULL) {
        fprintf(stderr, "Unable to parse data in %s; JSON structure is probably not valid. Try dubbuging at http://jsonlint.com \n", contentFilePath);
        free(file_contents);
        exit(1);
    }
    content = malloc(sizeof(GMAutomataContent));
//    GMTileType *a = malloc(sizeof(GMTileType));
//    content->first = a;
//    GMScatterChance *b = malloc(sizeof(GMScatterChance));
//    a->scatter = b;

    process_automata_content_value(value, 0, NULL);

    json_value_free(value);
    free(file_contents);


//    GMTileType *c = content->first;
//    //c->name = "hi!";
//
//    GMTileType *t = content->first->next;
//    GMScatterChance *s = content->first->scatter;
//    // Gera Mapa
//    printf("\n---Starting types printing---\n %s \n %d \n %s \n %d \n\n %d \n %d \n", t->name, t->id, t->imageDir, t->type, s->chance, s->id);

    // Processa Mapa
    int i, j;
    for(i = 0; i < map->width; i ++){
        for(j = 0; j < map->height; j ++){
            GMTile *tiler = getTile(map, i, j);
            tiler->token = 0;
        }

    }
    GMTile *startingTile = getTile(map, 20, 20);
    printf("starting to tile\n");

    startingTile->token = 1;

    GPTileList *list = malloc(sizeof(GPTileList));
    list->tile = NULL;
    list->x = 40;
    list->y = 40;
    addTileList(list, startingTile, 40, 40);

    printf("starting to generate\n");

    while (list != NULL) {
        GMTile* t = list->tile;
        int x = list->x;
        int y = list->y;

        //start spreadding
        if(x < map->width - 1){  // To the right
            GMTile *temp = getTile(map, x + 1, y);
            if(temp->token == 0){
                spread(t, temp);
                if(temp->token == 1) //Succeeded
                    addTileList(list, temp, x + 1, y);
            }
        }

        if(y < map->height - 1){  // To below
            GMTile *temp = getTile(map, x + 1, y);
            if(temp->token == 0){
                spread(t, temp);
                if(temp->token == 1) //Succeeded
                    addTileList(list, temp, x + 1, y);
            }
        }

        if(x > 0){  // To the left
            GMTile *temp = getTile(map, x - 1, y);
            if(temp->token == 0){
                spread(t, temp);
                if(temp->token == 1) //Succeeded
                    addTileList(list, temp, x - 1, y);
            }
        }

        if(y > 0){  // To above
            GMTile *temp = getTile(map, x - 1, y);
            if(temp->token == 0){
                spread(t, temp);
                if(temp->token == 1) //Succeeded
                    addTileList(list, temp, x - 1, y);
            }
        }

        //removes entry
        list = list->next;


    }




    return map;
}

void spread(GMTile *tile, GMTile *nextTile){

    // Searches for current tile type
    GMTileType *tileType = content->first;
    while (tileType->id != tile->id) {
        tileType = tileType->next;
        if(tileType == NULL){
            printf("Could not find content tile of Id %d when attempting to spread from tile with Id %d. Aborting.\n", tile->id, tile->id);
            exit(0);
        }
    }
    //Searches and sums scatter chances
    GMScatterChance* scatter = tileType->scatter;
    if(scatter == NULL){    //This tile does not spread
        tile->token = 2;
        return;
    }
    int sum = 0;
    while (scatter->next != NULL) {
        sum += scatter->chance;
        scatter = scatter->next;
    }

    scatter = tileType->scatter;    //Returns to head.
    if (sum < 100) sum = 100;
    int random = PMrand() % sum;    //Gets random number within possible results

    // Starting scatter attempt
    while (scatter->next != NULL) {
        random -= scatter->chance;
        if(random <= 0){ // Scatter success!!

            tile->token = 2;
            GMTileType *tileType = content->first;  // returns to head.
            while (tileType->id != scatter->id) {   // searches for a type with same ID than spread.
                tileType = tileType->next;
                if(tileType == NULL){
                    printf("Could not find content tile of Id %d when attempting to spread from tile with Id %d. Aborting.\n", scatter->id, tile->id);
                    exit(0);
                }
            }
            // Found type
            tile->id = tileType->id;
            tile->name = tileType->name;
            tile->imageDir = tileType->imageDir;
            tile->type = tileType->type;
            tile->token = 1; // set for spreadding.
            return; // Success
        }
    }

    // reached end of scatter line without result: no spread
    tile->token = 2;
}

void addTileList(GPTileList *l, GMTile* t, int x, int y){
    GPTileList *new = malloc(sizeof(GPTileList));
    new->tile = t;
    new->x = x;
    new->y = y;
    new->next = NULL;
    GPTileList *temp = l;

    if(temp == NULL){
        l = new;
        return;
    }
    if(temp->tile == NULL){
        l = new;
        return;
    }
    while (l->next != NULL) {
        l = l->next;
    }
    l->next = new;
}



