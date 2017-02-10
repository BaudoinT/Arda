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

	initialiser_signaux();

	int fd  = open("ascii_art",O_RDONLY);
 	char *message_bienvenue = malloc(3000*sizeof(char));
 	read(fd,message_bienvenue,3000*sizeof(char));
 	close(fd);

	int socket_serv=creer_serveur(8080);


	if(socket_serv == -1){
		perror("Erreur lors de la création du serveur");
		return 0;
	}
	
	int socket_client;
	while(1){
	
		socket_client=accept(socket_serv, NULL, NULL);
		if(socket_client==-1){
	  		perror("Erreur lors de la connection du client");
	  		return 0;
		}

		if((pid = fork()) == -1) {
			perror("Erreur lors de la création du processus fils");
		}else if(pid==0){
			write(socket_client,message_bienvenue,strlen(message_bienvenue));
		
	
			char mess[50];
			while(socket_client){
	  			int length = read(socket_client,mess, 50);
	  			if(length>0){
	    			write(socket_client, mess, length);		    
	  			}
			}
		}
	}	
}


