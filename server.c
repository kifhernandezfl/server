#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(void)
{
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons(8080);

	int servidor = socket(AF_INET, SOCK_STREAM, 0);

	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	if(bind(servidor,(void*)&direccionServidor,sizeof(direccionServidor)) != 0)
	{
		perror("Fallo el bind");
		return 1;
	}

	printf("Estoy escuchando\n");
	listen(servidor, 100);

	//-----------------------------------------------

	struct sockaddr_in direccionCliente;
	unsigned int tamanoDireccion;
	int cliente = accept(servidor, (void*)&direccionCliente, &tamanoDireccion);

	printf("Recibi una conexion en %d!!\n", cliente);

	char* buffer = malloc(5);

	while(1)
	{
		int br = recv(cliente, buffer, 4, 0);
		if(br < 0)
		{
			perror("Desconecto");
			return 1;
		}

		buffer[br] = '\0';
		printf("llegaron %d bytes con %s", br, buffer);
	}
	
	free(buffer);

	return 0;

}