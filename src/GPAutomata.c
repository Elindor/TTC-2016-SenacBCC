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

static void process_content_value(json_value* value, int depth, char* name);

static void process_content_object(json_value* value, int depth)
{
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++) {
        print_depth_shift(depth);
        printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
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
        
        process_content_value(value->u.array.values[x], depth, NULL);
    }
}

static void process_content_value(json_value* value, int depth, char* name)
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
                    printf("Acquired tile Id: %lld\n", value->u.integer);
                    t->id = (int)value->u.integer;
                }
                if(generatingScatter){
                    GMScatterChance *t = content->first->scatter;
                    printf("Acquired scatter Id: %lld\n", value->u.integer);
                    t->id = (int)value->u.integer;
                }
//                map->width = (int)value->u.integer;
//                printf("Acquired width: %d\n", map->width);
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
    
   
    GMTileType *c = content->first;
    //c->name = "hi!";
    
    GMTileType *t = content->first->next;
    GMScatterChance *s = content->first->scatter;
    // Gera Mapa
    printf("\n---Starting types printing---\n %s \n %d \n %s \n %d \n\n %d \n %d \n", t->name, t->id, t->imageDir, t->type, s->chance, s->id);
    
    // Processa Mapa
    
    return map;
}





