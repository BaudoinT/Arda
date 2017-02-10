#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "socket.h"
#include <signal.h>
#include <sys/wait.h>

pid_t pid;


void traitement_signal ( int sig ){
	printf("Signal %d reçu \n",sig );
	waitpid(pid,NULL,WNOHANG);
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

    	char* message_bienvenue="Three Rings for the Elven-kings under the sky,\nSeven for the Dwarf-lords in their halls of stone,\nNine for Mortal Men doomed to die,\nOne for the Dark Lord on his dark throne\nIn the Land of Mordor where the Shadows lie.\nOne Ring to rule them all. One Ring to find them,\nOne Ring to bring them all and in the darkness bind them\nIn the Land of Mordor where the Shadows lie.\n\n";
		sleep(1);
	

		if((pid = fork()) == -1) {
			perror("Erreur lors de la création du processus fils");
		}else if(pid==0){
			write(socket_client , message_bienvenue , strlen(message_bienvenue));
		
	
			char mess[50];
			while(socket_client){
	  			int length = read(socket_client,mess, 50);
	  			if(length>0){
	    			write(socket_client, mess, length);		    
	  			}
			}
		}
		initialiser_signaux();
	}	
}


