#if !defined(DBRUCHDANE)
#define DBRUCHDANE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sda.h>

struct json
{
	char *key;
	char *value;
};
struct json *put(char *key, char *value);
char *get(char *key);
int delete(char *key);

#endif // DBRUCHDANE