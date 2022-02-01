#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sda.h>
#include "protocole.h"
extern int sock;
static void end(int sig)
{
	printf("\r\n<<<\n");
	exit(sig - SIGINT);
}
char *copy(char *original, size_t len)
{
	char *result = malloc(len + 1);
	strncpy(result, original, len);
	return result;
}
void put(const char *args)
{
	char *key = NULL, *value = NULL, *end = NULL;
	ssize_t keyLen, valueLen;
	if ((key = strchr(args, '\'')) && (end = strchr(key + 1, '\'')))
	{
		keyLen = end - key - 1;
		if ((value = strchr(end + 1, '\'')) && (end = strchr(value + 1, '\'')))
		{
			valueLen = end - value - 1;
			if (_put(sock,key + 1, keyLen, value + 1, valueLen))
				printf("Un probleme est survenue coté serveur\n");
			return;
		}
		printf("Aucune valeur fournie\n");
		return;
	}
	printf("Aucune clé fournie\n");
}
void get(const char *args)
{
	char *key = NULL, *end = NULL, *value = NULL;
	if ((key = strchr(args, '\'')) && (end = strchr(key + 1, '\'')))
	{
		ssize_t len = end - key + 1;
		if (value = _get(sock,key + 1, len - 2))
		{
			printf("%.*s:%s\n", len - 2, key + 1, value);
			return;
		}
		printf("clé non retrouver\n");
		return;
	}
	printf("Aucune clé fournie\n");
}
void delete (const char *args)
{
	char *key = NULL, *end = NULL;
	if ((key = strchr(args, '\'')) && (end = strchr(key + 1, '\'')))
	{
		ssize_t len = end - key + 1;
		if (_delete(sock,key + 1, len - 2))
			printf("clé non retrouver\n");
		return;
	}
	printf("Aucune clé fournie\n");
}
void help(const char *args)
{
	printf("Command disponible:\n"
		   "\thelp\n"
		   "\tget 'key'\n"
		   "\tput 'key' 'valeur'\n"
		   "\tdelete 'key'\n");
}
void commandHandler(const char *command)
{
	char *commands[] = {"get", "put", "delete", "end", "help"};
	int commands_len = 5, command_index = -1;

	for (int i = 0; i < commands_len; i++)
		if (strncmp(command, commands[i], strlen(commands[i])) == 0)
		{
			command_index = i;
			break;
		}
	switch (command_index)
	{
	case 0:
		get(command);
		break;
	case 1:
		put(command);
		break;
	case 2:
		delete(command);
		break;
	case 3:
		end(SIGINT);
		break;
	default:
		help(command);
		break;
	}
}
int shell()
{
	// init(socket);
	// signal(SIGINT, end);
	char command[256];
	while (true)
	{
		printf(">>> ");
		fgets(command, 256, stdin);
		commandHandler(command);
	}

	return 0;
}
