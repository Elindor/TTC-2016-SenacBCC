#include "GPMap_Genetic.h"

GGnode *head;
GPRoomList *current;
GMRoomContent *content;
GMAutomataContent *content1;
static GPGenetic_Map *map;

static void print_depth_shift(int depth)
{
    int j;
    for (j=0; j < depth; j++) {
        printf(" ");
    }
}





int generatingTile;
int generatingScatter;
int generatingRoom;
int generatingMap;
int generatingDoor;

int generatingIncompaexit;
int generatingIncompaentrade;
int generatingCompati;

static void process_genetic_content_value(json_value* value, int depth, char* name);


static void process_genetic_content_object(json_value* value, int depth)
{
    int length, x;
    if (value == NULL) {
        return;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++) {
        print_depth_shift(depth);
        printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
        process_genetic_content_value(value->u.object.values[x].value, depth+1, value->u.object.values[x].name);
    }
}

static void process_genetic_content_array(json_value* value, int depth)
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

            GMTileType *a = content1->first;
            GMTileType *b = malloc(sizeof(GMTileType));
            b->next = a;
            content1->first = b;
        }
        if(generatingRoom){

            if(content->first == NULL){
                GMRoom *a = malloc(sizeof(GMRoom));
                a->next = NULL;
                a->doors = NULL;
                a->gene = NULL;
                a->compativeExit = NULL;
                a->incompativeEntrad = NULL;
                a->incompative = NULL;
                content->first = a;
            }
            else{
                GMRoom *a = content->first;
                GMRoom *b = malloc(sizeof(GMRoom));
                b->next = a;
                b->doors = NULL;
                b->gene = NULL;
                b->compativeExit = NULL;
                b->incompativeEntrad = NULL;
                b->incompative = NULL;
                content->first = b;
            }
        }
        else if(generatingScatter && !generatingTile){
            GMTileType *tile = content1->first;
            GMScatterChance *a = tile->scatter;
            GMScatterChance *b = malloc(sizeof(GMScatterChance));
            b->next = a;
            tile->scatter = b;
        }
        if(generatingDoor){
            if(content->first->doors == NULL){
                GMDoor *a = malloc(sizeof(GMDoor));
                a->next = NULL;
                content->first->doors = a;
            }else{
                GMDoor *a = content->first->doors;
                GMDoor *b = malloc(sizeof(GMDoor));
                b->next = a;
                content->first->doors = b;
            }

        }
        if(generatingIncompaentrade){
            GmCrossover *a = content->first->incompativeEntrad;
            GmCrossover *b = malloc(sizeof(GmCrossover));
            b->next = a;
            content->first->incompativeEntrad = b;
        }
        if(generatingIncompaexit){
            GmCrossover *a = content->first->incompative;
            GmCrossover *b = malloc(sizeof(GmCrossover));
            b->next = a;
            content->first->incompative = b;
        }
        if(generatingCompati){
            GmCrossover *a = content->first->compativeExit;
            GmCrossover *b = malloc(sizeof(GmCrossover));
            b->next = a;
            content->first->compativeExit = b;
        }

        process_genetic_content_value(value->u.array.values[x], depth, NULL);
    }
}

static void process_genetic_content_value(json_value* value, int depth, char* name)
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
            process_genetic_content_object(value, depth+1);
            break;
        case json_array:
            if(name == NULL)
                return;
            if(!strcmp (name,"Tiles")){
                generatingTile = 1;
                generatingRoom = 0;
                process_genetic_content_array(value, depth+1);
                generatingTile = 0;
                generatingRoom = 0;
            }
            if(!strcmp (name,"ScatterRules")){
                generatingScatter = 1;
                generatingTile = 0;

                process_genetic_content_array(value, depth+1);

                generatingScatter = 0;
                generatingTile = 1;
            }
            if(!strcmp (name,"Rooms")){
                generatingRoom = 1;
                generatingTile = 0;
                process_genetic_content_array(value, depth+1);
                generatingTile = 0;
                generatingRoom = 0;

            }
            if(!strcmp (name,"Map")){
                generatingMap = 1;
                generatingDoor = 0;

                generatingRoom = 0;
                generatingScatter = 0;
                generatingTile = 0;

                process_genetic_content_array(value, depth+1);

                generatingMap = 0;
                generatingDoor = 0;

                generatingRoom = 1;
                generatingScatter = 0;
                generatingTile = 0;
            }
            if(!strcmp (name,"Doors")){
                generatingMap = 0;
                generatingDoor = 1;

                generatingRoom = 0;
                generatingScatter = 0;
                generatingTile = 0;

                process_genetic_content_array(value, depth+1);

                generatingMap = 0;
                generatingDoor = 0;

                generatingRoom = 1;
                generatingScatter = 0;
                generatingTile = 0;
            }

            if(!strcmp (name,"IncompatibilityWithExit")){
                generatingMap = 0;
                generatingDoor = 0;

                generatingRoom = 0;
                generatingScatter = 0;
                generatingTile = 0;
                generatingCompati = 0;
                generatingIncompaentrade = 0;
                generatingIncompaexit = 1;

                process_genetic_content_array(value, depth+1);

                generatingCompati = 0;
                generatingIncompaentrade = 0;
                generatingIncompaexit = 0;
                generatingMap = 0;
                generatingDoor = 0;

                generatingRoom = 1;
                generatingScatter = 0;
                generatingTile = 0;
            }
            if(!strcmp (name,"IncompatibilityWithEntrad")){
                generatingMap = 0;
                generatingDoor = 0;

                generatingRoom = 0;
                generatingScatter = 0;
                generatingTile = 0;
                generatingCompati = 0;
                generatingIncompaentrade = 1;
                generatingIncompaexit = 0;

				process_genetic_content_array(value, depth+1);

				generatingCompati = 0;
                generatingIncompaentrade = 0;
                generatingIncompaexit = 0;
                generatingMap = 0;
                generatingDoor = 0;

                generatingRoom = 1;
                generatingScatter = 0;
                generatingTile = 0;
            }
            if(!strcmp (name,"CompatibilityWithExit")){
                generatingMap = 0;
                generatingDoor = 0;

                generatingRoom = 0;
                generatingScatter = 0;
                generatingTile = 0;
                generatingCompati = 1;
                generatingIncompaentrade = 0;
                generatingIncompaexit = 0;

                process_genetic_content_array(value, depth+1);

                generatingCompati = 0;
                generatingIncompaentrade = 0;
                generatingIncompaexit = 0;
                generatingMap = 0;
                generatingDoor = 0;

                generatingRoom = 1;
                generatingScatter = 0;
                generatingTile = 0;
            }

            break;
        case json_integer:
            if(name == NULL)
                return;
            if(!strcmp (name,"Id")){
                if(generatingTile){
                    GMTileType *t = content1->first;
                    printf("Acquired tile Id: %lld\n", value->u.integer);
                    t->id = (int)value->u.integer;
                }
                if(generatingScatter){
                    GMScatterChance *t = content1->first->scatter;
                    printf("Acquired scatter Id: %lld\n", value->u.integer);
                    t->id = (int)value->u.integer;
                }
                if(generatingRoom){
                    GMRoom *t = content->first;
                    printf("Acquired room Id: %lld\n", value->u.integer);
                    t->id = (int)value->u.integer;

                }
                if(generatingDoor){
                    GMDoor *t = content->first->doors;
                    printf("Acquired door Id: %lld\n", value->u.integer);
                    t->idDoor = (int)value->u.integer;
                    t->idNextdoor = 0;
                    printf("ID NEXT: %d\n", t->idNextdoor);
                }

            }
            else if(!strcmp (name,"Type")){
                GMTileType *t = content1->first;
                printf("Acquired tile wall type: %lld\n", value->u.integer);
                t->type = (int)value->u.integer;
            }
            else if(!strcmp (name,"Chance")){
			//
				if(generatingScatter){
                    GMScatterChance *t = content1->first->scatter;
					printf("Acquired scatter chance: %lld\n", value->u.integer);
					t->chance = (int)value->u.integer;
				}
				if(generatingRoom){
					GMRoom *t = content->first;
					printf("Acquired Room chance: %lld\n", value->u.integer);
					t->chance = (int)value->u.integer;
				}
				if(generatingCompati){
					GmCrossover *t = content->first->compativeExit;
					printf("Acquired compative chance: %lld\n", value->u.integer);
					t->chance = (int)value->u.integer;
				}
            }
            else if(!strcmp (name,"Tam")){
                GMRoom *t = content->first;
                printf("Acquired Tamanho de sala: %lld\n", value->u.integer);
                t->Tam = (int)value->u.integer;
            }

			else if(!strcmp (name,"Width")){

				if(generatingRoom){
					GMRoom  *t = content->first;
					printf("Acquired room width: %lld\n", value->u.integer);
					t->width = (int)value->u.integer;
				}
				if(generatingDoor){
					GMDoor  *t = content->first->doors;
					printf("Acquired door width: %lld\n", value->u.integer);
					t->idRoomY = (int)value->u.integer;
				}
			}
			else if(!strcmp (name,"Height")){

				if(generatingRoom){
					GMRoom  *t = content->first;
					printf("Acquired room height: %lld\n", value->u.integer);
					t->height = (int)value->u.integer;
				}
				if(generatingDoor){
					GMDoor  *t = content->first->doors;
					printf("Acquired door height : %lld\n", value->u.integer);
					t->idRoomX = (int)value->u.integer;
				}
            }

			else if(!strcmp (name,"RoomId")){
				//
				if(generatingCompati){
					GmCrossover *t = content->first->compativeExit;
					printf("Acquired Room ID for compatividade: %lld\n", value->u.integer);
					t->id = (int)value->u.integer;
				}
				if(generatingIncompaentrade){
					GmCrossover *t = content->first->incompativeEntrad;
					printf("Acquired Room ID de incompatividade de entrada: %lld\n", value->u.integer);
					t->id = (int)value->u.integer;
				}
				if(generatingIncompaexit){
					GmCrossover *t = content->first->incompative;
					printf("Acquired Room ID de incompatividade de saida: %lld\n", value->u.integer);
					t->id = (int)value->u.integer;
				}
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
                if(generatingTile){
                    GMTileType *t = content1->first;
                    printf("Acquired Tile Name: %s\n", value->u.string.ptr);
                    t->name = getCopyFromString(value->u.string.ptr);
                }
                if(generatingRoom){
                    GMRoom *t = content->first;
                    printf("Acquired Room Name: %s\n", value->u.string.ptr);
                    //t->name, = getCopyFromString(value->u.string.ptr);
                     strcpy(t->name, value->u.string.ptr);
                }
            }
            else if(!strcmp (name,"ImageDir")){
                GMTileType *t = content1->first;
                printf("Acquired Tile ImageDir: %s\n", value->u.string.ptr);
                t->imageDir = getCopyFromString(value->u.string.ptr);
            }
            else if(!strcmp (name,"Gene")){
                if(generatingRoom){
                    GMRoom *t = content->first;
                    printf("Acquired Room genes: %s\n", value->u.string.ptr);
                    t->gene = getCopyFromString(value->u.string.ptr);
                }
                if(generatingDoor){
                    GMDoor *t = content->first->doors;
                    printf("Acquired Door gene: %s\n", value->u.string.ptr);
                    t->gene = getCopyFromString(value->u.string.ptr);
                }
                if(generatingIncompaentrade)
                {
                    GmCrossover *t = content->first->incompativeEntrad;
                    printf("Acquired gene for incompative entrande: %s\n", value->u.string.ptr);
                    t->genes = getCopyFromString(value->u.string.ptr);
                }
                if(generatingIncompaexit)
                {
                    GmCrossover *t = content->first->incompative;
                    printf("Acquired gene for incompative exit: %s\n", value->u.string.ptr);
                    t->genes = getCopyFromString(value->u.string.ptr);
                }
                if(generatingCompati)
                {
                    GmCrossover *t = content->first->compativeExit;
                    printf("Acquired Compative gene: %s\n", value->u.string.ptr);
                    t->genes = getCopyFromString(value->u.string.ptr);
                }
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

GPMap* generateGeneticMap(GPMap *mapa){


    int saida = 1;
	int  tentativas = 0;
	GmPonto g;
    int x, y;


    map = malloc(sizeof(GPGenetic_Map));

    map->height = mapa->height;
    map->width = mapa->width;
    map->name =mapa->name  ;

    map->conditionFilePath =mapa->conditionFilePath;
    map->contentPathSet =mapa->contentPathSet;
    map->coreFilePath = mapa->coreFilePath;

    map->maximumContent = mapa->maximumContent;
    map->minimalContent =mapa->minimalContent;



    map->grid = (GMRoom***)malloc(sizeof(GMRoom**)*map->width);
    printf("%d %d\n", map->height, map->width);
    int i, j;
   // printf("%d\n", map->width);
    for(i = 0; i < map->height; i ++){
        map->grid[i] = (GMRoom**)malloc(sizeof(GMRoom*)*map->height);

        for(j = 0; j < map->height; j ++){

            GMRoom *tiler = malloc(sizeof(GMRoom));
            tiler->id = 0;
            tiler->next = NULL;
            strcpy(tiler->name, "name");
            printf("%d %d\n", i, j);

            map->grid[i][j] = tiler;
        }
    }
    printf("%d %d\n", map->height, map->width);



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


    contentFile = fopen(contentFilePath, "rb");
    if (contentFile == NULL) {
        fprintf(stderr, "Unable to open %s\n", contentFilePath);
        fclose(contentFile);
        free(file_contents);
        return NULL;
    }
    if ( fread(file_contents, file_size, 1, contentFile) != 1 ) {
                if (feof(contentFile)) {
                  puts ("End-of-File reached.");
                  printf ("Total number of bytes read: %d\n", file_size);
                }
        fprintf(stderr, "Unable to read content of %s\n", contentFilePath);
        fclose(contentFile);
        free(file_contents);
        return NULL;
    }

    fclose(contentFile);


    printf("%s\n", file_contents);

    printf("starting to tileA: %d %d\n", map->width, map->height);

    json_char* json;
    json_value* value;

    json = (json_char*)file_contents;
    value = json_parse(json,file_size);

    printf("B\n");

    if (value == NULL) {
        fprintf(stderr, "Unable to parse data in %s; JSON structure is probably not valid. Try dubbuging at http://jsonlint.com \n", contentFilePath);
        free(file_contents);
        exit(1);
    }
        printf("C\n");

    content = malloc(sizeof(GMRoomContent));
    content->first = NULL;
    content1 = malloc(sizeof(GMAutomataContent));
        printf("D\n");

    head = malloc(sizeof(GGnode));
    head->sala = NULL;
    current = (GPRoomList*) calloc(1,sizeof(GPRoomList));
//    GMTileType *a = malloc(sizeof(GMTileType));
//    content->first = a;
//    GMScatterChance *b = malloc(sizeof(GMScatterChance));
//    a->scatter = b;
    printf("Pre-process\n");

    process_genetic_content_value(value, 0, NULL);

    json_value_free(value);
    free(file_contents);
    printf("li os arquivos\n");


//    GMTileType *c = content->first;
//    //c->name = "hi!";
//
//    GMTileType *t = content->first->next;
//    GMScatterChance *s = content->first->scatter;
//    // Gera Mapa
//    printf("\n---Starting types printing---\n %s \n %d \n %s \n %d \n\n %d \n %d \n", t->name, t->id, t->imageDir, t->type, s->chance, s->id);

    // Processa Mapa
    printf("%d %d\n", map->height, map->width);


    head->sala = content->first;
    int startX = 3;//PMrand()%map->height;
    int starty = 3; //PMrand()%map->width;
    printf("Pontos inicial %d ---%d\n", startX,starty);
    x = startX;
	g.x = startX;

    y =  starty;
    g.y = starty;
        i = PMrand()%10;
        i =  i+1;

        printList();

    GMRoom* res = malloc(sizeof(GMRoom));
    res = seach(i, head);
    if(res != NULL)
        map->grid[startX][starty] = res;        printList();
    //aqui funciona
   // system("cls");

  // printList();
 //   system("pause");
//printf("\n\nSala inicial id: %d\nSala inicial nome: %s\nSala inicial Chance: %d\n\n", map->grid[g.x][g.y]->id, map->grid[startX][starty]->name, map->grid[startX][starty]->chance);
    //printList();

    while (saida) {
        //aqui não funciona
        //valor errado usando a mesma condição
        //printf("OI\n");

        g = selectdoor(map->grid[x][y], g.x, g.y);
       //printf("Olha passe vamo ver %s",g.genes);
		if(strcmp(g.genes,"I")){
            printf("indo fazer a lista 1\n");
            current = GerateListPosibylit(g.genes, current);
            printf("Fiz a lista 1\n");
            printList();
            printListTemp();
            printf("Indo para lista 2\n");
            current =SeachIncopatibility(g.genes, current);
            printList();
            printListTemp();
            printf("Indo para lista 3\n");
            current = SeachIncopatibilityid(map->grid[x][y]->id, current);
            printf("fiz a lista 3\n");
            printList();
            printListTemp();
            printf("Indo para lista 4\n");
            current = SeachIncopatibilityExit(map->grid[x][y], current);
            printf("Fiz a lista 4\n comecando a deletar portas vazias\n");
            printList();
            printListTemp();


            current = DeleteVoidDoors(map,x,y,g,current);
            printf("removidas portas vazia\n");
            printList();
            printListTemp();
            printf("\nAdiconando compatibilidade\n");
            current =SeachCopatibility(map->grid[x][y],current);
            printf("Compartibilidades adicionadas\n Sorteando\n");
            i = Sort_Rooom(current);
            printf("Fixando a sala %d no ponto %d--%d\n", i, g.x,g.y);

            GMRoom* res = malloc(sizeof(GMRoom));
            res = seach(i, head);
            if(res != NULL)
                map->grid[g.x][g.y] = res;

            printf("\nSala fixada\n");
            //printf("\n\nSala id: %d\nSala nome: %s\n\n", map->grid[g.x][g.y]->id, map->grid[g.x][g.y]->name);

            printf("\nFixado portas\n");
			fixdoor(map, x,y, g); //liga as portas de ambas as salas x y e px py, identificação da sala, g identificação da porta.

            printf("\nSala e portas fixadas, reiniciando\n");
			x = g.x;
			y = g.y;
            int con;
            printf("Vendo quanto do mapa esta preenchido\n");
			con = ocuppedspace(map);

			if(con == 2){
                g = varremapapor(map,g);
                if( g.x != -1){
                    tentativas++;
                    if(tentativas < 5){
                        printf("Vou descartar");
                        discartmap(map);
                    }else{
                        printf("Numero limite de descarte atingido");
                        saida = 0;
                    }
                }
			}else if(con == 1){
                printf("\nMinimo de salas atingido\n");

			}else{
                 printf("\nAinda nao atingido o minimo\n");

			}


		}else{
			printf("Not found void next door.");
			int con = -1;
			map->grid[x][y]->alldoorsocupped = 1;
			g = seachNextRoom(map, g); //Encontra a proxima sala que se pode continuar a geraççao de portas --- arrumar uma maneira de identificar sala de 2 portas

			if(g.x == -1){
				g = varremapapor(map,g);//função muito ruim
                if(g.x == -1){
                    con = ocuppedspace(map);
                }else{
                    con = ocuppedspace(map);
                    if(con == 2){
                       tentativas++;
                       if(tentativas < 5){
                        printf("Vou descartar");
                        discartmap(map);
                    }else{
                        printf("Numero limite de descarte atingido");
                        saida = 0;
                    }
                            }
                }

			}

			if(con == 0 || con == 2){

                //se pegou o mapa completo ainda inclui certo
                printf("Todas as condições atendidas\n");
                for (i = 0; i < map->height; i++){
                    for (j = 0; j < map->width; j++){
                        mapa->grid[i][j]->id = map->grid[i][j]->id;
                    }
                }

                saida = 0;

			}
		}

    }



    return mapa;
}

void fixdoor(GPGenetic_Map *mapa, int x, int y, GmPonto ponto){
           GMDoor* previus = NULL;
           GMDoor* proximus;
           GMDoor *a;
           int i = 0, exi = 0;
           char*geneSala;
           int px, py, id1;
           px = ponto.x;
           py = ponto.y;
           printf("\nSituação atual\n---------------------------------------\n");

            printDoorList(x,y);
            printDoorList(ponto.x,ponto.y);
            printf("\n--------------------------------------------\nSituação atual\n");

//            printf("Fixando portas -- %d\n",map->grid[px][py]->id);
           printf("Invertendo o gene para fixar porta\n ");
           //inverte gene
           if(!strcmp (ponto.genes,"N") || !strcmp (ponto.genes,"n")){
                geneSala = "S";

            }else if(!strcmp (ponto.genes,"S") || !strcmp (ponto.genes,"s")){
                geneSala = "N";

            }else if(!strcmp (ponto.genes,"L") || !strcmp (ponto.genes,"l")){
                geneSala = "O";

            }else if(!strcmp (ponto.genes,"O") || !strcmp (ponto.genes,"o")){
                geneSala = "L";

            }else{
                geneSala = "I";
			}

            a = mapa->grid[x][y]->doors;
            if(mapa->grid[x][y]->doors == NULL){
                printf("Starting mapa->grid[x][y]->doors is NULL at fixDoor\n");
                exit(0);
            }

            printf("Iniciando busca\n");
            while(a != NULL && a->idDoor && exi == 0 ){
              //if it is last GGnode
              if(i ==0 ){
                previus = a;
                i++;
              }else{
                previus->next = a;
              }

              if( !strcmp(a->gene,geneSala)){
                //printf("Sala encontrada\n");
                proximus = a->next;
                 //printf("AAAAA");
                 a->idNextRoomX = ponto.x;
               //  printf("BBBBB");
                 a->idNextRoomY = ponto.y;
             //    printf("CCCCC");
                 a->idNextRoom =  mapa->grid[px][py]->id;
                 printf("Fixados pontos\n");
                 if(previus->idDoor == a->idDoor)
                 {

                        previus = a;
                        previus->next = proximus;
                        printf("Primeira sala fixada\n");
                 }else{

                    previus->next = a;
                 }
                 exi = 1;

              }
              a = a->next;
           }
            printf("Tem que sair %d %d\n", px, py);
            if(mapa->grid[px][py]){     // ERRO
                printf("grid existe\n");
                if(mapa->grid[px][py]->doors)
                    printf("tudo existe?\n");
            }
            mapa->grid[px][py]->doors = previus;
             printDoorList(x,y);

           printf("Fixando portas sala sorteada\n");
            i = 0;
            a = mapa->grid[ponto.x][ponto.y]->doors;
            if(mapa->grid[ponto.x][ponto.y]->doors == NULL){
                printf("Starting mapa->grid[x][y]->doors is NULL at fixDoor\n");
                exit(0);
            }
            printDoorList(ponto.x, ponto.y);
            printf("Iniciando busca\n");
            while(a != NULL && exi == 0 ){
              //if it is last GGnode
              if(i ==0 ){
                previus = a;
                i++;
              }else{
                previus->next = a;
              }

              if( !strcmp(a->gene,geneSala)){
                 printf("Sala encontrada\n");
                 proximus = a->next;
                 a->idNextRoomX = x;
                 a->idNextRoomY = y;
                 a->idNextRoom = mapa->grid[x][y]->id;
                 if(previus->idDoor == a->idDoor)
                 {
                        previus = a;
                        previus->next = proximus;
                 }else{
                    previus->next = a;
                 }
                 exi = 1;

              }
              a = a->next;
           }
            mapa->grid[x][y]->doors = previus;
            printDoorList(ponto.x, ponto.y);
}

GmPonto seachNextRoom(GPGenetic_Map *mapa, GmPonto ponto){
    int px = ponto.x;
    int py = ponto.y;
	GMRoom* a = mapa->grid[px][py];
	GMDoor* b = a->doors;      // DENIS b nunca é inicializado, e esse while parece nunca sair.
	while(b != NULL){

		if(mapa->grid[b->idNextRoomX][b->idNextRoomY]->alldoorsocupped != 1){
				ponto.x = b->idNextRoomX;
				ponto.y = b->idNextRoomY;
				printf("sala faltando conexão encontrada\n");
				return ponto;
		}
		b = b->next;
	}
	ponto.x = -1;
	ponto.y = -1;
	printf("nenhuma sala faltando conexão encontrada\n");
	return ponto;
}

GmPonto varremapapor(GPGenetic_Map *mapa, GmPonto ponto){
    int i, j;

    for (i = 0; i < mapa->height; i++){
        for (j = 0; j < mapa->width; j++){
            if(mapa->grid[i][j]->alldoorsocupped != 1){
                ponto.x = i;
				ponto.y = j;
				printf("Econtrada sala com portas para conectar\n");
				return ponto;
			}
		}
	}

    ponto.x = -1;
    ponto.y = -1;


	printf("Nenhuma sala aberta encontrada!\n");
    return ponto;
}

void discartmap(GPGenetic_Map *mapa){
    int i , j;
        for(i = 0; i < map->height; i ++){

        for(j = 0; j < map->height; j ++){

            GMRoom *tiler = malloc(sizeof(GMRoom));
            tiler->id = 0;
            strcpy(tiler->name,"name");
            map->grid[i][j] = tiler;

        }
    }
    int startX = PMrand()%map->height;
    int starty = PMrand()%map->width;
    i =(PMrand()%10)+1;
    GMRoom* res = malloc(sizeof(GMRoom));
    res = seach(i, head);
    if(res != NULL)
        map->grid[startX][starty] = res;

}

void printList()
{
       GMRoom *ptr = head->sala;
       GMRoom *a;
       printf("\nHead :: [ ");

       //start from the beginning
       while(ptr != NULL)
        {
            a = ptr->next;
            printf("(%d) ",ptr->id);
            ptr = a;

       }
       printf(" ]\n");
}

void printListTemp()
{
       GPRoomList *ptr = current;
       GPRoomList *a;
       printf("\nCurrent :: [ ");

       //start from the beginning
       while(ptr != NULL)
        {
            a = ptr->next;
            printf("(%d) ",ptr->room->id);
            ptr = a;

       }
       printf(" ]\n");
}

void printDoorList(int x, int y)
{
       GMDoor *ptr = map->grid[x][y]->doors;
       GMDoor *a;
       printf("\n[ ");

       //start from the beginning
       while(ptr != NULL)
        {
            a = ptr->next;
            printf("(%d) ",ptr->idDoor);
            ptr = a;

       }
       printf(" ]\n");
}

int length()
{
   int length = 0;
   GGnode *curren;

   for(curren = head; curren != NULL; curren->sala = curren->sala->next)
	{
      length++;
   }

   return length;
}

GMRoom* seach(int key, GGnode*head){

   //start from the first link
    GMRoom* curren = head->sala;
    GMRoom* a;
    printf("buscando sala %d\n", key);
   //if list is empty
    if(head == NULL)
	{
      return NULL;
    }

   //navigate through list
    while(curren != NULL && curren->id != NULL){
        printf("a\n");

      //if it is last GGnode
      if(curren->id == key){
          printf("a\n");
            a = curren;
            printf("\n\nSala inicial id: %d\nSala inicial nome: %s\nSala inicial Chance: %d\n\n", a->id, a->name, a->chance);
         return a;
      }else {
         //go to next link
         curren = curren->next;
      }
   }
     //if data found, return the current Link
   return NULL;
}

GPRoomList* GerateListPosibylit(char *gene, GGnode* alpha){ // alpha list global --- // lista para amarzenar as possibilidades usada somente na função
    // OK
   //start from the first link
   GGnode* headi;
   headi =  alpha;
    GMRoom *c;
    GPRoomList* curren = malloc(sizeof(GPRoomList));
    curren->room = NULL;
    curren->next = NULL;
   //if list is empty
   int pch;
   if(headi == NULL)
   {
       free(curren);
       return NULL;
   }
    int i = 0;
    GMRoom *a = headi->sala; // Sem malloc aqui, se n for pra criar e manter algo.
//    a->next = NULL;
    printf("\nGerando primeira lista: [");
   //navigate through list
    while((NULL != a)){//what

         pch = (int)strspn(gene,a->gene);
         //printf("\n%s -- gene ---- %s\n %d resul do pch\n", headi->gene,gene, pch);
        //c = headi->sala->next;
        //printf("\npassa merda %d", c->id);

         if(pch > 0 ){
            i++;

            if(curren->room == NULL){
                curren->room = a;
            }
            else{
                GPRoomList* new = malloc(sizeof(GPRoomList));
                new->room=a;
                new->next = curren;
                curren = new;
            }

//             free(a); // Se ele herdar headi, o ponteiro anterior fica perdido pra trás e vira lixo.
            printf(" %d", curren->room->id);
         }
        a=a->next;
       // headi->sala = c;
         //go to next link

    }
//    free(a);
   printf(" ] --- numero de Elementos: %d\n", i);
   //if found all possibilits return new list
   return curren;
}

GPRoomList* SeachCopatibility(GMRoom *atual, GPRoomList *head){
	int c = 0;
	GPRoomList* curren = head;
    int i = 0;
    GmCrossover* a;
    GMRoom* b;
    if(curren == NULL){
        printf("b is null at search compatibility\n");
        exit(0);
    }
	while(  curren != NULL)
    {
        printf("Verificando compatibilidade para sala: %d\n", b->id);
        c = 0;
   		for(a = atual->compativeExit; a != NULL; a = a->next){
			printf("ok2\n");
			if(!strcmp(curren->room->gene, a->genes)){
				printf("Gene compativel encontrado\n");
				c = a->chance;
			}
			if(a->id == curren->room->id){
				printf("ID compativel encontrado\n");
				c = a->chance;
			}
		}
		if(c>0){
            printf("Adicionando chance\n");
            i++;
            // DENIS, esse a aqui é sempre null. Pra ele sair do for ali em cima, a é null. a->chance vai dar M.
			curren->chance =  curren->room->chance + c;
		}
         curren = curren->next;
   }

   if(i == 0)
    return head;

   return curren;


}

GPRoomList* deleteid(int key, GPRoomList *head){

   //start from the first link
    GPRoomList* curren = head;
    GPRoomList* previous = NULL;

   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list
   while(curren->room->id != key){

      //if it is last GGnode
      if(curren->room->next == NULL){
         return NULL;
      }else {
         //store reference to current link
         previous = curren;
         //move to next link
         curren = curren->next;
      }

   }

   //found a match, update the link
   if(curren == head) {
      //change first to point to next link
      previous = curren;
      curren = curren->next;
      free(previous);
   }else {
      //bypass the current link
      previous->next = curren->next;
      free(curren);
   }

   return head;
}

GPRoomList*  SeachIncopatibility(char *gene, GPRoomList *head){

	int i;
	int c;
	GPRoomList* curren = head;

    GmCrossover* a =NULL;
    int pch;
    c = 0;
    i = 0;
	while(curren->room != NULL )
   {
       c= 0;

        printf("Procurando incompatibilidades genetica para sala: %d\n", curren->room->id);
        a =  curren->room->incompativeEntrad;
		while( a != NULL && c == 0){
			if(!strcmp(gene,a->genes)){
                printf("Inconpatibilidade encontrada sala: %d para \n",curren->room->id);
                curren = deleteid(curren->room->id, current);
				c = 1;
                i++;
                a = a->next;
			}
		}
		curren = curren->next;
   }


   printf("Incompatibilidades encontradas: %d\n", i);
    if(c == 0){
        //free(curren);
        return head;
    }


	return curren;

}

GPRoomList* SeachIncopatibilityid(int salaid, GPRoomList* head){
    int i;
    int c;
	GPRoomList *curren;
    curren = head;
	GmCrossover* a;
    GmCrossover* e;
    i = 0;
	while(curren!= NULL)
   {
		c = 0;
        printf("Testado incompatibilidades para sala de id: %d\n", curren->room->id);
		a = curren->room->incompativeEntrad;
		while( a != NULL && c == 0){
            printf("Verificando se existe\n");
            e = a->next;
			if( a->id == salaid){
				c = 1;
                i++;
                printf("Apagando sala de id(%d) da lista\n", curren->room->id);
                curren = deleteid(curren->room->id, curren);

			}
			a = e;
		}
        curren= curren->next;


   }


   if(i == 0)
        return head;


	return curren;


}

 GPRoomList*  SeachIncopatibilityExit(GMRoom *atual, GPRoomList *head){

	int i,c;
	GPRoomList* curren = head;
    GmCrossover* a = atual->incompative;
     i = 0;
     c = 0;
	while( curren != NULL )
   {
        printf("Verificando se existe incompatibilidades de saida com a sala: %d\n",  curren->room->id);
   		while( a != NULL && c == 0){
			if(!strcmp(curren->room->gene,a->genes) ){
				i++;
				printf("Encontrado incompatibilidade\n");
				curren = deleteid(curren->room->id, curren);
				c = 1;
			}
			if(curren->room->id == a->id){
				i++;
				printf("Encontrado incompatibilidade\n");
				curren = deleteid(curren->room->id, curren);
				c = 1;
			}
			a = a->next;
		}

        curren =curren->next;

   }
   if(i == 0)
    return head;

	return curren;

}

GPRoomList*  DeleteVoidDoors(GPGenetic_Map *mapa, int x, int y,GmPonto ponto,GPRoomList *head){

	int tx, ty, pch;
	int i = 0;
	GPRoomList* curren = head;
    int test;
        printf("Verificando se nao vai apontar para fora do mapa x: %d --- y: %d\n",x,y);
        //Remove salas que apontam para fora do mapa principal
        tx = ponto.x -1;
		if( tx < 0){
            i++;
            printf("Porta L invalida encontrada\n");
            curren = DeleteAllG("L", curren);
		}
		tx = ponto.x + 1;
		test= mapa->height;
		if(tx>test){
		    i++;
            printf("Porta O invalida encontrada\n");
            curren = DeleteAllG("O", curren);
		}
		ty = ponto.y - 1;
		if( ty < 0){
            i++;
            printf("Porta N invalida encontrada\n");
            curren = DeleteAllG("N", curren);
		}
		tx = ponto.y + 1;
		test = mapa->width;
		if( tx > test){
            i++;
            printf("Porta S invalida encontrada\n");
            curren = DeleteAllG("S", curren);
		}

	   //remove sala que vão causar conflito
        test = mapa->grid[ponto.x-1][ponto.y]->id;
        if(test != 0){

            if(((ponto.x-1) != x) && (ponto.y != y))
            {
                pch = (int)strspn("O", mapa->grid[x-1][y]->gene);
                if(pch > 0){
                    i++;
                    printf("Porta O Conflitante encontrada\n");
                    curren = DeleteOffG("L", curren);
                }
            }
        }

        test = mapa->grid[ponto.x+1][ponto.y]->id;
        if(test != 0){

            if(((ponto.x+1) != x) && (ponto.y != y)){
                pch = (int)strspn("L", mapa->grid[x+1][y]->gene);
                if(pch > 0){
                    i++;
                    printf("Porta L Conflitante encontrada\n");
                    curren = DeleteOffG("O", curren);
                }
            }
        }

        test = mapa->grid[ponto.x][ponto.y-1]->id;
        if(test != 0){

            if(((ponto.x) != x) && (ponto.y - 1 != y)){
                pch = (int)strspn("S", mapa->grid[x][y-1]->gene);
                if(pch > 0){
                    i++;
                    printf("Porta S Conflitante encontrada\n");
                    curren = DeleteOffG("N", curren);
                }
            }
        }

        test = mapa->grid[ponto.x][ponto.y+1]->id;
        if(test != 0){

            if(((ponto.x) != x) && (ponto.y+1 != y)){
                pch = (int)strspn("N", mapa->grid[x][y+1]->gene);
                if(pch > 0){
                    i++;
                    printf("Porta N Conflitante encontrada\n");
                    curren = DeleteOffG("S", curren);
                }
            }
        }

    if(i == 0)
        return head;

	return curren;

}

GPRoomList* DeleteEG(char *key, GPRoomList *head){ //Delete especific gene

   //start from the first link
    GPRoomList* curren = head;

   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list

   while( curren != NULL)
   {
      if( !strcmp(key,curren->room->gene)){
		curren = deleteid(curren->room->id, curren);
      }
      curren = curren->next;

   }
   return curren;
}

GPRoomList* DeleteAllG(char *key, GPRoomList *head){ // delete al intance the one gene

   //start from the first link
    GPRoomList* curren = head;
   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list
    while(curren != NULL)
   {
       int pch = (int)strspn(key, curren->room->gene);
       if(pch > 0){
    		curren = deleteid(curren->room->id, curren);
      }
      curren = curren->next;
   }
   return curren;
}

GPRoomList* DeleteOffG(char *key, GPRoomList *head){ // delete intances in case not encontre one type especifc gene

   //start from the first link
    GPRoomList* curren = head;
    //GGnode* previous = NULL;
   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list

  while(curren != NULL)
   {
       int pch = (int)strspn(key, curren->room->gene);
       if(pch > 0){
    		curren = deleteid(curren->room->id, curren);
      }
      curren = curren->next;
   }
   return curren;
}

int Sort_Rooom(GPRoomList *head){
    //GGnode* curren = head;
    GPRoomList* b;
    int globalchace= 0;
    int sorte =0;

    printf("Gerando chance global do conjunto\n");
    b = head;
    if(b == NULL){
        printf("b is NULL at Sort_Room\n");
        exit(1);
    }
    while(b != NULL)
    {
        printf("Adicionando chance: %d\n", b->chance);
        globalchace += b->chance;
        b = b->next;
    }

    sorte = PMrand() % globalchace;


    b = head;
    printf("Valor soteardo -- %d -- verificando sala correnspondente\n", sorte);
    while( b != NULL)
    {
        sorte -= b->chance;
        if (sorte <= 0){
            printf("Sala sorteada: %d\n", b->room->id);
            return b->room->id;
        }
        b = b->next;
    }
    printf("Nenhuma sala encontrada!");
    return 0;
}

int ocuppedspace(GPGenetic_Map *mapa){
    int i, j;
    int ocupped = 0;
    if(mapa->maximumContent == -1)
        mapa->maximumContent = mapa->height * mapa->width;
     if(mapa->minimalContent == -1)
        mapa->minimalContent = (mapa->height * mapa->width)/2;


    for (i = 0; i < mapa->height; i++)
        for (j = 0; j < mapa->width; j++)
            if(mapa->grid[i][j]->id != 0)
                ocupped += mapa->grid[i][j]->Tam;

    if(ocupped >= mapa->minimalContent)
        return 1;

    if(ocupped >= mapa->maximumContent)
        return 2;


    return 0;
}

GmPonto selectdoor(GMRoom *atual, int x, int y){
//	int i;
	GmPonto chosen;
	chosen.x = x;
	chosen.y = y;
	GMDoor *a;
//	a = atual->doors;  // Linha nunca usada, iniciado no FOR
    //printf("vou tentar ver, atual door");
    if(atual == NULL){
        printf("atual->doors is NULL at selectDoor\n");
        exit(0);
    }

	for(a = atual->doors; a != NULL; a = a->next){
       printf("ID para proxima sala: %d\n", a->idNextRoom);

        if(a->idNextRoom <= 0){            // DENIS, tava null, não se usa null pra int em C. Use um valor negativo
            if(!strcmp (a->gene,"N") || !strcmp (a->gene,"n")){
                strcpy(chosen.genes, "S");
                chosen.y--;
            }else if(!strcmp (a->gene,"S") || !strcmp (a->gene,"s")){
                strcpy(chosen.genes, "N");
                chosen.y++;
            }else if(!strcmp (a->gene,"L") || !strcmp (a->gene,"l")){
                strcpy(chosen.genes, "O");
                chosen.x++;
            }else if(!strcmp(a->gene,"O") || !strcmp (a->gene,"o")){
                strcpy(chosen.genes, "L");
                chosen.x--;
            }else{

                strcpy(chosen.genes, "I");
                printf("pequei nada\n");
			}

			return chosen;
        }

	}

    strcpy(chosen.genes, "I");
    printf("pequei nada\n");
	return chosen;
}

GMRoom *getRoom (GPGenetic_Map *map, int xPosition, int yPosition){
    GMRoom *tile = map->grid[xPosition][yPosition];
    return tile;
}
