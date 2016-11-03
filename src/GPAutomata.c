#include "GPAutomata.h"
GMAutomataContent *content;
int generatingTile;
int generatingScatter;
GPTileListHead *tileList;
//int order[4] = {0, 2, 3, 1};
//int order[5] = {1, 3, 2, 0, 0};
int order[4] = {-1, 0, 0, 0};
int scount = 0;
int ponderingAdjacency = 1;

static void print_depth_shift(int depth)
{
    int j;
    for (j=0; j < depth; j++) {
//        printf(" ");
    }
}

static void process_content_value(json_value* value, int depth, char* name);
void addTileList(GMTile* t, int x, int y);
void spread(GPMap *map, GMTile *tile, GMTile *nextTile, int x, int y);
int ponderAdjacency(GPMap *map, GMTile *tile, GMTile *nextTile, int x, int y);
void printList(GPTileList *node);

static void process_content_object(json_value* value, int depth)
{
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++) {
        print_depth_shift(depth);
        //printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
        process_content_value(value->u.object.values[x].value, depth+1, value->u.object.values[x].name);
    }
}

static void process_content_array(json_value* value, int depth)
{
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.array.length;
//    printf("array\n");
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
        
        process_content_value(value->u.array.values[x], depth, NULL);
    }
}

static void process_content_value(json_value* value, int depth, char* name)
{
    //    int j;
    if (value == NULL) {
//        printf("NULL value\n");
        return;
    }
    if (value->type != json_object) {
        print_depth_shift(depth);
    }
    
    switch (value->type) {
        case json_none:
//            printf("none\n");
            break;
        case json_object:
            process_content_object(value, depth+1);
            break;
        case json_array:
            if(name == NULL)
                return;
            if(!strcmp (name,"Tiles")){
                generatingTile = 1;
                process_content_array(value, depth+1);
                generatingTile = 0;

            }
            if(!strcmp (name,"ScatterRules")){
                generatingScatter = 1;
                generatingTile = 0;
                process_content_array(value, depth+1);
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
//                    printf("Acquired tile Id: %lld\n", value->u.integer);
                    t->id = (int)value->u.integer;
                }
                if(generatingScatter){
                    GMScatterChance *t = content->first->scatter;
//                    printf("Acquired scatter Id: %lld\n", value->u.integer);
                    t->id = (int)value->u.integer;
                }

            }
            else if(!strcmp (name,"Type")){
                GMTileType *t = content->first;
//                printf("Acquired tile wall type: %lld\n", value->u.integer);
                t->type = (int)value->u.integer;
            }
            else if(!strcmp (name,"Chance")){
                GMScatterChance *t = content->first->scatter;
//                printf("Acquired scatter chance: %lld\n", value->u.integer);
                t->chance = (int)value->u.integer;
            }
//            else
//                printf("int: %10" PRId64  "\n", value->u.integer);
            
            break;
        case json_double:
//            printf("double: %f\n", value->u.dbl);
            break;
        case json_string:
            if(name == NULL)
                return;
            if(!strcmp (name,"Name")){
                GMTileType *t = content->first;
//                printf("Acquired Tile Name: %s\n", value->u.string.ptr);
                t->name = getCopyFromString(value->u.string.ptr);

            }
            else if(!strcmp (name,"ImageDir")){
                GMTileType *t = content->first;
//                printf("Acquired Tile ImageDir: %s\n", value->u.string.ptr);
                t->imageDir = getCopyFromString(value->u.string.ptr);
            }
//            else printf("string: %s\n", value->u.string.ptr);
            break;
        case json_boolean:
//            printf("bool: %d\n", value->u.boolean);
            break;
        default:
            break;
    }
}



















GPMap* generateAutomataMap(GPMap *map){
    scount = 0;
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
    fclose(contentFile);
    
    
//    printf("%s\n", file_contents);
    
    json_char* json;
    json_value* value;
    json = (json_char*)file_contents;
    value = json_parse(json,file_size);

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
    
    process_content_value(value, 0, NULL);
    
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
    startingTile->id = 1;
//    printf("starting to tile\n");

    startingTile->token = 1;

    tileList = malloc(sizeof(GPTileListHead));
    tileList->first = NULL;

    addTileList(startingTile, 20, 20);
    
//    printf("starting to generate %d\n", tileList->first->x);
    
    while (tileList->first != NULL) {

        GMTile* t = tileList->first->tile;
        int x = tileList->first->x;
        int y = tileList->first->y;
        
//        printf("Removing 1st item %d from list: ", t->id);
        printList(tileList->first);
        
        //removes entry
        tileList->first = tileList->first->next;
        

//        printf("generating tile at indexes %d %d\n", x, y);
        
        //start spreadding
        if(x < map->width - 1){  // To the right
            GMTile *temp = getTile(map, x + 1, y);
            if(temp->token == 0){
                spread(map, t, temp, x + 1, y);
                
                if(temp->token == 1) //Succeeded
                    addTileList(temp, x + 1, y);
            }
        }
        
        if(y < map->height - 1){  // To below
            GMTile *temp = getTile(map, x, y + 1);
            if(temp->token == 0){
                spread(map, t, temp, x, y + 1);
                if(temp->token == 1) //Succeeded
                    addTileList(temp, x, y + 1);
            }
        }
        
        if(x > 0){  // To the left
            GMTile *temp = getTile(map, x - 1, y);
            if(temp->token == 0){
                spread(map, t, temp, x - 1, y);
                if(temp->token == 1) //Succeeded
                    addTileList(temp, x - 1, y);
            }
        }
        
        if(y > 0){  // To above
            GMTile *temp = getTile(map, x, y - 1);
            if(temp->token == 0){
                spread(map, t, temp, x, y - 1);
                if(temp->token == 1) //Succeeded
                    addTileList(temp, x, y - 1);
            }
        }
        
        
        
        
    }
    
    map->generation = scount;
    
    
    return map;
}

void spread(GPMap *map, GMTile *tile, GMTile *nextTile, int x, int y){
    scount++;
    if(ponderingAdjacency){
        int result = ponderAdjacency(map, tile, nextTile, x, y);
        if(result >= 0) return;
        printf("result: %d.\n", result);

    }
    
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
    while (scatter != NULL) {
        random -= scatter->chance;
        printf("random = %d, SId: %d.\n", random, scatter->id);

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
            nextTile->id = tileType->id;
//            printf("tile of Id %d spawned from tile of Id %d\n", nextTile->id, tile->id);

            nextTile->name = tileType->name;
            nextTile->imageDir = tileType->imageDir;
            nextTile->type = tileType->type;
            nextTile->token = 1; // set for spreadding.
            return; // Success
        }
        scatter = scatter->next;
    }
    
    // reached end of scatter line without result: no spread
    tile->token = 2;
}


int ponderAdjacency(GPMap *map, GMTile *tile, GMTile *nextTile, int x, int y){
    GMTileType *tileType = content->first;
    //Searches and sums scatter chances

    
    GMScatterChance* scatter = tileType->scatter;
    if(scatter == NULL){    //This tile does not spread
        tile->token = 2;
        return -1;
    }
    
    

    GMScatterChance* newScatter = malloc(sizeof(GMScatterChance));
    newScatter->next = NULL;
    newScatter->id = -1;
    GMScatterChance *currentScatter = newScatter;

    while (tileType!= NULL) {
        if(currentScatter->id < 0){
            currentScatter->id = tileType->id;
            currentScatter->chance = 0;
        }
        else{
            GMScatterChance* new = malloc(sizeof(GMScatterChance));
            new->id = tileType->id;
            new->chance = 0;
            new->next = NULL;
            currentScatter-> next = new;
            currentScatter = new;
        }
        tileType = tileType->next;
    }
    GMScatterChance *currScatter;
    int i, currX, currY;
    for(i = 0; i < 4; i++){
        currX = currY = -1; //invalid
        if(i == 0 && x > 0){    // Check Left
            currX = x - 1;
            currY = y;
        }
        if(i == 1 && x < map->width - 1){    // Check Right
            currX = x + 1;
            currY = y;
        }
        if(i == 2 && y > 0){    // Check Down
            currX = x;
            currY = y - 1;
        }
        if(i == 3 && y < map->height - 1){    // Check Above
            currX = x;
            currY = y + 1;
        }
        
        if(currX != -1 && currY != -1){
            GMTile * temp = getTile(map, currX, currY);
            if(temp->token > 0){ // Has been proccessed.
                tileType = content->first;
                while (tileType->id != temp->id) {
                    tileType = tileType->next;
                    if(tileType == NULL){
                        exit(0);
                    }
                }
                
                currScatter = newScatter;

                // Sum up probabilities and set to -1 those that are not found in one of the tiles.
                while (currScatter != NULL) {
                    GMScatterChance *tileScatter = tileType->scatter;
                    int found = 0;
                    
                    if (currScatter->chance != -1)  //
                        while (tileScatter != NULL && !found) {
                            if(currScatter->id == tileScatter->id){
                                currScatter->chance += tileScatter->chance;
                                found = 1;
                            }
                            else tileScatter = tileScatter->next;

                            if(tileScatter == NULL) currScatter->chance = -1; // Cannot spread to that.
                            
                        }
                    currScatter = currScatter->next;
                }
                
            }
        }
        
    }

    
    // Removes links with negative chance. If all are negative, throws response -1.
    // Also sums total chance
    int sum = 0;
    while (newScatter != NULL && newScatter->chance == -1){
        currScatter = newScatter;
        newScatter = newScatter->next;
        free(currScatter); currScatter = NULL;
    }
    if(newScatter == NULL) return -1; // LIST is null;
    currScatter = newScatter;
    GMScatterChance * prevScatter = currScatter;
    while (currScatter != NULL){
        if(currScatter->chance == -1){
            GMScatterChance* dead = currScatter;
            prevScatter->next = currScatter->next;
            currScatter = currScatter->next;
            free(dead); dead = NULL;
        }
        else{
            sum += currScatter->chance;
            prevScatter = currScatter;
            currScatter = currScatter->next;
            
        }
    }
    
    // sorts a tile
//    if(sum < 100) sum = 100;
    int random = PMrand() % sum;    //Gets random number within possible results
    currScatter = newScatter;
    
    while (currScatter != NULL) {
        random -= currScatter->chance;
        if(random <= 0){ // Scatter success!!
            tile->token = 2;
            GMTileType *tileType = content->first;  // returns to head.
            while (tileType->id != currScatter->id) {   // searches for a type with same ID than spread.
                tileType = tileType->next;
            }
            // Found type
            printf("Success: tile %d generated at x and y %d:%d\n", tileType->id, x, y);

            nextTile->id = tileType->id;
            //            printf("tile of Id %d spawned from tile of Id %d\n", nextTile->id, tile->id);
            
            nextTile->name = tileType->name;
            nextTile->imageDir = tileType->imageDir;
            nextTile->type = tileType->type;
            nextTile->token = 1; // set for spreadding.
            
            // limpa a lista
            currScatter = newScatter;
            GMScatterChance* dead;;

            while (currScatter != NULL){
                dead = currScatter;
                currScatter = currScatter->next;
                free(dead);
                
            }
            return 1; // Success
        }
        currScatter = currScatter->next;
    }
    // No spread
    printf("Failure %d:%d\n", x, y);
    tile->token = 2;
    currScatter = newScatter;
    while (currScatter != NULL){
        GMScatterChance* dead = currScatter;
        currScatter = currScatter->next;
        free(dead); dead = NULL;
    }
    return 0; // No change.

}


void addTileList(GMTile* t, int x, int y){
    GPTileList *new = malloc(sizeof(GPTileList));
    new->tile = t;
    new->x = x;
    new->y = y;
    new->next = NULL;
    GPTileList *temp = tileList->first;
    
    if(temp == NULL){
        tileList->first = new;
        return;
    }
    if(temp->tile == NULL){
        tileList->first = new;
        return;
    }
    
    if(order[0] != -1){
//
//        printf("\n\n spawning %d\n", new->tile->id);

        
        int tokenIndex = 0;
        int found = 0;
        int tokenOrder = order[tokenIndex];
        
        
        for(; tileList->first->tile->id != new->tile->id && tileList->first->tile->id != tokenOrder; tokenIndex++){
            tokenOrder = order[tokenIndex];
            if(new->tile->id == tokenOrder){
                new->next = tileList->first;
                tileList->first = new;
                
                printList(tileList->first);
//                printf("\n\n first %d\n", new->tile->id);

                return;
            }
        }
        tokenIndex = 0;
        tokenOrder = order[tokenIndex];
        
        while (temp->next != NULL) {
//            printf("looping %d %d\n", found, tokenIndex);

            if(new->tile->id != tokenOrder && found == 0){

                if(temp->tile->id == temp->next->tile->id || temp->next->tile->id == tokenOrder){
                    temp = temp->next;
                }
                else{
//                    temp = temp->next;
                    while (tokenOrder != temp->next->tile->id && tokenOrder != new->tile->id) {
                        tokenIndex++;
                        tokenOrder = order[tokenIndex];
                        if(tokenOrder == 0)
                            //printf("FAIL ILLPALAZZO\n");
                        if(tokenOrder == new->tile->id)
                            found = 1;
//                        else
//                            temp = temp->next;
                    }
                }
            }
            else{

//                found = 1;
                if(temp->tile->id != temp->next->tile->id){
//                    temp = temp->next;
                    new->next = temp->next;
                    temp->next = new;
//                    printf("temp:  %d  new:  %d\n", temp->tile->id, new->tile->id);
                    printList(tileList->first);
                    return;

                }
                else{
                    temp = temp->next;
                }
            }
            
        }
//        printf("added to tail: %d, List:", new->tile->id);
        printList(tileList->first);

        temp->next = new;
    }
    else{
        if(0){
            new->next = tileList->first;
            tileList->first = new;
            return;
        }
        else{
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new;
        }
        
    }
    
}



void printList(GPTileList *node)
{
    return;
    while (node != NULL)
    {
        printf("%d  ", node->tile->id);
        node = node->next;
    }
    printf("\n");
}



