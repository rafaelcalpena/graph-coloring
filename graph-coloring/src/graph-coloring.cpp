#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "./utils/grafo.h"
#include "./utils/ler-arquivo.h"
#include "./utils/vector-utils.h"
#include "./utils/adj-list-file.h"
#include "./utils/salvar-arquivo.h"
// #include "./experimental/greedy-backtracking3.h"
#include "./algorithms/no-clique/greedy.h"
#include "./algorithms/no-clique/dsatur.h"
#include "./algorithms/no-clique/greedy-backtracking2.h"
#include "./algorithms/no-clique/dsatur-backtracking.h"
#include "./algorithms/no-clique/dsatur-sewell.h"
#include "./algorithms/no-clique/dsatur-pass-always.h"
#include "./algorithms/no-clique/dsatur-pass-conditional.h"
#include "./algorithms/no-clique/dsatur-gac.h"


using namespace std;
using namespace grafo;

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
        salvarArquivo::salvarEmArquivo("output/greedy-heuristic-output.js", grafo, cores);
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
        salvarArquivo::salvarEmArquivo("output/dsatur-heuristic-output.js", grafo, cores2);
    }

    else if (algorithm == "dsatur-backtracking") {
        int backtrackingVertices = 0;

        std::cout << "{\"dsatur-backtracking\": {\"colors\": ";
        /* Cria um arquivo log para o greedy-backtracking */
        fstream dsaturBacktrackingLog;
        /* TODO: Pasta deve estar criada para abertura do arquivo funcionar */
        dsaturBacktrackingLog.open("output/dsatur-backtracking-output.js", fstream::out);
        adjlist::adicionaListaDeAdjAoArquivo(dsaturBacktrackingLog, grafo);
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
        adjlist::adicionaListaDeAdjAoArquivo(dsaturSewellLog, grafo);
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
        adjlist::adicionaListaDeAdjAoArquivo(dsaturPassLog, grafo);
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
        adjlist::adicionaListaDeAdjAoArquivo(dsaturPassLog, grafo);
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
        adjlist::adicionaListaDeAdjAoArquivo(dsaturGACLog, grafo);
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
        adjlist::adicionaListaDeAdjAoArquivo(greedyBacktrackingLog, grafo);
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