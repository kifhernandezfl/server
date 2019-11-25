#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(void){
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = PF_INET;
	direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
	direccionServidor.sin_port = (in_port_t)htons(8080);

	int servidor = socket(PF_INET, SOCK_STREAM, 0);

	int act = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &act, sizeof(act));

	if(bind(servidor, (void*) &direccionServidor, sizeof(direccionServidor)) != 0){
		perror("Fallo el bind");
		return 1;
	}

	printf("Estoy escuchando\n");
	listen(servidor, 100);
	
	struct sockaddr_in direccionCliente;
	unsigned int tamanoDireccion;
	int cliente = accept(servidor, (void*) &direccionCliente, &tamanoDireccion);

	printf("Recibí una conexión en %d\n", cliente);

	//--------------------------------------

	char* opcion = malloc(1);

	
	int bytesRecibidos = recv(cliente, opcion, 1, 0);
	if (bytesRecibidos <= 0){
		perror("Error en la conexión");
		return 1;
	}

	int o = atoi(opcion);

	if(o == 1){
		printf("La opción Ingresar se ejecuta \n");
	}else{
		if(o == 2){
			printf("La opción Ver se ejecuta \n");
		}else{
			if(o == 3){
				printf("La opción Borrar se ejecuta \n");
			}else{
				if(o == 4){
					printf("La opción Buscar se ejecuta \n");
				}else{
					printf("La opcion Salir se ejecuta \n");
				}
			}
		}
	}		
		
	free(opcion);

	return 0;
}