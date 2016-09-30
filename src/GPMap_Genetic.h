#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "GPMap.h"
#include "json.h" // https://github.com/udp/json-parser



typedef struct _node
{
   GMRoom sala;
   struct node *next;
}node;


typedef struct _map
{
    char *name;
	char *coreFilePath;
	char *conditionFilePath;
	char *contentPathSet;
	int height;
	int width;
    int maximumContent;
    int minimalContent;
    GMRoom** grid;
}GPGenetic_Map

void printList()
void insertFirst(GMRoom *key, node *alvo)
struct node* deleteFirst()
int isEmpty()
int length()


struct node* find(int key){
struct node* GerateListPosibylit(char *gene, node *alpha)
struct node* addchance(int key, int chance)
struct node* SeachCopatibility(char *gene, node *head)
struct node* SeachCopatibilityid(int salaid, node *head)
struct node* deleteid(int key, node *head)
struct node* SeachIncopatibility(char *gene, node *head)
struct node* SeachIncopatibilityid(int salaid, node *head)
struct node* DeleteEG(char *key, node *head)
struct node* DeleteAllG(char *key, node *head)
struct node* DeleteOffG(char *key, node *head)

