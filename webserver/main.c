#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

int main(int argc,char ** argv){

	int socket_serv=creer_serveur(8080);
	if(socket_serv == -1){
		perror("Erreur lors de la création du serveur");
		return 0;
	}

	int socket_client;
	while(1){
		socket_client=accept(socket_serv, NULL, NULL);
		if(socket_client==-1){
			perror("accept");
			return 0;
		}
		const char * message_bienvenue = "Bienvenue sur Arda, serveur web géré par Illuvatar en personne, ici, vous pouvez apprendre l'intégralité de l'histoire des peuples de Valinor, de Beleriand et de la Terre de millieu, sans oublié l'île de numéror.";
		write(socket_client , message_bienvenue , strlen(message_bienvenue));
	}
}