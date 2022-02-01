#include "client.h"
int sock;
void end(int SIG)
{
  close(sock);
  printf("\r\n<<<\n");
  exit(0);
}

int main(int argc, char *argv[]){
  int valeur;
  bool isPeer = false;
  signal(SIGSEGV,end);
  signal(SIGINT,end);
  if(argc == 3)
    sock = connectTo(argv[1],argv[2]);
  else
  	sock = connectTo("3000", "127.0.0.1");
  if(sock==-1){
    printf("Erreur lors de la connection au serveur\n");
    return 1;
  }
  printf("Connecter\n");
  write(sock,&isPeer,sizeof(bool));
  shell();
  close(sock);
  return 0;
}
