#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int open_socket();
void bind_to_port(int , int);


int open_socket()
{
	int s = socket(PF_INET, SOCK_STREAM, 0);

	if(s == -1)
		printf("Error al abrir socket\n");

	return s;
}

void bind_to_port(int socket, int port)
{
	struct sockaddr_in name;
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t)htons(port);
	name.sin_addr.s_addr= htonl(INADDR_ANY);

	int reuse = 1;
	if(setsockopt(socket, SOL_SOCKET, SO_REUSEADDR,(char*)&reuse, sizeof(int)) == -1)
	{
		error("No es posible reusar el socket\n");
	}

	int c = bind(socket, (struct sockaddr*) &name, sizeof(name));

	if (c == -1)
	{
		perror("No se puede enlazar al puerto: direccion en uso \n");		
	}
}


int main()
{
	const int PORT = 7200;

	int listener = open_socket();

	if(listener == -1)
	{
		printf("Error en el listener\n");
		return;
	}

	bind_to_port(listener, PORT);

	if(listen(listener, 10) == -1)
	{
		printf("No es posible escuchar en ese puerto\n");
		return;
	}

	printf("Enlazado al puerto\n");

	while(1)
	{
		struct sockaddr_storage client;
		unsigned int addres_size = sizeof(client);

		//printf("Esperando al cliente\n");

		int connect = accept(listener, (struct sockaddr*) &client, &addres_size);
		
		if(connect == -1)
		{
			printf("No se puede conectar socket secundario\n");
		}

		//printf("Atendiendo al cliente\n");
		char* msg = malloc(2);

		send(connect,"Menu Principal", 14, 0);
		send(connect,"\n", 1, 0);
		send(connect,"1. Ingresar Registro", 20, 0);
		send(connect,"\n", 1, 0);
		send(connect,"2. Ver Registro", 15, 0);
		send(connect,"\n", 1, 0);
		send(connect,"3. Borrar Registro", 18, 0);
		send(connect,"\n", 1, 0);
		send(connect,"4. Buscar Registro", 18, 0);
		send(connect,"\n", 1, 0);
		send(connect,"5. Salir", 8, 0);
		send(connect,"\n", 1, 0);
		send(connect,"Ingrese una opcion:", 20, 0);
		int msn = recv(connect, msg, 1, 0);
		send(connect, msg, strlen(msg), 0);

		msg[msn] = '\0';

		printf("Me llegaron %d con %s", msn, msg);

		free(msg);
		
		close(connect); 

	}

	return 0;
}
