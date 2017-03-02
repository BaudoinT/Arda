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


int main(int argc,char ** argv){

	
	// ascii art 
	int fd  = open("ascii_art",O_RDONLY);
 	char *message_bienvenue = malloc(3000*sizeof(char));
 	read(fd,message_bienvenue,3000*sizeof(char));
 	close(fd);



 	initialiser_signaux();
	int socket_serv=creer_serveur(8080);


	if(socket_serv == -1){
		perror("Erreur lors de la création du serveur");
		return 0;
	}
	
	int socket_client;

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
			fprintf(discript_socket,message_bienvenue);
	
			char recu[50];
			char prompt[] = "<Arda> ";
			while(socket_client){
	  			fgets(recu,50,discript_socket);
	  			strcat(prompt,recu);
	  			fprintf(discript_socket,prompt);
			}
		}
	}	
}


