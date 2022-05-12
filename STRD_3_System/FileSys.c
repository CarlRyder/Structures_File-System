// Made by Y. Sendov. May 2022

#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT_EXIT_ERROR -1
#define DEGREE 30
#define CREATION_DATE_LEN 25
#define NAME_MAX_LEN 30
#define EXPAN_MAX_LEN 10
#define INPUT_MAX_LEN 256
#define WAY_MAX_LEN 2048

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct TNode
{
	int num_nodes;
	int num_keys;
	struct Key* keys[DEGREE - 1];
	struct Node* childrens[DEGREE];
	struct Node* parent;
	char creation_date[CREATION_DATE_LEN];
	char name[NAME_MAX_LEN];
} Node;

typedef struct FKey
{
	char name[NAME_MAX_LEN + EXPAN_MAX_LEN + 1];
	char creation_date[CREATION_DATE_LEN];
	Node* directory;
} Key;

// Инициализация необходимых глобальных переменных
Node* root = NULL;
Node* directory_now = NULL;
char* way_now = NULL;

char* get_date_now()
{
	time_t time_now = time(NULL);
	char* date = ctime(&time_now);
	return date;
}

void tree_init()
{
	root = (Node*)malloc(sizeof(Node));
	if (root != NULL)
	{
		root->num_nodes = 0;
		root->num_keys = 0;
		root->parent = NULL;
		for (int i = 0; i < DEGREE; i++) root->childrens[i] = NULL;
		strcpy(root->name, "/");
		strcpy(root->creation_date, get_date_now());
		char* temp = (char*)realloc(way_now, WAY_MAX_LEN * sizeof(char));
		if (temp != NULL) way_now = temp;
		directory_now = root;
	}
	else
	{
		printf("The memory for the tree was not allocated.\n"
			"Emergency shutdown with error: %d\n", DEFAULT_EXIT_ERROR);
		exit(DEFAULT_EXIT_ERROR);
	}
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

void cd_command(char* str)
{

}

void begin()
{
	int flag = 0;
	while (1)
	{
		if (flag == 1) break;
		printf("> ");
		char input[INPUT_MAX_LEN];
		fgets(input, INPUT_MAX_LEN, stdin);
		input[strcspn(input, "\n")] = 0;
		fseek(stdin, 0, SEEK_END);
		int num = read_command(input);
		if (num == 0) flag = 1; // exit
		else if (num == 1) // cd
		{
			cd_command(input);
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
	begin();
}