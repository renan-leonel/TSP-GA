#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<vector>
#include <cstring>
#include <iostream>
#include<string>

#define PORT 4444

std::vector<std::string> parametros{"0.1|20|50000|100"};

int main(){
	int idx = 0;
	int contador = 0;
	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		std::cout<<"[-]Error in connection.\n";
		
		exit(1);
	}
	std::cout<<"[+]Server Socket is created.\n";

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		std::cout<<"[-]Error in binding.\n";
		
		exit(1);
	}
	std::cout<<"[+]Bind to port "<< 4444 <<std::endl;
	
	if(listen(sockfd, 10) == 0){
		std::cout<<"[+]Listening....\n";
	}
	else{
		std::cout<<"[-]Error in binding.\n";
	}

	while(1){ 
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		std::cout<<"Connection accepted from " <<inet_ntoa(newAddr.sin_addr) <<" : "<< ntohs(newAddr.sin_port) <<std::endl;
		
		if((childpid = fork()) == 0){
			close(sockfd);
			while (1){	
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, "exit") == 0){
					std::cout<<"Disconnected from " <<inet_ntoa(newAddr.sin_addr) <<" : "<< ntohs(newAddr.sin_port) <<std::endl;
					
					break;
				}
				else{
					
					send(newSocket, parametros[idx].c_str(), parametros[idx].size(), 0);
					bzero(buffer, sizeof(buffer));
					//recv(newSocket, buffer, 1024, 0);
					std::cout<<"Client: Algoritmo finalizado "<<buffer<<std::endl;
				}
			}
		}
	}
	close(newSocket);

	return 0;
}