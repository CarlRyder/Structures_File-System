// Made by Y. Sendov. May 2022

#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT_EXIT_ERROR -1
#define TREE_DEGREE 30
#define CREATION_DATE_LEN 25
#define NAME_MAX_LEN 30
#define EXPAN_MAX_LEN 10
#define INPUT_MAX_LEN 256
#define WAY_MAX_LEN 2048
#define ERROR 0
#define	TRUE 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct TNode
{
	int num_nodes;
	int num_keys;
	Key* keys[TREE_DEGREE - 1];
	Node* childrens[TREE_DEGREE];
	Node* parent;
	char creation_date[CREATION_DATE_LEN];
	char name[NAME_MAX_LEN];
} Node;

typedef struct FKey
{
	char name[NAME_MAX_LEN + EXPAN_MAX_LEN + 1];
	char creation_date[CREATION_DATE_LEN];
	Node* directory;
} Key;

// Initialization of required global variables
Node* root = NULL;
Node* directory_now = NULL;
char way_now[WAY_MAX_LEN] = { 0 };

char* get_date_now()
{
	time_t time_now = time(NULL);
	char* date = ctime(&time_now);
	return date;
}

void memory_error()
{
	printf("The memory for the tree was not allocated.\n"
		"Emergency shutdown with error: %d\n", DEFAULT_EXIT_ERROR);
	exit(DEFAULT_EXIT_ERROR);
}

void tree_init()
{
	root = (Node*)malloc(sizeof(Node));
	if (root != NULL)
	{
		root->num_nodes = 0;
		root->num_keys = 0;
		root->parent = NULL;
		for (int i = 0; i < TREE_DEGREE; i++) root->childrens[i] = NULL;
		strcpy(root->name, "/");
		strcpy(root->creation_date, get_date_now());
		way_now[0] = '/';
		directory_now = root;
	}
	else memory_error();
}

int node_push(Node* directory, char* dname)
{
	// Checking if the limit of created folders in the directory is exceeded
	if (directory->num_nodes == TREE_DEGREE - 1)
	{
		printf("mkdir: Exceeded the limit on the number of folders in the directory \"%s\"\n", directory->name);
		return ERROR;
	}
	// Checking for the existence of a folder with the same name
	for (int i = 0; i < TREE_DEGREE; i++)
	{
		if (directory->childrens[i] == NULL) break;
		char* directory_name = directory->childrens[i]->name;
		if (strcmp(directory_name, dname) == 0)
		{
			printf("mkdir: Error creating the directory \"%s\". A folder with that name already exists.\n", dname);
			return ERROR;
		}
	}
	// Creating new folder
	Node* new_folder = (Node*)malloc(sizeof(Node));
	if (new_folder != NULL)
	{
		new_folder->parent = directory;
		new_folder->num_keys = 0;
		new_folder->num_nodes = 0;
		for (int i = 0; i < TREE_DEGREE; i++) new_folder->childrens[i] = NULL;
		for (int i = 0; i < TREE_DEGREE - 1; i++) new_folder->keys[i] = NULL;
		strcpy(new_folder->name, dname);
		strcpy(new_folder->creation_date, get_date_now());
		// Changing current directory
		for (int i = 0; i < TREE_DEGREE; i++)
		{
			if (directory->childrens[i] == NULL)
			{
				directory->childrens[i] = new_folder;
				directory->num_nodes++;
				break;
			}
		}
	}
	else memory_error();
}

int read_command(char* str)
{
	char commands[7][6] = { "exit", "cd", "ls", "rm", "mkdir", "touch", "find" };
	char command[6] = { 0 };
	int mark = -1;
	for (int i = 0; i < 5; i++)
	{
		if (str[i] == ' ') break;
		command[i] = str[i];
	}
	command[strlen(command)] = '\0';
	for (int j = 0; j < 7; j++)
	{
		if (strcmp(command, commands[j]) == 0)
		{
			mark = j;
			break;
		}
	}
	return mark;
}

void inputs()
{
	bool flag = false;
	while (true)
	{
		if (flag == true) break;
		printf("%s > ", way_now);
		char input[INPUT_MAX_LEN];
		fgets(input, INPUT_MAX_LEN, stdin);
		input[strcspn(input, "\n")] = 0;
		fseek(stdin, 0, SEEK_END);
		int num = read_command(input);
		if (num == 0) flag = true; // exit
		else if (num == 1) // cd
		{
			
		}
		else if (num == 2) // ls
		{

		}
		else if (num == 3) // rm
		{

		}
		else if (num == 4) // mkdir
		{
			
		}
		else if (num == 5) // touch
		{

		}
		else if (num == 6) // find
		{

		}
		else printf("%s: command not found\n\n", input);
	}
}

int main()
{
	printf("File System \"Alpha\"\n"
		"Copyright (c) 2022 Saint-Petersburg Polytechnic University, Russia\n");
	tree_init();
	inputs();
}