#include "protocole.h"
// int sock;
// void init(int _socket){
// 	sock = _socket;
// }
//CLient
char* _get(int sock, char* key, size_t len)
{
	char* result;
	int option = 0;
	networkWrite(sock, &option, sizeof(option));
	networkWrite(sock, &len, sizeof(len));
	networkWrite(sock, key, len);
	networkRead(sock, &len, sizeof(len));
	if (len == -1)
		return NULL;
	result = malloc(len + 1);
	networkRead(sock, result, len);
	result[len] = '\0';
	return result;
}

int _delete(int sock, char* key, size_t len)
{
	int option = 1;
	ssize_t result = 0;
	read(sock, &result, sizeof(result));
	if (result) {
		if (result == -1)
			printf("Write request refused\n");
		return 0;
	}
	networkWrite(sock, &option, sizeof(option));
	networkWrite(sock, &len, sizeof(len));
	networkWrite(sock, key, len);
	networkRead(sock, &result, sizeof(result));
	return result != 0;
}
int _put(int sock, char* key, size_t keyLen, char* value, size_t valueLen)
{
	int option = 2;
	ssize_t result = 0;
	networkWrite(sock, &option, sizeof(option));
	read(sock, &result, sizeof(result));
	if (result) {
		if (result == -1)
			printf("Write request refused\n");
		return 0;
	}
	networkWrite(sock, &keyLen, sizeof(keyLen));
	networkWrite(sock, key, keyLen);
	networkWrite(sock, &valueLen, sizeof(valueLen));
	networkWrite(sock, value, valueLen);
	networkRead(sock, &result, sizeof(result));
	return result != 0;
}

void _writeRequestRefused(int sock) {
	ssize_t result = -1;
	write(sock, &result, sizeof(result));
}

void _writeRequestAccepeted(int sock) {
	ssize_t result = 0;
	write(sock, &result, sizeof(result));
}

//Master get 
ssize_t __get(int sock, char* (*get)(char*))
{
	ssize_t len, result;
	char* key, * value;
	result = read(sock, &len, sizeof(len));
	key = malloc(len + 1);
	result = read(sock, key, len);
	key[len] = '\0';
	printf("Execution de get '%s'\n", key);
	key[len] = '\0';
	value = get(key);
	len = value ? strlen(value) : -1;
	result = write(sock, &len, sizeof(len));
	if (len != -1)
		write(sock, value, len);
	free(key);
	return result;
}

char* __delete(int sock, int(*delete)(char*))
{
	ssize_t len, result;
	char* key;
	result = read(sock, &len, sizeof(len));
	key = malloc(len + 1);
	result = read(sock, key, len);
	key[len] = '\0';
	printf("Execution de delete '%s'\n", key);
	result = delete(key);
	result = write(sock, &result, sizeof(result));
	return key;
}

struct json* __put(int sock, struct json* (*put)(char*, char*))
{
	struct json* element;
	ssize_t len, result;
	char* key, * value;
	result = read(sock, &len, sizeof(len));
	key = malloc(len + 1);
	result = read(sock, key, len);
	key[len] = '\0';
	result = read(sock, &len, sizeof(len));
	value = malloc(len + 1);
	result = read(sock, value, len);
	value[len] = '\0';
	printf("Execution de put '%s' '%s' \n", key, value);
	element = put(key, value);
	result = (element == NULL);
	result = write(sock, &result, sizeof(result));
	return element;
}