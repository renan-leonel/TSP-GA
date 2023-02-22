#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "utils.cpp"
#include <time.h>

#define PORT 4444

int main(){
	clock_t start_t, end_t;
	double total_t;
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		std::cout<<"[-]Error in connection.\n";
		exit(1);
	}
	std::cout<<"[+]Client Socket is created.\n";
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		std::cout<<"[-]Error in connection.\n";
		exit(1);
	}
	std::cout<<"[+]Connected to Server.\n";


	while(1){
		std::cout<<"Client: ";
		scanf("%s", &buffer[0]);
		
		send(clientSocket, buffer, strlen(buffer), 0);

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			std::cout<<"[-]Disconnected from server.\n";
			
			exit(1);
		}

			if(recv(clientSocket, buffer, 1024, 0) < 0){
				std::cout<<"[-]Error in receiving data.\n";
		
			}else{
				std::cout<<"Server: \t"<<buffer<<std::endl;
				

			char* token;
			char* rest = buffer;
			int c = 1;
			double mut;
			int gen;
			int cross;
			int popsize;
			while ((token = strtok_r(rest, "|", &rest))){
				//"0.2|20|2000|100";
				switch (c)
				{
				case 1:
					mut = strtod(token,NULL);
					break;
				
				case 2:
					popsize = atoi(token);
					break;
				case 3:
					gen = atoi(token);
				
				case 4:
					cross = atoi(token);
				default:
					break;
				}
				c+=1;
			}
			std::cout<<"Executando o AG para populacao = "<<popsize<<" com taxa de mutacao de "<<mut<<" com "<<gen <<" geracoes"<<std::endl;
			std::string fileName = "att48.txt";
			std::string outputFile = "saidaClient.txt";

			fileReader::Reader entrada(fileName);
			entrada.readFile();
			Ga::Genetic genetico(mut, gen, entrada.get_cityPoints() , entrada.get_inputSize(), popsize, outputFile, 1, 1 );
			
			start_t = clock();
			float x = genetico.run();
			end_t = clock();
			total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
			
			std::string retorno = "O melhor valor encontrado = " + std::to_string(x) +  " com um tempo de execucao de " + std::to_string(total_t) +  " sec\n";
			std::cout<<"Resultad final 3: "<<x<<" Tempo de execucao: "<<total_t <<" sec\n";
			send(clientSocket, retorno.c_str(), retorno.size(), 0);
		}
	}
	return 0;
}