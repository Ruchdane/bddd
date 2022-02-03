#include "server.h"

struct server server = { -1, "3000","localhost", NULL, NULL };

static void end(int sig) {
	struct client* client, * peer;
	struct element* tmp, * foo;
	char address_buffer[100];

	printf("\rSIG %d\n", sig);

	for (tmp = server.clients; tmp;) {
		client = (struct client*)tmp->value;
		getnameinfo((struct sockaddr*)&(client->addr), sizeof(client->addr), address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);

		close(client->socket);
		pthread_kill(client->thread, SIGKILL);
		printf("%s : Client disconnected \n", address_buffer);

		foo = tmp;
		tmp = tmp->next;
		free(freeElement(foo));
	}

	for (tmp = server.peers; tmp;) {
		peer = (struct client*)tmp->value;

		getnameinfo((struct sockaddr*)&(peer->addr), sizeof(peer->addr), address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
		close(peer->readsock);
		close(peer->writesock);
		pthread_kill(peer->thread, SIGKILL);
		printf("%s : peer disconnected \n", address_buffer);

		foo = tmp;
		tmp = tmp->next;
		free(freeElement(foo));
	}
	close(server.socket);
	exit(EXIT_SUCCESS);
}


void* broadcastPutThread(void* data) {
	struct json* json = (struct json*)data;
	struct element* element;
	struct client* peer;
	foreach(element, server.peers) {
		peer = (struct client*)element->value;
		_put(peer->writesock, json->key, strlen(json->key), json->value, strlen(json->value));

	}
	pthread_exit(NULL);
}

void* broadcastDeleteThread(void* data) {
	char* key = (char*)data;
	struct element* element;
	struct client* peer;
	foreach(element, server.peers) {
		peer = (struct client*)element->value;
		_delete(peer->writesock, key, strlen(key));
	}
	pthread_exit(NULL);
}

void* ClientThread(void* param) {
	struct client* client = (struct client*)param;
	int option, exit = 1;
	char* key;
	struct json* json;
	pthread_t replication_thread;

	printf("Client conected\n");

	while (exit) {
		exit = read(client->socket, &option, sizeof(option));
		switch (option) {
			case 0: //GET
				exit = __get(client->socket, get);
				break;

			case 1: //DELETE
				key = __delete(client->socket, delete);
				if (key)
					pthread_create(&replication_thread, NULL, broadcastDeleteThread, (void*)key);
				else
					exit = 0;
				break;

			case 2: //PUT
				json = __put(client->socket, put);
				if (json)
					pthread_create(&replication_thread, NULL, broadcastPutThread, (void*)json);
				else
					exit = 0;
				break;
			default:
				exit = 0;
				break;
		}
	}
	pthread_exit(NULL);
}

void* PeerThread(void* param) {
	struct client* peer = (struct client*)param;
	int option, exit = 1;
	printf("Peer conected\n");
	while (exit) {
		exit = read(peer->readsock, &option, sizeof(option));
		switch (option) {
			case 0: //GET
				exit = __get(peer->readsock, get);
				break;

			case 1: //DELETE
				exit = __delete(peer->readsock, delete) ? 1 : 0;
				break;

			case 2: //PUT
				exit = __put(peer->readsock, put) ? 1 : 0;
				break;

			default:
				exit = 0;
				break;
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char const* argv[]) {
	server.clients = createList();
	server.peers = createList();
	signal(SIGINT, end);
	signal(SIGSEGV, end);

	if (argc > 1)
		server.port = argv[1];

	printf("Configuration du serveur au port %s ....\n", server.port);
	server.socket = bindTo(server.port, NULL);
	printf("Fin de la configuration\n");

	if (argc > 3) {
		printf("Connection au peer %s::%S ....\n", argv[3], argv[2]);
		struct client* client;
		size_t len;
		socklen_t addrlen;

		client = malloc(sizeof(*client));
		addrlen = sizeof(client->addr);
		client->socket = 0;
		client->isPeer = true;
		client->writesock = connectTo(argv[2], argv[3]);
		client->readsock = 0;
		write(client->writesock, &client->isPeer, sizeof(bool));

		len = strlen(server.port);
		write(client->writesock, &len, sizeof(len));
		write(client->writesock, server.port, len);

		len = strlen(server.address);
		write(client->writesock, &len, sizeof(len));
		write(client->writesock, server.address, len);
		if (!listen(server.socket, 1)) {
			client->readsock = accept(server.socket, (struct sockaddr*)&(client->addr), &addrlen);
			if (client->readsock == -1) {
				log("erreur lors reception de la connexion")
			}
			pthread_create(&(client->thread), NULL, PeerThread, (void*)client);
			server.peers = append(server.peers, client);
			printf("\nFin de la configuration\n");
		}
		else
			log("erreur lors de la connection inverse vers");
		printf("\nFin de la configuration\n");
	}

	while (true) {
		if (listen(server.socket, 10)) {
			log("Erreur lors de la receptions des connection au socket\n");
		}

		printf("Ecoute...\n");
		struct client* client;
		socklen_t client_len;
		char address_buffer[100];
		int client_socket;

		client = malloc(sizeof(*client));
		client_len = sizeof(client->addr);
		client_socket = accept(server.socket, (struct sockaddr*)&(client->addr), &client_len);

		read(client_socket, &client->isPeer, sizeof(bool));
		getnameinfo((struct sockaddr*)&(client->addr), sizeof(client->addr), address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);

		if (!client->isPeer) {
			client->socket = client_socket;
			server.clients = append(server.clients, client);
			pthread_create(&(client->thread), NULL, ClientThread, (void*)client);
		}
		else {
			char* peerPort, * peerAddress;
			size_t len;

			client->socket = 0;
			client->readsock = client_socket;

			read(client->readsock, &len, sizeof(len));
			peerPort = malloc(len);
			read(client->readsock, peerPort, len);

			read(client->readsock, &len, sizeof(len));
			peerAddress = malloc(len);
			read(client->readsock, peerAddress, len);

			client->writesock = connectTo(peerPort, peerAddress);
			server.peers = append(server.peers, client);

			pthread_create(&(client->thread), NULL, PeerThread, (void*)client);

			// free(peerAddress);
			// free(peerPort);
		}

	}

	return 0;
}
//test
// put 'a' 'A'
// get 'a'
// put 'b' 'B'
// put 'c' 'C'
// delete 'b'   
// get 'b'
// put 'c' 'B'
// get 'c'
//

// >>> put 'a' 'A'
// >>> get 'a'
// a:A
// >>> put 'b' 'B'
// >>> put 'c' 'C'
// >>> delete 'b' 
// >>> put 'c' 'B'  
// >>> get 'b'
// clé non retrouver
// >>> get 'c'
// c:B
