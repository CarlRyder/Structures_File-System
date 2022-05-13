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
	int count_nodes;
	int count_keys;
	struct FKey* keys[TREE_DEGREE - 1];
	struct TNode* childrens[TREE_DEGREE];
	struct TNode* parent;
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

/* Tree transformation functions */

void tree_init()
{
	// Creating the root of tree
	root = (Node*)malloc(sizeof(Node));
	if (root != NULL)
	{
		root->count_nodes = 0;
		root->count_keys = 0;
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
	if (directory->count_nodes == TREE_DEGREE)
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
		new_folder->count_keys = 0;
		new_folder->count_nodes = 0;
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
				directory->count_nodes++;
				break;
			}
		}
		return TRUE;
	}
	else
	{
		memory_error();
		return ERROR;
	}
}

int key_push(Node* directory, char* fname)
{
	// Checking if the limit of created folders in the directory is exceeded
	if (directory->count_keys == TREE_DEGREE - 1)
	{
		printf("touch: Exceeded the limit on the number of files in the directory \"%s\"\n", directory->name);
		return ERROR;
	}
	// Checking for the existence of a file with the same name
	for (int i = 0; i < TREE_DEGREE - 1; i++)
	{
		if (directory->keys[i] == NULL) break;
		char* file_name = directory->keys[i]->name;
		if (strcmp(file_name, fname) == 0)
		{
			printf("touch: Error creating the file \"%s\". A file with that name already exists.\n", fname);
			return ERROR;
		}
	}
	// Creating new file
	Key* new_file = (Key*)malloc(sizeof(Key));
	if (new_file != NULL)
	{
		new_file->directory = directory;
		strcpy(new_file->creation_date, get_date_now());
		strcpy(new_file->name, fname);
		// Changing current directory
		for (int i = 0; i < TREE_DEGREE - 1; i++)
		{
			if (directory->keys[i] == NULL)
			{
				directory->keys[i] = new_file;
				directory->count_keys++;
				break;
			}
		}
		return TRUE;
	}
	else
	{
		memory_error();
		return ERROR;
	}
}

void node_delete(Node* directory)
{
	// Deleting all files in this directory
	for (int i = 0; i < TREE_DEGREE - 1; i++)
	{
		if (directory->keys[i] == NULL) break;
		free(directory->keys[i]);
		directory->keys[i] = NULL;
	}
	// Deleting all folders in this directory
	for (int i = 0; i < TREE_DEGREE; i++)
	{
		if (directory->childrens[i] == NULL) break;
		node_delete(directory->childrens[i]);
	}
	// Removing this directory from the tree
	for (int i = 0; i < TREE_DEGREE; i++)
	{
		if ((directory->parent)->childrens[i] == directory)
		{
			free((directory->parent)->childrens[i]);
			(directory->parent)->childrens[i] = NULL;
			break;
		}
	}
}

void key_delete(Node* directory, int key_number)
{
	for (int i = 0; i < TREE_DEGREE - 1; i++)
	{
		if (directory->keys[i] == NULL) break;
		if (i >= key_number)
		{
			// If this is the first file in the directory, then first just delete it
			if (i == 0)
			{
				free(directory->keys[i]);
				directory->keys[i] == NULL;
			}
			else
			{
				directory->keys[i - 1] = directory->keys[i];
				free(directory->keys[i]);
				directory->keys[i] == NULL;
			}
		}
	}
}

int item_delete(Node* directory, char* name, int flag)
{
	// Delete the file
	for (int i = 0; i < TREE_DEGREE - 1; i++)
	{
		if (directory->keys[i] == NULL) break;
		char* file_name = directory->keys[i]->name;
		if (strcmp(file_name, name) == 0)
		{
			key_delete(directory, i);
			return TRUE;
		}
	}
	for (int i = 0; i < TREE_DEGREE; i++)
	{
		if (directory->childrens[i] == NULL) break;
		char* directory_name = directory->childrens[i]->name;
		if (strcmp(directory_name, name) == 0)
		{
			if (flag == 1)
			{
				node_delete(directory);
				return TRUE;
			}
			else
			{
				printf("rm: Deletion \"%s\" is not possible. This object is a folder.", name);
				return ERROR;
			}
		}
	}
	printf("rm: \"%s\" cannot be deleted. This object is missing in the current directory.", name);
}

int item_find(Node* directory, char* name)
{

}

int items_print(int flag)
{
	if (flag == 1)
	{
		// Output of elements using the ls command with the - l flag (Output of all attributes)
		for (int i = 0; i < TREE_DEGREE - 1; i++)
		{
			if (directory_now->keys[i] == NULL) break;
			printf("%s %s %s\n", "Professional", directory_now->keys[i]->creation_date, directory_now->keys[i]->name);
		}
		for (int i = 0; i < TREE_DEGREE; i++)
		{
			if (directory_now->childrens[i] == NULL) break;
			int count_items = directory_now->childrens[i]->count_keys + directory_now->childrens[i]->count_nodes;
			printf("%s %s %d %s\n", "Professional", directory_now->childrens[i]->creation_date, count_items, directory_now->childrens[i]->name);
		}
	}
	else
	{
		// Output of elements using the ls command without a flag
		for (int i = 0; i < TREE_DEGREE - 1; i++)
		{
			if (directory_now->keys[i] == NULL) break;
			printf("%s ", directory_now->keys[i]->name);
		}
		for (int i = 0; i < TREE_DEGREE; i++)
		{
			if (directory_now->childrens[i] == NULL) break;
			printf("%s ", directory_now->childrens[i]->name);
		}
		if (directory_now->childrens[0] != NULL || directory_now->keys[0] != NULL) printf(" \n");
	}
}

/* ------------------------------ */

/*  Command processing functions  */

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

void command_cd(char* str)
{

}

void command_ls(char* str)
{

}

void command_rm(char* str)
{

}

void command_mkdir(char* str)
{

}

void command_touch(char* str)
{

}

void command_find(char* str)
{

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
		else if (num == 1) command_cd(input);
		else if (num == 2) command_ls(input);
		else if (num == 3) command_rm(input);
		else if (num == 4) command_mkdir(input);
		else if (num == 5) command_touch(input);
		else if (num == 6) command_find(input);
		else printf("%s: command not found\n\n", input);
	}
}

/* ------------------------------ */

int main()
{
	printf("File System \"Alpha\"\n"
		"Copyright (c) 2022 Saint-Petersburg Polytechnic University, Russia\n");
	tree_init();
	inputs();
}