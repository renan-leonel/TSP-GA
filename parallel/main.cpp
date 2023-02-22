#include<iostream>
#include <thread>
#include "utils.cpp"
#include <unistd.h>
// #include <pthread.h>
#include <time.h>

std::ofstream outfile;

std::string fileName = "att48.txt";

void thread1(){
    std::string outputFile = "thread1.txt";
    fileReader::Reader entrada(fileName);
    entrada.readFile();
    Ga::Genetic genetico(0.1, 50000, entrada.get_cityPoints() , entrada.get_inputSize(), 25, outputFile, 0, 1 );
    clock_t start_t, end_t;
	double total_t;
    start_t = clock();
    float res = genetico.run();
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    outfile<<"Resultad final 3: "<<res<<" Tempo de execucao: "<<total_t <<" sec\n";
}

void thread2(){
    std::string outputFile = "thread2.txt";
    fileReader::Reader entrada(fileName);
    entrada.readFile();
    Ga::Genetic genetico(0.3, 10000, entrada.get_cityPoints() , entrada.get_inputSize(), 10, outputFile, 0, 2 );
    clock_t start_t, end_t;
	double total_t;
    start_t = clock();
    float res = genetico.run();
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    outfile<<"Resultad final 3: "<<res<<" Tempo de execucao: "<<total_t <<" sec\n";
}

void thread3(){
    std::string outputFile = "thread3.txt";
    fileReader::Reader entrada(fileName);
    entrada.readFile();
    Ga::Genetic genetico(0.2, 10000, entrada.get_cityPoints() , entrada.get_inputSize(), 50, outputFile, 1,3);
    clock_t start_t, end_t;
	double total_t;
    start_t = clock();
    float res = genetico.run();
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    outfile<<"Resultad final 3: "<<res<<" Tempo de execucao: "<<total_t <<" sec\n";
}

void thread4(){
    std::string outputFile = "thread4.txt";
    fileReader::Reader entrada(fileName);
    entrada.readFile();
    Ga::Genetic genetico(0.7, 10000, entrada.get_cityPoints() , entrada.get_inputSize(), 5, outputFile, 1,4);
    clock_t start_t, end_t;
	double total_t;
    start_t = clock();
    float res = genetico.run();
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    outfile<<"Resultad final 3: "<<res<<" Tempo de execucao: "<<total_t <<" sec\n";
}

void (*ponteirodefuncao[4])() = {thread1,thread2,thread3,thread4};

int main(){
    outfile.open("Saida.txt");
    
    try
    {
        std::vector<std::thread> threadsList;

        for(int i = 0; i< 4; i++)
            threadsList.push_back(std::thread(ponteirodefuncao[i]));
        
        for(int i = 0; i< 4; i++)
            threadsList[i].join();

    }
    catch (const std::exception &ex)
    {
        std::cout << "Error" << ex.what() << "\n";
    }
    return 0 ;
}