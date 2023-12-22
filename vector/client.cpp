#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

int	main(int argc, char const *argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

	if (argc != 3)
	{
		printf("Usage: %s <ip> <port>\n", argv[0]);
		return (1);
	}

	int PORT = atoi(argv[2]);

	// socket fonksiyonu, bir soket oluşturmak için kullanılır.
	// AF_INET, IPv4 adresleri için kullanılan bir makro
	// SOCK_STREAM, TCP protokolü için kullanılan bir makro
	// 0, varsayılan protokolü kullanmak için kullanılan bir makro
	client_fd = socket(AF_INET, SOCK_STREAM, 0);

	// struct yapısında hangi aileden olduğunu belirtiyorum
	serv_addr.sin_family = AF_INET;

	// port numaralarını ağ byte sırasına çevirmek amacıyla
	serv_addr.sin_port = htons(PORT);

	// inet_pton fonksiyonu,
	//	IPv4 veya IPv6 adreslerini metin formatından ("127.0.0.1") binary formatına dönüştürmek için kullanılır
	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

	// connect fonksiyonu, bir soketi bir sunucuya bağlamak için kullanılır
		status = connect(client_fd, (struct sockaddr *)&serv_addr,
			sizeof(serv_addr));

	while (1)
	{
		int byte = read(0, buffer, 1024 - 1);
		buffer[byte] = '\0';
		send(client_fd, buffer, byte + 1, 0);
		memset(buffer, 0, 1024);
	}
	close(client_fd);
	return (0);
}