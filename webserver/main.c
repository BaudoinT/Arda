#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>


pid_t pid;


void traitement_signal ( int sig ){
	printf("Signal %d reçu \n",sig );
	waitpid(pid,&sig,WNOHANG);
}

void initialiser_signaux ( void ){
	if(signal(SIGPIPE,SIG_IGN) == SIG_ERR){
		perror ( "Erreur lors de l'initialisation des signaux" );
	}

	struct sigaction sa;

	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGCHLD,&sa,NULL)==-1){
		perror("sigaction(SIGCHLD)");
	}
}


char *substr(char *src,int pos,int len) { 
  char *dest=NULL;                        
  if (len>0) {                            
    dest = (char *) malloc(len+1);        
    strncat(dest,src+pos,len);            
  }
  return dest;                            
}


int verif_requete(char *mot){
	if (strcmp("GET",substr(mot,0,3))){
		return -1;
	}
	if (strcmp("HTTP/1.1",substr(mot,6,8))!=0 && strcmp("HTTP/1.0",substr(mot,6,8))!=0){
		return -1;
	}
	return 0;
}

int main(int argc,char ** argv){

	
	// ascii art 
	int fd  = open("ascii_art",O_RDONLY);
 	char *message_bienvenue = malloc(1600*sizeof(char));
 	read(fd,message_bienvenue,1600*sizeof(char));
 	close(fd);

    initialiser_signaux();
    int socket_client;
    int socket_serv=creer_serveur(8080);
	
    if(socket_serv == -1){
		perror("Erreur lors de la création du serveur");
		return 0;
	}

    while(1){
		socket_client=accept(socket_serv, NULL, NULL);
		FILE * discript_socket = fdopen(socket_client ,"w+");
		if(socket_client==-1){
	  		perror("Erreur lors de la connection du client");
	  		return 0;
		}

		if((pid = fork()) == -1) {
			perror("Erreur lors de la création du processus fils");
		}else if(pid==0){			
            char recu[250];
            fgets(recu,250,discript_socket);
            if(verif_requete(recu)==-1)
                fprintf(discript_socket,"HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n");
            else{
              
                    fprintf(discript_socket,"HTTP/1.1 200 OK\r\nConnection: open\r\nContent-Length: 1602\r\n\r\n%s\r\n", message_bienvenue);
            }
            
            while(fgets(recu,250,discript_socket)!= NULL){
                if(recu[0]=='\n' || (recu[0]=='\r' && recu[1]=='\n'))
	  	            printf("%s",recu);
            }
			close(socket_client);
			return 0;

		}
	}	
}

