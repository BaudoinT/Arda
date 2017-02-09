#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "socket.h"

int creer_serveur(int port){
	
	int socket_serveur=socket(AF_INET , SOCK_STREAM , 0);
	if ( socket_serveur == -1){
		perror ( " socket_serveur " );
		return -1;
	} 
	
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	saddr.sin_addr.s_addr = INADDR_ANY;
	
	int optval=1;
	if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1){
		perror("Can no set SO_REUSEADDR option");
		return -1;
	}

	if (bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
		perror("bind socket_serveur");
		return -1;
	}
	
	if (listen(socket_serveur, 10) == -1){
		perror("listen socket_serveur");
		return -1;
	}
	
	return socket_serveur;
}