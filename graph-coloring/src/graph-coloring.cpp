#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "grafo.h"
#include "ler-arquivo.h"
#include "greedy.h"
#include "vector-utils.h"
#include "dsatur.h"
#include "greedy-backtracking2.h"
#include "./experimental/greedy-backtracking3.h"
#include "dsatur-backtracking.h"
#include "dsatur-sewell.h"
#include "dsatur-pass-always.h"
#include "dsatur-pass-conditional.h"
#include "dsatur-gac.h"

using namespace std;
using namespace grafo;

void adicionaListaDeAdjAoArquivo(fstream& arquivo, grafo::Grafo& grafo) {
    arquivo << "adjList=[";
    for (int j=0; j < grafo.listaAdj.size(); j++) {
        arquivo << "[";
        for (int k=0; k < grafo.listaAdj[j].size(); k++) {
            arquivo << grafo.listaAdj[j][k];
            if (k != grafo.listaAdj[j].size() - 1) {
                arquivo << ","; 
            }
        }
        arquivo << "]";
        if (j != grafo.listaAdj.size() - 1) {
            arquivo << ","; 
        }
    }
    arquivo << "]; \n";
}

void salvarEmArquivo (string filename, grafo::Grafo& grafo, vector<int> cores) {
    fstream myfile;
    myfile.open(filename,fstream::out);
    myfile << "dataResult = ";
    myfile << "{\"adjList\":[";
    for (int j=0; j < grafo.listaAdj.size(); j++) {
        myfile << "[";
        for (int k=0; k < grafo.listaAdj[j].size(); k++) {
            myfile << grafo.listaAdj[j][k];
            if (k != grafo.listaAdj[j].size() - 1) {
                myfile << ","; 
            }
        }
        myfile << "]";
        if (j != grafo.listaAdj.size() - 1) {
            myfile << ","; 
        }
    }
    myfile << "], \"colors\": [";
        for (int k=0; k < cores.size(); k++) {
            myfile << cores[k];
            if (k != cores.size() - 1) {
                myfile << ","; 
            }
        }
    myfile << "]}";
    myfile.close();
}

/* Função principal */
int main(int argc, char** argv) {
    grafo::Grafo grafo = lerArquivo::lerArquivo(getenv("FILE"));

    string algorithm = getenv("ALG");

    if (algorithm == "greedy") {
        std::cout << "{\"greedy\": {\"colors\": ";
        /* https://www.geeksforgeeks.org/measure-execution-time-function-cpp/ */

        auto start = std::chrono::high_resolution_clock::now();
        vector<int> cores = greedy::greedy(grafo);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        std::cout << (*max_element(cores.begin(), cores.end()) + 1);
        std:: cout << ", \"time\":" << duration.count();
        grafo::verificaColoracao(grafo, cores);
        std::cout << "}}";
        salvarEmArquivo("output/greedy-heuristic-output.js", grafo, cores);
    }

    else if (algorithm == "dsatur") {
        std::cout << "{\"dsatur\": {\"colors\": ";

        auto start = std::chrono::high_resolution_clock::now();
        vector<int> cores2 = dsatur::DSATUR(grafo);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        std::cout << (*max_element(cores2.begin(), cores2.end()) + 1);
        std:: cout << ", \"time\":" << duration.count();
        grafo::verificaColoracao(grafo, cores2);
        std::cout << "}}";
        salvarEmArquivo("output/dsatur-heuristic-output.js", grafo, cores2);
    }

    else if (algorithm == "dsatur-backtracking") {
        int backtrackingVertices = 0;

        std::cout << "{\"dsatur-backtracking\": {\"colors\": ";
        /* Cria um arquivo log para o greedy-backtracking */
        fstream dsaturBacktrackingLog;
        /* TODO: Pasta deve estar criada para abertura do arquivo funcionar */
        dsaturBacktrackingLog.open("output/dsatur-backtracking-output.js", fstream::out);
        adicionaListaDeAdjAoArquivo(dsaturBacktrackingLog, grafo);
        dsaturBacktrackingLog << "graphFileName = '" << getenv("FILE") << "';\n";    
        dsaturBacktrackingLog << "logs = ";
        dsaturBacktrackingLog << "[";

        auto start = std::chrono::high_resolution_clock::now();
        vector<int> cores4 = dsaturBacktracking::dsaturBacktracking(grafo, dsaturBacktrackingLog, backtrackingVertices);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        dsaturBacktrackingLog << "]";

        std::cout << (*max_element(cores4.begin(), cores4.end()) + 1);
        std::cout << ", \"time\":" << duration.count();
        std::cout << ", \"backtrackingVertices\":" << backtrackingVertices;

        grafo::verificaColoracao(grafo, cores4);
        std::cout << "}}";
        dsaturBacktrackingLog.close();    
    }

    else if (algorithm == "dsatur-sewell") {
        int backtrackingVertices = 0;

        std::cout << "{\"dsatur-sewell\": {\"colors\": ";
        /* Cria um arquivo log para o dsatur-sewell */
        fstream dsaturSewellLog;
        /* TODO: Pasta deve estar criada para abertura do arquivo funcionar */
        dsaturSewellLog.open("output/dsatur-sewell-output.js", fstream::out);
        adicionaListaDeAdjAoArquivo(dsaturSewellLog, grafo);
        dsaturSewellLog << "graphFileName = '" << getenv("FILE") << "';\n";    
        dsaturSewellLog << "logs = ";
        dsaturSewellLog << "[";

        auto start = std::chrono::high_resolution_clock::now();
        vector<int> cores4 = dsaturSewell::dsaturSewell(grafo, dsaturSewellLog, backtrackingVertices);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        dsaturSewellLog << "]";

        std::cout << (*max_element(cores4.begin(), cores4.end()) + 1);
        std::cout << ", \"time\":" << duration.count();
        std::cout << ", \"backtrackingVertices\":" << backtrackingVertices;        
        grafo::verificaColoracao(grafo, cores4);
        std::cout << "}}";
        dsaturSewellLog.close();    
    }

    else if (algorithm == "dsatur-pass-always") {
        int backtrackingVertices = 0;

        std::cout << "{\"dsatur-pass-always\": {\"colors\": ";
        /* Cria um arquivo log para o dsatur-pass */
        fstream dsaturPassLog;
        /* TODO: Pasta deve estar criada para abertura do arquivo funcionar */
        dsaturPassLog.open("output/dsatur-pass-always-output.js", fstream::out);
        adicionaListaDeAdjAoArquivo(dsaturPassLog, grafo);
        dsaturPassLog << "graphFileName = '" << getenv("FILE") << "';\n";    
        dsaturPassLog << "logs = ";
        dsaturPassLog << "[";

        auto start = std::chrono::high_resolution_clock::now();
        vector<int> cores4 = dsaturPassAlways::dsaturPassAlways(grafo, dsaturPassLog, backtrackingVertices);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        dsaturPassLog << "]";

        std::cout << (*max_element(cores4.begin(), cores4.end()) + 1);
        std::cout << ", \"time\":" << duration.count();
        std::cout << ", \"backtrackingVertices\":" << backtrackingVertices;        
        grafo::verificaColoracao(grafo, cores4);
        std::cout << "}}";
        dsaturPassLog.close();    
    }    

    else if (algorithm == "dsatur-pass-conditional") {
        int backtrackingVertices = 0;

        std::cout << "{\"dsatur-pass-conditional\": {\"colors\": ";
        /* Cria um arquivo log para o dsatur-pass */
        fstream dsaturPassLog;
        /* TODO: Pasta deve estar criada para abertura do arquivo funcionar */
        dsaturPassLog.open("output/dsatur-pass-conditional-output.js", fstream::out);
        adicionaListaDeAdjAoArquivo(dsaturPassLog, grafo);
        dsaturPassLog << "graphFileName = '" << getenv("FILE") << "';\n";    
        dsaturPassLog << "logs = ";
        dsaturPassLog << "[";

        auto start = std::chrono::high_resolution_clock::now();
        vector<int> cores4 = dsaturPassConditional::dsaturPassConditional(grafo, dsaturPassLog, backtrackingVertices);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        dsaturPassLog << "]";

        std::cout << (*max_element(cores4.begin(), cores4.end()) + 1);
        std::cout << ", \"time\":" << duration.count();
        std::cout << ", \"backtrackingVertices\":" << backtrackingVertices;        
        grafo::verificaColoracao(grafo, cores4);
        std::cout << "}}";
        dsaturPassLog.close();    
    }        

    else if (algorithm == "dsatur-gac") {
        int backtrackingVertices = 0;

        std::cout << "{\"dsatur-gac\": {\"colors\": ";
        /* Cria um arquivo log para o dsatur-pass */
        fstream dsaturGACLog;
        /* TODO: Pasta deve estar criada para abertura do arquivo funcionar */
        dsaturGACLog.open("output/dsatur-gac-output.js", fstream::out);
        adicionaListaDeAdjAoArquivo(dsaturGACLog, grafo);
        dsaturGACLog << "graphFileName = '" << getenv("FILE") << "';\n";    
        dsaturGACLog << "logs = ";
        dsaturGACLog << "[";

        auto start = std::chrono::high_resolution_clock::now();
        vector<int> cores4 = dsaturGAC::dsaturGAC(grafo, dsaturGACLog, backtrackingVertices);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        dsaturGACLog << "]";

        std::cout << (*max_element(cores4.begin(), cores4.end()) + 1);
        std::cout << ", \"time\":" << duration.count();
        std::cout << ", \"backtrackingVertices\":" << backtrackingVertices;        
        grafo::verificaColoracao(grafo, cores4);
        std::cout << "}}";
        dsaturGACLog.close();    
    }        

    else if (algorithm == "greedy-backtracking") {
        int backtrackingVertices = 0;

        std::cout << "{\"greedy-backtracking\": {\"colors\": ";
        /* Cria um arquivo log para o greedy-backtracking */
        fstream greedyBacktrackingLog;
        /* TODO: Pasta deve estar criada para abertura do arquivo funcionar */
        greedyBacktrackingLog.open("output/greedy-backtracking-output.js", fstream::out);
        adicionaListaDeAdjAoArquivo(greedyBacktrackingLog, grafo);
        greedyBacktrackingLog << "graphFileName = '" << getenv("FILE") << "';\n";
        greedyBacktrackingLog << "logs = ";
        greedyBacktrackingLog << "[";
        /* Greedy backtracking 2 contem uma versão com variavel de ordenacao
        Greedy backtracking (1) é um pouco mais rápida */
        auto start = std::chrono::high_resolution_clock::now();
        vector<int> cores3 = greedyBacktracking2::greedyBacktracking2(grafo, greedyBacktrackingLog, backtrackingVertices);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        
        greedyBacktrackingLog << "]";
        
        std::cout << (*max_element(cores3.begin(), cores3.end()) + 1);
        std::cout << ", \"time\":" << duration.count();
        std::cout << ", \"backtrackingVertices\":" << backtrackingVertices;

        grafo::verificaColoracao(grafo, cores3);
        std::cout << "}}";
        greedyBacktrackingLog.close();
    }

    return 0;
}