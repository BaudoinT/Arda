#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "socket.h"


int main(int argc,char ** argv){
	int socket_serv=creer_serveur(8080);
	if(socket_serv == -1){
		perror("Erreur lors de la création du serveur");
		return 0;
	}
	int socket_client;
	
	socket_client=accept(socket_serv, NULL, NULL);
	if(socket_client==-1){
	  perror("accept");
	  return 0;
	}

        char* message_bienvenue="Three Rings for the Elven-kings under the sky,\nSeven for the Dwarf-lords in their halls of stone,\nNine for Mortal Men doomed to die,\nOne for the Dark Lord on his dark throne\nIn the Land of Mordor where the Shadows lie.\nOne Ring to rule them all. One Ring to find them,\nOne Ring to bring them all and in the darkness bind them\nIn the Land of Mordor where the Shadows lie.";
	
	sleep(1);
	write(socket_client , message_bienvenue , strlen(message_bienvenue));
		
	
	char mess[50];
	while(socket_client){
	  int length = read(socket_client,mess, 50);
	  if(length>0){
	    write(socket_client, mess, length);		    
	  }
	}
	
}