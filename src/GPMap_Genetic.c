#include "GPMap.h"
#include "GPMap_Genetic.h"
#include <string.h>


node *head = NULL;
node *current = NULL;

void printList()
{
   node *ptr = head;
   printf("\n[ ");

   //start from the beginning
   while(ptr != NULL)
	{
      printf("(%d) ",ptr->sala.id);
      ptr = ptr->next;
   }

   printf(" ]");
}
void insertFirst(GMRoom *key, node *alvo)
{
   //create a link
   node *link = ( node*) malloc(sizeof( node));

   link->sala.id = key;

   //point it to old first node
   link->next = alvo;

   //point first to new first node
   alvo = link;
}

struct node* deleteFirst()
{

   //save reference to first link
   struct node *tempLink = head;

   //mark next to first link as first
   head = head->next;

   //return the deleted link
   return tempLink;
}

int isEmpty()
{
   return head == -1;
}

int length()
{
   int length = 0;
   node *current;

   for(current = head; current != NULL; current = current->next)
	{
      length++;
   }

   return length;
}

struct node* find(int key){

   //start from the first link
   node* current = head;

   //if list is empty
   if(head == NULL)
	{
      return NULL;
   }

   //navigate through list
   while(current->sala.id != key){

      //if it is last node
      if(current->next == NULL){
         return NULL;
      }else {
         //go to next link
         current = current->next;
      }
   }

   //if data found, return the current Link
   return current;
}
/*
int findgene(char* genes, char gene){
   cont = 0;
   for(i=0, vet[i] != '\ 0';i++){
     if(vet[i] == gene){
          cont++;
      }
   }

   return 0;
}*/

struct node* GerateListPosibylit(char *gene, node *alpha){ // alpha list global --- // lista para amarzenar as possibilidades usada somente na função

   //start from the first link
   node* current = alpha;

   //if list is empty
   if(head == NULL)
   {
      return NULL;
   }

   //navigate through list
   while(current->next != NULL){

      //if it is last node
      if(current->next == NULL){

         int pch = strchr(gene, current->sala.gene);
         if(pch != NULL)
            insertFirst(&current->sala, current);


      }else {
         //go to next link
         //find gene
         int pch = strchr(current->sala.gene, current);
         if(pch != NULL)
            insertFirst(&current->sala, current);

         current = current->next;
      }
   }

   //if found all possibilits return new list
   return current;
}

struct node* addchance(int key, int chance){

   //start from the first link
   node* current = head;

   //if list is empty
   if(head == NULL)
	{
      return NULL;
   }

   //navigate through list
   while(current->sala.id != key){

      //if it is last node
      if(current->next == NULL){
         return NULL;
      }else {
         //go to next link
         current = current->next;
      }
   }
    current->sala.chance += chance;
   //if data found, return the current Link
   return current;
}

struct node*  SeachCopatibility(char *gene, node *head){
	int i,c;
	node* current = head;

	for(current = head; current != NULL; current = current->next)
   {
		for( i = 0; i < sizeof(current->sala.compativeExit); i++){
			if(strcmp(gene,current->sala.compativeExit[i].genes) ==1){
				c = 1;
			}
		}
		if(c)
			current = addchance(current->sala.id, current);
   }
	return current;

}

struct node* SeachCopatibilityid(int salaid, node *head){
	int i,c;
	node* current = head;
	for(current = head; current != NULL; current = current->next)
   {
		c = 0;
		for( i = 0; i <  sizeof(current->sala.compativeExit); i++){
			if(current->sala.compativeExit[i].id == salaid){
				c = 1;
			}
		}
		if(c)
			current = addchance(current->sala.id, current);
   }
	return current;


}

struct node* deleteid(int key, node *head){

   //start from the first link
    node* current = head;
    node* previous = NULL;

   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list
   while(current->sala.id != key){

      //if it is last node
      if(current->next == NULL){
         return NULL;
      }else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }

   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   }else {
      //bypass the current link
      previous->next = current->next;
   }

   return current;
}



struct node*  SeachIncopatibility(char *gene, node *head){
	int i,c;
	node* current;

	for(current = head; current != NULL; current = current->next)
   {
		for( i = 0; i < sizeof(current->sala.incompativeEntrad); i++){
			if(strcmp(gene,current->sala.incompativeEntrad[i].genes) ==1){
				c = 1;
			}
		}
		if(c)
			current = deleteid(current->sala.id, current);
   }
	return current;

}

struct node* SeachIncopatibilityid(int salaid, node *head){
	int i,c;
	node* current = head;
	for(current = head; current != NULL; current = current->next)
   {
		c = 0;
		for( i = 0; i <  sizeof(current->sala.incompativeEntrad); i++){
			if(current->sala.incompativeEntrad[i].id == salaid){
				c = 1;
			}
		}
		if(c)
			current = deleteid(head->sala.id, current);
   }
	return current;


}

struct node*  DeleteVoidDoors(GPGenetic_Map **mapa, int x, int y,node *head){

	int tx, ty, pch;
	node* current;
    room test;

        //Remove salas que apontam para fora do mapa principal
        tx = x -1;
		if( tx < 0)
            current = DeleteAllG("L", current);
		tx = x + 1;
		if( tx > mapa.height);
            current = DeleteAllG("O", current);
		ty = y - 1;
		if( ty < 0)
            current = DeleteAllG("N", current);
		tx = y + 1;
		if( tx > mapa.width);
            current = DeleteAllG("S", current);
	   //remove sala que vão causar conflito
        test = mapa[x-1][y];
        if(test != NULL){
            pch = strchr("O", current->sala.gene);
            if(pch == NULL)
                current = DeleteOffG("L", current);
                test = mapa[x-1][y];
        }

        test = mapa[x+1][y];
        if(test != NULL){
            pch = strchr("L", test.gene);
            if(pch == NULL)
                current = DeleteOffG("O", current);
        }

        test = mapa[x][y-1];
        if(test != NULL){
            pch = strchr("S", current->sala.gene);
            if(pch == NULL)
                current = DeleteOffG("N", current);    test = mapa[x-1][y];
        }

        test = mapa[x][y+1];
        if(test != NULL){
            pch = strchr("N", current->sala.gene);
            if(pch == NULL)
                current = DeleteOffG("S", current);
        }




	return current;

}


struct node* DeleteEG(char *key, node *head){

   //start from the first link
    node* current = head;
    node* previous = NULL;

   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list

   for(current = head; current != NULL; current = current->next)
   {
      if(strcmp(key,current->sala.gene) ==1){
		current = deleteid(current->sala.id, current);
      }
   }
   return current;
}

struct node* DeleteAllG(char *key, node *head){

   //start from the first link
    node* current = head;
    node* previous = NULL;

   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list

   for(current = head; current != NULL; current = current->next)
   {
       int pch = strchr(gene, current->sala.gene);
       if(pch != NULL){
    		current = deleteid(current->sala.id, current);
      }
   }
   return current;
}

struct node* DeleteOffG(char *key, node *head){

   //start from the first link
    node* current = head;
    node* previous = NULL;

   //if list is empty
   if(head == NULL){
      return NULL;
   }

   //navigate through list

   for(current = head; current != NULL; current = current->next)
   {
       int pch = strchr(gene, current->sala.gene);
       if(pch == NULL){
    		current = deleteid(current->sala.id, current);
      }
   }
   return current;
}
