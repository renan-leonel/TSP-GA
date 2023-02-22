#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <sstream>
#include <fstream>
#include <map>

namespace Ga
{
    class Genetic{
        private:
            float mutationRate;
            int nGenerations;
            int popSize;
            int inputSize;
            std::vector<int> bestSolution ;
            std::vector<std::vector<int>> population;
            std::vector<float> fitness;
            std::vector<std::vector<float>> cityPoints;
            std::vector<int> result ;
            std::string outputFile;
            int op;
            int t;
            
            double pitagoras(float x1, float y1, float x2, float y2){
                float x = x2 - x1 ;
                float y = y2 - y1; 
                float raiz = (x*x) + (y*y) ;
                return sqrt(raiz);
            }

            int getIndex(std::vector<int> v, int K)
            {
                auto it = std::find(v.begin(), v.end(), K);
                if (it != v.end()) 
                {
                    int index = it - v.begin();
                    return index;
                }
                else {
                    return -1;
                }
            }

            int findLowestFit(){
                int idx = 0;
                float val = this->fitness[0]; 

                for(int i = 0; i < this->popSize; i++){
                    if(val > this->fitness[i]){
                        val = this->fitness[i];
                        idx = i;
                    }
                } 

                return idx;
            }

            int findBestFit(){
                int idx = 0;
                float val = this->fitness[0]; 

                for(int i = 0; i < this->popSize; i++){
                    if(val < this->fitness[i]){
                        val = this->fitness[i];
                        idx = i;
                    }
                } 
                return idx;
            }

            bool findElem(std::vector<int> v, int K)
            {
                for(int i = 0; i < v.size(); i++){
                    if(v[i] == K)
                        return true;
                }
                return false;
            }
            
            void createInitialPopulation(){

                std::random_device dev;
                std::mt19937 rng(dev());

                for(int i = 0; i < this->popSize; i++){
                    std::vector<int> pop(this->inputSize);
                    std::iota(pop.begin(), pop.end(), 1);
                    shuffle(pop.begin(), pop.end(), rng);

                    this->population.push_back(pop);
                }
            }

            std::vector<std::vector<int>>& get_population(){
                return this->population;
            }

            std::vector<float> get_fitness(){
                return this->fitness;
            }

            float pathValue(std::vector<int> path){
                
                float value = 0.0; 

                for(int i = 0; i < path.size() - 2; i++){
                    value += this->pitagoras(
                        this->cityPoints[path[i]-1][0],
                        this->cityPoints[path[i]-1][1],
                        this->cityPoints[path[i+1]-1][0],
                        this->cityPoints[path[i+1]-1][1]
                    );
                }
                
                return value ;
            }

            void calculateFitness(){
                std::vector<float> fit;
                for(int i = 0; i< this->popSize; i++){
                    fit.push_back(1.0/this->pathValue(this->population[i]));
                }   
                this->fitness = fit;
            }
            
            void showPath(std::vector<int> path){
                for(auto& c: path)
                    std::cout<<c<<" ";
                
                std::cout<<"\n";
            }

            int rouletteSelection(){
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_real_distribution<double> dist(0.0, 1.0);
                float total = std::accumulate(this->fitness.begin(), this->fitness.end(), 0.0);
                std::vector<float> probability ;
                for(auto fit: this->fitness)
                    probability.push_back(fit/total);
                    
                float offset = 0.0;
                int pick = 0;
                int c = 0; 
                float rng = dist(mt);
                
                while (c < this->popSize){
                    offset += probability[c];
                    if(rng < offset){
                        return c;
                    }
                    c+=1;
                }
                return pick;
            }

            void mutation(std::vector<int>& path){
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_int_distribution<int> dist(0, this->inputSize-1);
                int pos1 = dist(mt);
                int pos2 = dist(mt);
                //std::cout<<pos1<<" "<<pos2<<std::endl;
                int aux = path[pos1]; 
                path[pos1] = path[pos2]; 
                path[pos2] = aux;
                //std::cout<<"executandoMutacao"<<std::endl;
            }
            
            void cX(std::vector<int> path1, std::vector<int> path2){
                std::vector<int> result1; 
                std::vector<int> result2; 

                result1 = path2;
                int idx = 0; 
                do{
                    result1[idx] = path1[idx]; 
                    idx = this->getIndex(path2, result1[idx]);
                }
                while(idx != 0 and idx != -1);

                result2 = path1;
                idx = 0; 
                do{
                    result2[idx] = path2[idx]; 
                    idx = this->getIndex(path1, result2[idx]);
                
                }
                while(idx != 0 and idx != -1);

                if (this->pathValue(result1) < this->pathValue(result2)){
                    this->result = result1;
                }else{
                    this->result = result2;
                }
            }

            void oX(std::vector<int> path1, std::vector<int> path2){

                std::vector<int> ind1 = path1; 
                std::vector<int> ind2 = path2; 

                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_int_distribution<int> dist(0, this->inputSize-1);

                std::vector<int> result1; 
                std::vector<int> result2; 
                int n1 = dist(mt);
                int n2 = dist(mt);
                int a = std::fmin(n1, n2);
                int b = std::fmax(n1, n2);

                std::vector<bool> holes1(this->inputSize, true); 
                std::vector<bool> holes2(this->inputSize, true); 
               
                for(int k = 0; k< this->inputSize; k++){
                    holes1[ind2[k]] = false;
                    holes2[ind1[k]] = false;
                }

                std::vector<int> temp1 = path1; 
                std::vector<int> temp2 = path2; 
                
                int k1 = b+1;
                int k2 = b+1;

                for(int i = 0; i < this->inputSize; i++){
                    if( ! holes1[temp1[(i + b + 1) % this->inputSize]]){
                        ind1[k1 % this->inputSize] = temp1[(i + b + 1) % this->inputSize];
                        k1 += 1;
                    }

                    if (! holes2[temp2[(i + b + 1) % this->inputSize]]){
                        ind2[k2 % this->inputSize] = temp2[(i + b + 1) % this->inputSize];
                        k2 += 1;
                    }
                }
                
                for(int i = a; i < b+1; i++){
                    int aux = ind1[i];
                    ind1[i] = ind2[i];
                    ind2[i] = aux;
                }

                if (this->pathValue(ind1) < this->pathValue(ind2)){
                    this->result = ind1;
                }else{
                    this->result = ind2;
                }
            }

            bool verifyPath(std::vector<int> path){
                std::vector<bool> aux(path.size(),false);

                for(int i = 0; i < path.size(); i++){
                    if(aux[path[i]] == false){
                        aux[path[i]] = true;
                    }
                    else{
                        return false;
                    }
                }
                return true;
            }

            int populationMaintenance(){
                int idx = this->findLowestFit(); 
                if (this->pathValue(this->result) < this->pathValue(this->population[idx])){
                    if( this->verifyPath(this->result)){
                        this->population[idx] = this->result;

                    }else{
                       // std::cout<<"CAMINHO INVALIDO"<<std::endl;
                        return 0;
                    }
                }
            }
      
            void mostrarPopulacao(){
                for(auto& row: this->population){
                    for(auto& col:row){
                        std::cout<<col<<" ";
                    }
                    std::cout<<"\n";
                }
            }

        public:
            Genetic(){
            }
            
            Genetic(float mutationRate,int nGenerations, std::vector<std::vector<float>> cityPoints, int inputSize, int popSize, std::string outputFile, int op, int t){
                this->mutationRate = mutationRate;
                this->nGenerations = nGenerations;
                this->cityPoints = cityPoints;
                this->popSize = popSize;
                this->inputSize = inputSize;
                this->outputFile = outputFile;
                this->op = op;
                this->t = t;
            } 
            
            float run(){
                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_real_distribution<double> dist(0.0, 1.0);

                std::ofstream outfile;
                outfile.open(this->outputFile);

                int gen = 0; 
                this->createInitialPopulation();
                //this->mostrarPopulacao();
                //std::cout<<this->popSize<<std::endl;
                
                //std::cout<<this->pathValue(this->population[best])<<std::endl;
                while (gen < this->nGenerations)
                {

                    this->calculateFitness();
                    //std::cout<<"executando thread "<<this->t<<std::endl;
                    int pathIdx1 = this->rouletteSelection();
                    int pathIdx2 = this->rouletteSelection();

                    if(this->op == 1)
                        this->oX(this->population[pathIdx1], this->population[pathIdx2]);
                    else 
                        this->cX(this->population[pathIdx1], this->population[pathIdx2]);

                    float mut = dist(mt);

                    if(mut<= this->mutationRate)
                        this->mutation(this->result);
                    

                    int x = this->populationMaintenance();
                    int best = this->findBestFit();
                    std::cout<<this->pathValue(this->population[best])<<std::endl;    
                    outfile<<this->pathValue(this->population[best])<<"\n";
                    

                    gen+=1;
                }

                int best = this->findBestFit();
                return this->pathValue(this->population[best]);   
            }
    };
    
}; // namespace LeituraArquivo

namespace fileReader
{
    class Reader{
        private:
            int inputSize = 0;
            std::string fileName;
            std::vector<std::vector<float>> cityPoints;

        public:
            Reader(){
            }
            
            Reader(std::string name){
                this->fileName = name;
            }

            void readFile(){
                std::ifstream arquivo;
                arquivo.open(this->fileName);
                
                int counter = 0;
                std::string line;
                std::string buffer;
                float n ,x, y ;
           

                if(arquivo.is_open()){
                    while(getline(arquivo, line)){
                        if(counter == 3){

                            std::stringstream read(line);
                            read >> buffer >> buffer >> this->inputSize;
                        }
                        if(counter > 5 and line != "EOF"){
                            std::stringstream read(line);
                            read >> n >> x >> y;
                            std::vector<float> points = {0,0};
                            points[0] = x ;
                            points[1] = y ;
                            this->cityPoints.push_back(points) ;     
            
                        }
                        counter+=1;
                    }
                }
                arquivo.close();
            }

            void showCityPoints(){
                for(auto city : this->cityPoints)
                    std::cout<<city[0] << " "<<city[1]<<std::endl;
            }

            int get_inputSize(){
                return this->inputSize;
            }
    
            std::vector<std::vector<float>> get_cityPoints(){
                return this->cityPoints;
            }
    };
    
}; // namespace LeituraArquivo