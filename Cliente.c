#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(void){
    struct sockaddr_in direccionServidor;
    direccionServidor.sin_family = PF_INET;
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
    direccionServidor.sin_port = (in_port_t)htons(8080);

    int cliente = socket(PF_INET, SOCK_STREAM, 0);
    if(connect(cliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0){
        perror("Error en la conexión");
        return 1;
    }

    printf("                  DOGS\n");
    printf("------------------------------------------\n\n");
    printf("Menu Principal\n");
    printf("1. Ingresar Registro\n");
    printf("2. Ver Registro\n");
    printf("3. Borrar Registro\n");
    printf("4. Buscar Registro\n");
    printf("5. Salir\n");
    printf("Ingrese una opcion: \n");

    
    while(1){
        char* opcion[1];
        scanf("%s", opcion);

        send(cliente, opcion, strlen(opcion), 0);
    }

    

    return 0;
}