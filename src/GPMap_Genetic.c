#include "GPMap_Genetic.h"

GGnode *head;
GGnode *current;
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

            GMTileType *a = content1->first;
            GMTileType *b = malloc(sizeof(GMTileType));
            b->next = a;
            content1->first = b;
        }
        if(generatingRoom){
            GMRoom *a = content->first;
            GMRoom *b = malloc(sizeof(GMRoom));
            b->next = a;
            content->first = b;
        }
        else if(generatingScatter){
            GMTileType *tile = content1->first;
            GMScatterChance *a = tile->scatter;
            GMScatterChance *b = malloc(sizeof(GMScatterChance));
            b->next = a;
            tile->scatter = b;
        }
        if(generatingDoor){
            GMDoor *a = content->first->doors;
            GMDoor *b = malloc(sizeof(GMDoor));
            b->next = a;
            content->first->doors = b;
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
                generatingRoom = 0;
                process_content_array(value, depth+1);
                generatingTile = 0;
                generatingRoom = 0;
            }
            if(!strcmp (name,"ScatterRules")){
                generatingScatter = 1;
                generatingTile = 0;

                process_content_array(value, depth+1);

                generatingScatter = 0;
                generatingTile = 1;
            }
            if(!strcmp (name,"Rooms")){
                generatingRoom = 1;
                generatingTile = 0;
                process_content_array(value, depth+1);
                generatingTile = 0;
                generatingRoom = 0;

            }
            if(!strcmp (name,"Map")){
                generatingMap = 1;
                generatingDoor = 0;

                generatingRoom = 0;
                generatingScatter = 0;
                generatingTile = 0;

                process_content_array(value, depth+1);

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

                process_content_array(value, depth+1);

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

                process_content_array(value, depth+1);

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

				process_content_array(value, depth+1);

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

                process_content_array(value, depth+1);

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
                    t->idNextdoor = -1;

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
                    t->name = getCopyFromString(value->u.string.ptr);
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


    map = malloc(sizeof(map));

    map->height = mapa->height;
    map->width = mapa->width;
    map->name =mapa->name  ;

    map->conditionFilePath =mapa->conditionFilePath;
    map->contentPathSet =mapa->contentPathSet;
    map->coreFilePath = mapa->coreFilePath;

    map->maximumContent = mapa->maximumContent;
    map->minimalContent =mapa->minimalContent;



    map->grid = (GMRoom***)malloc(sizeof(GMRoom**)*map->width);

    int i, j;
   // printf("%d\n", map->width);
    for(i = 0; i < map->height; i ++){
        map->grid[i] = (GMRoom**)malloc(sizeof(GMRoom*)*map->height);

        for(j = 0; j < map->height; j ++){

            GMRoom *tiler = malloc(sizeof(GMRoom));
            tiler->id = 0;
            tiler->name = "name";
            map->grid[i][j] = tiler;

        }
    }



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
    if ( fread(file_contents, file_size, 1, contentFile) != 0 ) {
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

    printf("starting to tileB: %d %d\n", map->width, map->height);

    if (value == NULL) {
        fprintf(stderr, "Unable to parse data in %s; JSON structure is probably not valid. Try dubbuging at http://jsonlint.com \n", contentFilePath);
        free(file_contents);
        exit(1);
    }
    content = malloc(sizeof(GMRoomContent));
    content1 = malloc(sizeof(GMAutomataContent));
    current = malloc(sizeof(GGnode));
    head = malloc(sizeof(GGnode));
//    GMTileType *a = malloc(sizeof(GMTileType));
//    content->first = a;
//    GMScatterChance *b = malloc(sizeof(GMScatterChance));
//    a->scatter = b;

    process_content_value(value, 0, NULL);

    json_value_free(value);
    free(file_contents);
//    printf("li os arquivos\n");


//    GMTileType *c = content->first;
//    //c->name = "hi!";
//
//    GMTileType *t = content->first->next;
//    GMScatterChance *s = content->first->scatter;
//    // Gera Mapa
//    printf("\n---Starting types printing---\n %s \n %d \n %s \n %d \n\n %d \n %d \n", t->name, t->id, t->imageDir, t->type, s->chance, s->id);

    // Processa Mapa


    head->sala = content->first;
    int startX = PMrand()%map->height;
    int starty = PMrand()%map->width;
    printf("Pontos inicial %d ---%d\n", startX,starty);
    x = startX;
	g.x = startX;

    y =  starty;
    g.y = starty;

    map->grid[startX][starty] = seach(8);
    //aqui funciona
   // system("cls");

   printList();
 //   system("pause");


    while (saida) {
        //aqui não funciona
        //valor errado usando a mesma condição
        printf("OI\n");
        g = selectdoor(map->grid[x][y], g.x, g.y);
       //printf("Olha passe vamo ver %s",g.genes);
		if(!!strcmp(g.genes,"I")){
            printf("indo fazer a lista 1\n");
            current = GerateListPosibylit(g.genes, head);
            printf("Fiz a lista 1\n Indo para lista 2\n");
            current =SeachIncopatibility(g.genes, current);
            printf("Fiz a lista 2\n Indo para a 3\n"); // ultima vez parou aqui
            current = SeachIncopatibilityid(map->grid[x][y]->id, current);
            printf("fiz a lista 3\nIndo para lista 4\n");
            current = SeachIncopatibilityExit(map->grid[x][y], current);
            printf("Fiz a lista 4\n comecando a deletar portas vazias\n");

            current = DeleteVoidDoors(map,g.x,g.y,current);
            printf("removidas portas vazia\n Adiconando compatibilidade\n");
            current =SeachCopatibility(map->grid[x][y],current);
            printf("Compartibilidades adicionadas\n Sorteando\n");
            map->grid[g.x][g.y] = seach(Sort_Rooom(current));
            printf("Fixado portas\n");
			fixdoor(map, x,y, g); //liga as portas de ambas as salas x y e px py, identificação da sala, g identificação da porta.
            printf("Sala e portas fixadas, reiniciando");
			x = g.x;
			y = g.y;
            int con;
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

			}

		}else{
			printf("Not found void next door.");
			int con;
			map->grid[x][y]->alldoorsocupped = 1;
			g = seachNextRoom(map, g); //Encontra a proxima sala que se pode continuar a geraççao de portas --- arrumar uma maneira de identificar sala de 2 portas

			if(g.x == -1){
				g = varremapapor(map,g);//função muito ruim
                if(g.x == -1){
                    con = ocuppedspace(map);
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
           GMDoor *a, *b;
           //inverte gene
           if(!strcmp (ponto.genes,"N") || !strcmp (ponto.genes,"n")){
                ponto.genes = "S";

            }else if(!strcmp (ponto.genes,"S") || !strcmp (ponto.genes,"s")){
                ponto.genes = "N";

            }else if(!strcmp (ponto.genes,"L") || !strcmp (ponto.genes,"l")){
                ponto.genes = "O";

            }else if(!strcmp (ponto.genes,"O") || !strcmp (ponto.genes,"o")){
                ponto.genes = "L";

            }else{
                ponto.genes = "I";
			}

           for(a = mapa->grid[x][y]->doors; a != NULL; a->next = a->next)
            {
                if(!strcmp (a->gene,ponto.genes)){
                    a->idNextRoom = mapa->grid[ponto.x][ponto.y]->id;
                    a->idNextRoomX = ponto.x;
                    a->idNextRoomY = ponto.y;
                }
            }
            //inverte gene
            if(!strcmp (ponto.genes,"N") || !strcmp (ponto.genes,"n")){
                ponto.genes = "S";

            }else if(!strcmp (ponto.genes,"S") || !strcmp (ponto.genes,"s")){
                ponto.genes = "N";

            }else if(!strcmp (ponto.genes,"L") || !strcmp (ponto.genes,"l")){
                ponto.genes = "O";

            }else if(!strcmp (ponto.genes,"O") || !strcmp (ponto.genes,"o")){
                ponto.genes = "L";

            }else{
                ponto.genes = "I";
			}

            for(b = mapa->grid[ponto.x][ponto.y]->doors; b != NULL; b->next = b->next)
            {
                if(!strcmp (a->gene,ponto.genes)){
                    b->idNextRoom = mapa->grid[x][y]->id;
                    b->idNextRoomX = x;
                    b->idNextRoomY = y;
                }
            }
}

GmPonto seachNextRoom(GPGenetic_Map *mapa, GmPonto ponto){
    int px = ponto.x;
    int py = ponto.y;
	GMRoom* a = mapa->grid[px][py];
	GMDoor* b;
	while(a->doors != NULL){
		if(mapa->grid[b->idNextRoomX][b->idNextRoomY]->alldoorsocupped != 1){
				ponto.x = b->idNextRoomX;
				ponto.y = b->idNextRoomY;
				printf("sala faltando conexão encontrada\n");
				return ponto;
		}
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
            tiler->name = "name";
            map->grid[i][j] = tiler;

        }
    }
    int startX = PMrand()%map->height;
    int starty = PMrand()%map->width;
    map->grid[startX][starty] = seach(8);

}

void printList()
{
   GMRoom *ptr = head->sala;
   printf("\n[ ");

   //start from the beginning
   while(ptr != NULL)
	{
      printf("(%d) ",ptr->id);
      ptr = ptr->next;

   }
   printf(" ]");
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

GMRoom *seach(int key){

   //start from the first link
   GMRoom* curren = head->sala;

   //if list is empty
   if(head == NULL)
	{
      return NULL;
   }

   //navigate through list
   while(curren->id != key){

      //if it is last GGnode
      if(curren->next == NULL){
         return NULL;
      }else {
         //go to next link
         curren = curren->next;
      }
   }
     //if data found, return the current Link
   return curren;
}

GGnode* GerateListPosibylit(char *gene, GGnode *alpha){ // alpha list global --- // lista para amarzenar as possibilidades usada somente na função

   //start from the first link
   GGnode *headi = alpha;
   GMRoom *c;
   GGnode* curren = malloc(sizeof(GGnode));

   //if list is empty
   int pch;
   if(headi == NULL)
   {
      return NULL;
   }
    int i = 0;


   //navigate through list
   while((NULL != headi->sala)){//what

         pch = (int)strspn(gene, headi->sala->gene);
         //printf("\n%s -- gene ---- %s\n %d resul do pch\n", headi->gene,gene, pch);
        c = headi->sala->next;


        //printf("\npassa merda %d", c->id);

         if(pch > 0 ){
            i++;

            GMRoom *a = malloc(sizeof(GMRoom));
           // GMRoom *b = malloc(sizeof(GMRoom));

            a->next = curren->sala;
            //printf("\nni %d", headi->id);
            a = headi->sala;
            //printf("\nnii \n");
            //printf("\nniii \n");

            //printf("\n oi so para comferir %d \n", a->id);
            curren->sala = a;
            //printf("\nniii \n");
         }
         //go to next link


        headi->sala = c;

   }

   printf("DesBuguei!  registors %d\n", i);
   //if found all possibilits return new list

   return curren;
}

GGnode* SeachCopatibility(GMRoom *atual, GGnode *head){
	int c = 0;
	GGnode* curren = head;

    GMRoom* a;
    GMRoom* b;
	for( b = head->sala; b != NULL; b = b->next)
    {
   		for(a = atual; a->compativeExit != NULL; a->compativeExit = a->compativeExit->next){
			if(strcmp(b->gene, a->compativeExit->genes) == 1){
				c = 1;
			}
			if(a->compativeExit->id == b->id){
				c = 1;
			}
		}
		if(c)
			b->chance += a->compativeExit->chance;
   }
   curren->sala = b;
   return curren;


}

GGnode* deleteid(int key, GGnode *head){

   //start from the first link
    GGnode* curren = head;
    GGnode* previous = NULL;

   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list
   while(curren->sala->id != key){

      //if it is last GGnode
      if(curren->sala->next == NULL){
         return NULL;
      }else {
         //store reference to current link
         previous = curren;
         //move to next link
         curren->sala = curren->sala->next;
      }

   }

   //found a match, update the link
   if(curren == head) {
      //change first to point to next link
      curren->sala = head->sala->next;
   }else {
      //bypass the current link
      previous->sala->next = curren->sala->next;
   }
   curren = previous;
   return curren;
}

GGnode*  SeachIncopatibility(char *gene, GGnode *head){
	int i,c = 0;
	GGnode* curren = malloc(sizeof(GGnode));
    GmCrossover* a;
    GMRoom* b;
  i = 0;
    printf("1\n");
	for(b = head->sala; b != NULL; b = b->next)
   {
           printf("2\n");

		for(a = b->incompativeEntrad; a != NULL; a = a->next){
			    printf("3\n");
			if(strspn(gene,a->genes)>=1){
				c = 1;
                i++;
			}
		}
		if(c)
			curren = deleteid(b->id, current);
   }

   if(i== 0)
    curren = head;


	return curren;

}

GGnode* SeachIncopatibilityid(int salaid, GGnode *head){
	int i,c;
	GGnode* curren;
    curren = head;
	GmCrossover* a =malloc(sizeof(GmCrossover));
	GMRoom* b = malloc(sizeof(GMRoom));
    printf("\nHello mundo infernal");
	for(b = curren->sala; b != NULL; b = b->next)
   {

		c = 0;
        //printf("\n Id da sala que tá dando merda: %d", b->id);
		for(a = curren->sala->incompativeEntrad; a != NULL; a = a->next){
            printf("\nEntrei mundo mais infernal");

			if( a->id == salaid){


				c = 1;
			}
		}
		printf("\nsubi um nivel\n");
		if(c)
			curren = deleteid(b->id, curren);
   }
	return curren;


}

 GGnode*  SeachIncopatibilityExit(GMRoom *atual, GGnode *head){
	int i,c = 0;
	GGnode* curren = NULL;
    GMRoom* a;
    GMRoom* b;
	for( b = head->sala; b != NULL; b = b->next)
   {
   		for(a = atual; a->incompative != NULL; a->incompative = a->incompative->next){
			if(strcmp(b->gene,a->incompative->genes) ==1){
				c = 1;
			}
			if(a->incompative->id == b->id){
				c = 1;
			}
		}
		if(c)
			curren = deleteid(b->id, curren);
   }
	return curren;

}

GGnode*  DeleteVoidDoors(GPGenetic_Map *mapa, int x, int y,GGnode *head){

	int tx, ty, pch;
	GGnode* curren = head;
    int test;

        //Remove salas que apontam para fora do mapa principal
        tx = x -1;
		if( tx < 0)
            curren = DeleteAllG("L", curren);

		tx = x + 1;
		test= mapa->height;
		if(tx>test)
            curren = DeleteAllG("O", curren);

		ty = y - 1;
		if( ty < 0)
            curren = DeleteAllG("N", curren);

		tx = y + 1;
		test = mapa->width;
		if( tx > test)
            curren = DeleteAllG("S", curren);


	   //remove sala que vão causar conflito
        test = mapa->grid[x-1][y]->id;
        if(test != 0){
            pch = (int)strspn("O", mapa->grid[x-1][y]->gene);
            if(pch > 0)
                curren = DeleteOffG("L", curren);
        }

        test = mapa->grid[x+1][y]->id;
        if(test != 0){
            pch = (int)strspn("L", mapa->grid[x+1][y]->gene);
            if(pch > 0)
                curren = DeleteOffG("O", curren);
        }

        test = mapa->grid[x][y-1]->id;
        if(test != 0){
            pch = (int)strspn("S", mapa->grid[x][y-1]->gene);
            if(pch > 0)
                curren = DeleteOffG("N", curren);
        }

        test = mapa->grid[x][y+1]->id;
        if(test != 0){
            pch = (int)strspn("N", mapa->grid[x][y+1]->gene);
            if(pch > 0)
                curren = DeleteOffG("S", curren);
        }




	return curren;

}

GGnode* DeleteEG(char *key, GGnode *head){ //Delete especific gene

   //start from the first link
    GGnode* curren = head;

   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list
    GMRoom *b;
   for(b = head->sala; b != NULL; b = b->next)
   {
      if( !strcmp(key,b->gene)){
		curren = deleteid(b->id, curren);
      }
   }
   return curren;
}

GGnode* DeleteAllG(char *key, GGnode *head){ // delete al intance the one gene

   //start from the first link
    GGnode* curren = head;
    GMRoom *b;
   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list

   for(b = head->sala; b != NULL; b = b->next)
   {
       int pch = (int)strspn(key, b->gene);
       if(pch > 0){
    		curren = deleteid(b->id, curren);
      }
   }
   return curren;
}

GGnode* DeleteOffG(char *key, GGnode *head){ // delete intances in case not encontre one type especifc gene

   //start from the first link
    GGnode* curren = head;
    GGnode* previous = NULL;
    GMRoom* b;
   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list

   for(b = head->sala; b != NULL; b = b->next)
   {
       int pch = (int)strspn(key, b->gene);
       if(pch > 0){
    		curren = deleteid(b->id, curren);
      }
   }
   return curren;
}

int Sort_Rooom(GGnode *head){
    GGnode* curren = head;
    GMRoom* b;
    int globalchace;
    int sorte;
    for(b = head->sala; b != NULL; b = b->next)
    {
        globalchace += b->chance;
    }
    sorte = PMrand() % globalchace;

    for(b = head->sala; b != NULL; b = b->next)
    {
        sorte -= b->chance;
        if (sorte <= 0)
            return b->id;
    }
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

    if(ocupped < mapa->minimalContent)
        return 1;

    if(ocupped >= mapa->maximumContent)
        return 2;


    return 0;
}

GmPonto selectdoor(GMRoom *atual, int x, int y){
	int i;
	GmPonto chosen;
	chosen.x = x;
	chosen.y = y;
	GMDoor *a;
	a = atual->doors;
    //printf("vou tentar ver, atual door");


	for(a = atual->doors; a != NULL; a = a->next){
       // printf("entrei %s\n", a->gene);
        if(a->idNextRoom == -1){            // DENIS, tava null, não se usa null pra int em C. Use um valor negativo
            if(!strcmp (a->gene,"N") || !strcmp (a->gene,"n")){
                chosen.genes = "S";
                chosen.y--;
            }else if(!strcmp (a->gene,"S") || !strcmp (a->gene,"s")){
                chosen.genes = "N";
                chosen.y++;
            }else if(!strcmp (a->gene,"L") || !strcmp (a->gene,"l")){
                chosen.genes = "O";
                chosen.x++;
            }else if(!strcmp (a->gene,"O") || !strcmp (a->gene,"o")){
                chosen.genes = "L";
                chosen.x--;
            }else{
                chosen.genes = "I";
                printf("pequei nada\n");
			}

			return chosen;
        }

	}
	chosen.genes = "I";
    printf("pequei nada\n");
	return chosen;
}

GMRoom *getRoom (GPGenetic_Map *map, int xPosition, int yPosition){
    GMRoom *tile = map->grid[xPosition][yPosition];
    return tile;
}
