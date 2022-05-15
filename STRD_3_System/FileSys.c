// Made by Y. Sendov. May 2022

/* Service defines */
#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT_EXIT_ERROR -1
#define TREE_DEGREE 30
#define CREATION_DATE_LEN 26
#define NAME_MAX_LEN 30
#define EXPAN_MAX_LEN 10
#define INPUT_MAX_LEN 256
#define FOLDER_NAME_MAX_LEN 128
#define WAY_MAX_LEN 2048
#define ERROR 0
#define	TRUE 1
#define USER "User"

/* Command defines */
#define CD 1
#define LS 2
#define RM_FLAG 3
#define RM_NOFLAG 0
#define MKDIR 4
#define TOUCH 5
#define FIND 6

#include <stdio.h>
#include <stdlib.h>
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

/* Global variables */ 
Node* root = NULL;
Node* directory_now = NULL;
char way_now[WAY_MAX_LEN] = { 0 };

char* get_date_now()
{
	time_t time_now = time(NULL);
	char* date = ctime(&time_now);
	date[strcspn(date, "\n")] = 0;
	return date;
}

int check_symbol(unsigned char symbol)
{
	if (symbol == '/' || symbol == '\\' || symbol == ':' || symbol == '|' || symbol == '?' || symbol == '"' || symbol == '<' || symbol == '>' || symbol == ' ') return 0;
	else return 1;
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
		for (int i = 0; i < TREE_DEGREE - 1; i++) root->keys[i] = NULL;
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
		printf("mkdir: exceeded the limit on the number of folders in the directory \"%s\"\n", directory->name);
		return ERROR;
	}
	// Checking for the existence of a folder with the same name
	for (int i = 0; i < TREE_DEGREE; i++)
	{
		if (directory->childrens[i] == NULL) break;
		char* directory_name = directory->childrens[i]->name;
		if (strcmp(directory_name, dname) == 0)
		{
			printf("mkdir: error creating the directory \"%s\". A folder with that name already exists.\n", dname);
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
		strcpy(new_folder->creation_date, get_date_now());
		strcpy(new_folder->name, dname);
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
		printf("touch: exceeded the limit on the number of files in the directory \"%s\"\n", directory->name);
		return ERROR;
	}
	// Checking for the existence of a file with the same name
	for (int i = 0; i < TREE_DEGREE - 1; i++)
	{
		if (directory->keys[i] == NULL) break;
		char* file_name = directory->keys[i]->name;
		if (strcmp(file_name, fname) == 0)
		{
			printf("touch: error creating the file \"%s\". A file with that name already exists.\n", fname);
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
		directory->keys[i] = NULL;
	}
	// Deleting all folders in this directory
	for (int i = 0; i < TREE_DEGREE; i++)
	{
		if (directory->childrens[i] == NULL) break;
		node_delete(directory->childrens[i]);
	}
	// Removing this directory from the tree
	if (directory->parent != NULL)
	{
		for (int i = 0; i < TREE_DEGREE; i++)
		{
			if ((directory->parent)->childrens[i] == directory)
			{
				(directory->parent)->childrens[i] = NULL;
				break;
			}
		}
	}
}

void key_delete(Node* directory, int key_number)
{
	for (int i = 0; i < TREE_DEGREE - 1; i++)
	{
		if (directory->keys[i] == NULL) break;
		if (i == key_number)
		{
			directory->keys[i] = NULL;
		}
		else if (i > key_number && i != 0)
		{
			directory->keys[i - 1] = directory->keys[i];
			directory->keys[i] = NULL;
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
			directory->count_keys--;
			return TRUE;
		}
	}
	int marker = 0;
	for (int i = 0; i < TREE_DEGREE; i++)
	{
		if (directory->childrens[i] == NULL) break;
		if (marker == 1 && i != 0) directory->childrens[i - 1] = directory->childrens[i];
		char* directory_name = directory->childrens[i]->name;
		if (strcmp(directory_name, name) == 0)
		{
			if (flag == 1)
			{
				node_delete(directory->childrens[i]);
				directory->count_nodes--;
				marker = 1;
			}
			else
			{
				printf("rm: deletion \"%s\" is not possible. This object is a folder.\n", name);
				return ERROR;
			}
		}
	}
	if (marker != 1) printf("rm: \"%s\" cannot be deleted. This object is missing in the current directory.\n", name);
	return ERROR;
}

int item_find(Node* directory, char* name)
{
	if (to_directory(name, FIND) == ERROR) return TRUE;
	for (int i = 0; i < TREE_DEGREE - 1; i++)
	{
		if (directory_now->keys[i] == NULL) break;
		if (directory_now == root) printf("%s%s\n", way_now, directory_now->keys[i]->name);
		else printf("%s/%s\n", way_now, directory_now->keys[i]->name);
	}
	for (int i = 0; i < TREE_DEGREE; i++)
	{
		if (directory_now->childrens[i] == NULL) break;
		item_find(directory_now->childrens[i], directory_now->childrens[i]->name);
	}
	if (directory_now != root)
	{
		directory_now = directory_now->parent;
		int count = strlen(way_now);
		for (int i = count - strlen(way_now) + 1; i < WAY_MAX_LEN; i++) way_now[i] = 0;
	}
	return TRUE;
}

void items_print(int flag)
{
	if (flag == 1)
	{
		// Output of elements using the ls command with the - l flag (Output of all attributes)
		for (int i = 0; i < TREE_DEGREE - 1; i++)
		{
			if (directory_now->keys[i] == NULL) break;
			printf("%s %s %s\n", USER, directory_now->keys[i]->creation_date, directory_now->keys[i]->name);
		}
		for (int i = 0; i < TREE_DEGREE; i++)
		{
			if (directory_now->childrens[i] == NULL) break;
			int count_items = directory_now->childrens[i]->count_keys + directory_now->childrens[i]->count_nodes;
			printf("%s %s %d %s\n", USER, directory_now->childrens[i]->creation_date, count_items, directory_now->childrens[i]->name);
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

int to_directory(char* name, int flag)
{
	// Processing the "cd" command
	if (strcmp(name, "\n") == 0 || strcmp(name, "\0") == 0)
	{
		directory_now = root;
		way_now[0] = '/';
		for (int i = 1; i < WAY_MAX_LEN; i++) way_now[i] = '\0';
		return TRUE;
	}
	// Processing the "cd .." command
	if (strcmp(name, "..") == 0)
	{
		if (directory_now == root) return TRUE;
		int count = strlen(way_now);
		for (int i = count - strlen(directory_now->name) - 1; i < count; i++) way_now[i] = '\0';
		directory_now = directory_now->parent;
		if (directory_now == root) way_now[0] = '/';
		return TRUE;
	}
	// Processing the "cd ." command
	if (strcmp(name, ".") == 0) return TRUE;
	// Relative path processing
	if (name[0] == '/')
	{
		int count = strlen(way_now);
		directory_now = root;
		way_now[0] = '/';
		for (int i = 1; i < count; i++) way_now[i] = '\0';
	}
	char* str = strtok(name, "/");
	if (str == NULL) return TRUE;
	int marker = TRUE;
	while (TRUE)
	{
		for (int i = 0; i < TREE_DEGREE; i++)
		{
			if (directory_now->childrens[i] == NULL)
			{
				// Processing the "find" command
				if (flag == FIND)
				{
					for (int j = 0; j < TREE_DEGREE - 1; j++)
					{
						if (directory_now->keys[j] == NULL) 
						{
							printf("find: this file does not exist\n");
							marker = ERROR;
							break;
						}
						if (directory_now->keys[j] != NULL && strcmp(directory_now->keys[j]->name, str) == 0)
						{
							if (directory_now == root) printf("%s%s\n", way_now, directory_now->keys[j]->name);
							else printf("%s/%s\n", way_now, directory_now->keys[j]->name);
							marker = TRUE;
							break;
						}
					}
				}
				// Processing the "mkdir" command
				if (flag == MKDIR) node_push(directory_now, str);
				else
				{
					marker = ERROR;
					break;
				}
			}
			if (directory_now->childrens[i] != NULL && strcmp(directory_now->childrens[i]->name, str) == 0)
			{
				if (directory_now != root) way_now[strlen(way_now)] = '/';
				int count = strlen(way_now);
				for (int j = 0; j < strlen(str); j++) way_now[j + count] = str[j];
				directory_now = directory_now->childrens[i];
				i = -1;
				str = strtok(NULL, "/");
				// Processing the delete command
				if (str == NULL)
				{
					if (flag == RM_FLAG) item_delete(directory_now->parent, directory_now->name, 1);
					if (flag == RM_NOFLAG) item_delete(directory_now->parent, directory_now->name, 0);
					return marker;
				}
			}
		}
		if (marker == ERROR) break;
	}
	if (flag == TOUCH)
	{
		if (strtok(NULL, "/") == NULL)
		{
			key_push(directory_now, str);
			marker = TRUE;
		}
	}
	if (str != NULL && (flag == RM_FLAG) || (flag == RM_NOFLAG))
	{
		item_delete(directory_now, str, 1);
	}
	return marker;
}

/* ------------------------------ */

/*  Command processing functions  */

int read_command(char* str)
{
	char commands[8][6] = { "exit", "cd", "ls", "rm", "mkdir", "touch", "find", "help"};
	char command[6] = { 0 };
	int mark = -1;
	for (int i = 0; i < 5; i++)
	{
		if (str[i] == ' ') break;
		command[i] = str[i];
	}
	command[strlen(command)] = '\0';
	for (int j = 0; j < 8; j++)
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
	// Reading an argument and checking it for correctness
	int i = 3, counter = 0;
	char argument[WAY_MAX_LEN] = { 0 };
	do
	{
		argument[counter] = str[i];
		if (counter + 1 == WAY_MAX_LEN) break;
		counter++;
		i++;
	} while (i < strlen(str));
	argument[strcspn(argument, "\n")] = '\0';
	if (to_directory(argument, CD) == ERROR) printf("cd: directory \"%s\" does not exist\n", argument);
}

void command_ls(char* str)
{
	int flag = -1;
	if (str[2] == 0) flag = 0;
	else
	{
		int i = 3, counter = 0;
		char argument[NAME_MAX_LEN] = { 0 };
		while (i < strlen(str))
		{
			if (str[2] != ' ')
			{
				printf("ls: incorrect argument\n");
				return;
			}
			argument[counter] = str[i];
			if (counter + 1 == NAME_MAX_LEN) break;
			counter++;
			i++;
		}
		argument[strlen(argument)] = '\0';
		if (strcmp(argument, "-l") == 0) flag = 1;
		else
		{
			printf("ls: incorrect argument\n");
			return;
		}
	}
	items_print(flag);
}

void command_rm(char* str)
{
	int i = 3, flag = 0, count = 0;
	char argument[FOLDER_NAME_MAX_LEN] = { 0 };
	while (str[i] != ' ' || i < strlen(str))
	{
		if (str[i] == '-' && str[i + 1] == 'r' && str[i + 2] == ' ')
		{
			flag = 1;
			count = i + 3;
			break;
		}
		if (i + 1 == strlen(str)) break;
		i++;
	}
	int temp = 0;
	if (flag == 1)
	{
		for (int i = count; i < strlen(str); i++)
		{
			argument[temp] = str[i];
			if (temp + 1 == FOLDER_NAME_MAX_LEN) break;
			temp++;
		}
	}
	else
	{
		temp = 0;
		for (int i = 3; i < strlen(str); i++)
		{
			argument[temp] = str[i];
			if (temp + 1 == FOLDER_NAME_MAX_LEN) break;
			temp++;
		}
	}
	argument[strlen(argument)] = '\0';
	if (strcmp(argument, "\0") == 0)
	{
		printf("rm: skipped argument\n");
		return;
	}
	Node* local_directory_now = directory_now;
	char local_way_now[WAY_MAX_LEN] = { 0 };
	strcpy(local_way_now, way_now);
	if (flag == 1) to_directory(argument, RM_FLAG);
	else to_directory(argument, RM_NOFLAG);
	directory_now = local_directory_now;
	memset(way_now, 0, WAY_MAX_LEN);
	strcpy(way_now, local_way_now);
}

void command_mkdir(char* str)
{
	// Reading an argument and checking it for correctness
	int i = 6, counter = 0;
	char argument[FOLDER_NAME_MAX_LEN] = { 0 };
	while (i < strlen(str))
	{
		if (str[5] != ' ' || check_symbol(str[i]) == 0)
		{
			printf("mkdir: incorrect argument\n");
			return;
		}
		argument[counter] = str[i];
		if (counter + 1 != FOLDER_NAME_MAX_LEN) counter++;
		i++;
	}
	argument[counter] = '\0';
	if (strcmp(argument, "\0") == 0) 
	{
		printf("mkdir: skipped argument\n");
		return;
	}
	// Checking for the existence of a file with the same name
	for (int j = 0; j < TREE_DEGREE - 1; j++)
	{
		if (directory_now->keys[j] == NULL) break;
		else if (strcmp(directory_now->keys[j]->name, argument) == 0)
		{
			printf("mkdir: error creating the folder \"%s\". A file with that name already exists.\n", argument);
			return;
		}
	}
	// Creating a new folder in the tree
	Node* local_directory_now = directory_now;
	char local_way_now[WAY_MAX_LEN] = { 0 };
	strcpy(local_way_now, way_now);
	to_directory(argument, MKDIR);
	directory_now = local_directory_now;
	memset(way_now, 0, WAY_MAX_LEN);
	strcpy(way_now, local_way_now);
}

void command_touch(char* str)
{
	// Reading an argument and checking it for correctness
	int i = 6, counter = 0;
	char argument[FOLDER_NAME_MAX_LEN] = { 0 };
	while (i < strlen(str))
	{
		if (str[5] != ' ' || check_symbol(str[i]) == 0)
		{
			printf("touch: incorrect argument\n");
			return;
		}
		argument[counter] = str[i];
		if (counter + 1 != FOLDER_NAME_MAX_LEN) counter++;
		i++;
	}
	argument[counter] = '\0';
	if (strcmp(argument, "\0") == 0)
	{
		printf("touch: skipped argument\n");
		return;
	}
	for (int i = 0; i < TREE_DEGREE; i++)
	{
		if (directory_now->childrens[i] == NULL) break;
		char* folder_name = directory_now->childrens[i]->name;
		if (strcmp(folder_name, argument) == 0)
		{
			printf("touch: error creating the file \"%s\". A folder with that name already exists.\n", folder_name);
			return ERROR;
		}
	}
	// Creating a new file in the tree
	Node* local_directory_now = directory_now;
	char local_way_now[WAY_MAX_LEN] = { 0 };
	strcpy(local_way_now, way_now);
	if (to_directory(argument, TOUCH) == ERROR)
	{
		printf("touch: error creating the file \"%s\"\n", argument);
	}
	directory_now = local_directory_now;
	memset(way_now, 0, WAY_MAX_LEN);
	strcpy(way_now, local_way_now);
}

void command_find(char* str)
{
	// Reading an argument and checking it for correctness
	int i = 5, counter = 0;
	char argument[WAY_MAX_LEN] = { 0 };
	while (i < strlen(str))
	{
		argument[counter] = str[i];
		if (counter + 1 != WAY_MAX_LEN) counter++;
		i++;
	}
	argument[counter] = '\0';
	Node* local_directory_now = directory_now;
	char local_way_now[WAY_MAX_LEN] = { 0 };
	strcpy(local_way_now, way_now);
	if (strcmp(argument, "\0") == 0)
	{
		item_find(root, root->name);
		directory_now = local_directory_now;
		memset(way_now, 0, WAY_MAX_LEN);
		strcpy(way_now, local_way_now);
		return;
	}
	item_find(directory_now, argument);
	directory_now = local_directory_now;
	memset(way_now, 0, WAY_MAX_LEN);
	strcpy(way_now, local_way_now);
}

void command_help(char* str)
{
	for (int i = 4; i < strlen(str); i++)
	{
		if (str[i] != 0)
		{
			printf("help: the argument of this function is not required\n");
			return;
		}
	}
	printf("Command list:\n"
		"1. cd <operator> <name> - following the specified path\n"
		"   <operator> can accept arguments: \" \" \".\" \"..\"\n"
		"   <name> can accept absolute and relative path to folder\n"
		"2. ls <operator> - viewing the current directory\n"
		"   <operator> can accept argument \"-l\" or be missing\n"
		"3. rm <operator> <name> - deleting files\n"
		"   <operator> can accept argument \"-r\" or be missing\n"
		"   <name> can accept the name of folder\n"
		"4. mkdir <name> - creating a folder\n"
		"   <name> can accept the name of folder without next symbols: ? | \\ / \" > <\n"
		"5. touch <name> - creating a file\n"
		"   <name> can accept the name of file without next symbols: ? | \\ / \" > <\n"
		"6. find <name> - search for files by the specified path\n"
		"   <name> can accept absolute and relative path to folder or file\n");
}

void inputs()
{
	int flag = ERROR;
	while (TRUE)
	{
		if (flag == TRUE) break;
		printf("\n%s > ", way_now);
		char input[INPUT_MAX_LEN];
		fgets(input, INPUT_MAX_LEN, stdin);
		input[strcspn(input, "\n")] = 0;
		fseek(stdin, 0, SEEK_END);
		int num = read_command(input);
		if (num == 0) flag = TRUE;
		else if (num == 1) command_cd(input);
		else if (num == 2) command_ls(input);
		else if (num == 3) command_rm(input);
		else if (num == 4) command_mkdir(input);
		else if (num == 5) command_touch(input);
		else if (num == 6) command_find(input);
		else if (num == 7) command_help(input);
		else printf("%s: command not found\n", input);
	}
}

/* ------------------------------ */

int main()
{
	printf("File System \"Pre-Release\"\n"
		"Copyright (c) 2022 Saint-Petersburg Polytechnic University, Russia\n");
	tree_init();
	inputs();
}