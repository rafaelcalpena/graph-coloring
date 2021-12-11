#include <iostream>
#include <vector>
#include <fstream>
#include "grafo.h"
#include "ler-arquivo.h"
#include "greedy.h"
#include "vector-utils.h"
#include "dsatur.h"
#include "greedy-backtracking.h"
#include "greedy-backtracking2.h"
#include "dsatur-backtracking.h"

using namespace std;
using namespace grafo;

/* Algoritmo Brélaz (DSATUR exato): */


/* Algoritmo SEWELL: */


/* Algoritmo PASS: */


/* Algoritmo do PGC */

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

    cout << "Analisando com Greedy:\n";
    vector<int> cores = greedy::greedy(grafo);
    grafo::verificaColoracao(grafo, cores);
    cout << "Cores totais: " << (*max_element(cores.begin(), cores.end()) + 1) << "\n";
    salvarEmArquivo("output/greedy-heuristic-output.js", grafo, cores);


    cout << "\n";
    cout << "Analisando com DSATUR:\n";
    vector<int> cores2 = dsatur::DSATUR(grafo);
    grafo::verificaColoracao(grafo, cores2);
    cout << "Cores totais: " << (*max_element(cores2.begin(), cores2.end()) + 1) << "\n";    
    salvarEmArquivo("output/dsatur-heuristic-output.js", grafo, cores2);


    cout << "\n";
    cout << "Analisando com Greedy Backtracking:\n";
    /* Cria um arquivo log para o greedy-backtracking */
    fstream greedyBacktrackingLog;
    /* TODO: Pasta deve estar criada para abertura do arquivo funcionar */
    greedyBacktrackingLog.open("output/greedy-backtracking-output.js", fstream::out);
    adicionaListaDeAdjAoArquivo(greedyBacktrackingLog, grafo);
    greedyBacktrackingLog << "logs = ";
    greedyBacktrackingLog << "[";
    /* Greedy backtracking 2 contem uma versão com variavel de ordenacao
    Greedy backtracking (1) é um pouco mais rápida */
    vector<int> cores3 = greedyBacktracking2::greedyBacktracking2(grafo, greedyBacktrackingLog);
    grafo::verificaColoracao(grafo, cores3);
    cout << "Cores totais: " << (*max_element(cores3.begin(), cores3.end()) + 1) << "\n";
    greedyBacktrackingLog << "]";
    greedyBacktrackingLog.close();


    cout << "\n";
    cout << "Analisando com DSATUR Backtracking:\n";
    /* Cria um arquivo log para o greedy-backtracking */
    fstream dsaturBacktrackingLog;
    /* TODO: Pasta deve estar criada para abertura do arquivo funcionar */
    dsaturBacktrackingLog.open("output/dsatur-backtracking-output.js", fstream::out);
    adicionaListaDeAdjAoArquivo(dsaturBacktrackingLog, grafo);
    dsaturBacktrackingLog << "logs = ";
    dsaturBacktrackingLog << "[";
    vector<int> cores4 = dsaturBacktracking::dsaturBacktracking(grafo, dsaturBacktrackingLog);
    grafo::verificaColoracao(grafo, cores4);
    cout << "Cores totais: " << (*max_element(cores4.begin(), cores4.end()) + 1) << "\n";
    dsaturBacktrackingLog << "]";
    dsaturBacktrackingLog.close();    

    return 0;
}