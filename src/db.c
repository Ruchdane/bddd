#include "db.h"

struct element *store = NULL;

struct json *put(char *key, char *value)
{
	struct element *tmp;
	struct json *element;
	foreach (tmp, store)
	{
		element = (struct json *)tmp->value;
		if (strcmp(key, element->key) == 0){
			// free(element->value);
			element->value = value;
		}
	}
	element = malloc(sizeof(*element));
	element->key = key;
	element->value = value;
	store = append(store, element);
	return element;
}
char *get(char *key)
{
	struct element *tmp;
	struct json *element;
	foreach (tmp, store)
	{
		element = (struct json *)tmp->value;
		if (strcmp(key, element->key) == 0)
			return element->value;
	}
	return NULL;
}

int delete(char *key)
{
	struct element *cur,*prev,*tmp;
	struct json *element;
	element = (struct json *)store->value;
	if (strcmp(key, element->key) == 0){
		store = removeFirst(store,NULL);
		return 0;
	}
	for(cur = store->next,prev = store;cur;prev = cur,cur = cur->next)
	{
		element = (struct json *)cur->value;
		if (strcmp(key, element->key) == 0){
			prev->next = removeFirst(cur,&tmp);
			element = (struct json *)tmp->value;
			free(element->key);
			free(element->value);
			return 0;
		}
	}
	return -1;
}
//TODO Hashage des clé
//TODO choisir une strucure approprié pour faciliter la recherche Arbre binaire
