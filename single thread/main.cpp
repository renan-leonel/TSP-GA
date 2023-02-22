#include<iostream>
#include "utils.cpp"
#include <unistd.h>
#include <time.h>
// #include <pthread.h>

std::string fileName = "att48.txt";

int main(){
    clock_t start_t, end_t;
	double total_t;

    std::string outputFile = "saidaThread1.txt";
    fileReader::Reader entrada(fileName);
    entrada.readFile();
    Ga::Genetic genetico(0.1, 50000, entrada.get_cityPoints() , entrada.get_inputSize(), 25, outputFile, 1, 1 );
    start_t = clock();
    genetico.run();
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    std::cout<<"Tempo de execucao: "<<total_t <<" sec \n";

    return 0 ;
}