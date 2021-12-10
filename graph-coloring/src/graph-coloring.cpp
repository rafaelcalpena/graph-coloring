#include <iostream>
#include <vector>
#include <fstream>
#include "grafo.h"
#include "ler-arquivo.h"
#include "greedy.h"
#include "vector-utils.h"
#include "dsatur.h"

using namespace std;
using namespace grafo;


int obterTotalCores (vector<int> coloracao) {
    vector<bool> coresUsadas(coloracao.size(), false);
    int total = 0;

    for (int cor: coloracao) {
        if (cor >= 0) {
            coresUsadas[cor] = true;
        };

    }

    for (bool c: coresUsadas) {
        if (c == true) {
            total++;
        }
    }    
    return total;
}


void debug (std::string message, fstream& logFile) {
    const std::string flag = getenv("DEBUG");
    if (flag == "2") {
        cout << message << "\n";
    }
    if (flag == "1" || flag == "2") {
        logFile << message << ",\n";
    }
}


/* Algoritmo Brown */
/* Contém o Backtracking para uma ordem de vértices arbitrária */
vector<int> greedyBacktracking (grafo::Grafo& G, fstream& logStream) {
    /* Associa indice do vertice com cor usada; Inicia com cor -1 (inexistente) */
    vector<int> coloracaoAtual(G.n, -1);
    debug("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);   

    /* Guarda a melhor coloracao encontrada; Inicia com cor -1 (inexistente) */
    vector<int> melhorColoracao(G.n, -1);
    debug("{action: 'set', key: 'melhorColoracao', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);     

    /* Guarda o numero de cores minimo atual; Inicia com n (pior caso) */
    int k = G.n; //grau maximo + 1 e um limitante; + DSATUR
    debug("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);

    /* Todo grafo possui limitante superior grau maximo + 1 */
    int grauMaximo = grafo::grauMaximoVertices(G);
    debug("{action: 'getMaximumDegree', value:" + to_string(grauMaximo) + "}", logStream);

    if (grauMaximo + 1 < k) {
        k = grauMaximo + 1;
        debug("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);
    }

    /* Guarda a posicao atual dentro da ordenacao a ser analisada */
    int i = 0;
    debug("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);      

    debug("{action: 'finishInitialSetup'}", logStream); 


    /* Repete ate o final (i = -1) */
    while (i != -1) {
        debug("{action: 'iteration', value: " + to_string(i) + "}", logStream);

        /* Tenta obter uma cor para o vertice */
        int cor = obterCorExistenteParaVertice(G, coloracaoAtual, i, coloracaoAtual[i] + 1, k);
        debug("{action: 'set', key: 'cor', value: " + to_string(cor) + "}", logStream);

        coloracaoAtual[i] = cor;
        debug("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

        int totalCores = obterTotalCores(coloracaoAtual);
        debug("{action: 'getColoringNumber', value: " + to_string(totalCores) + "}", logStream);

        /* Se nenhuma cor é válida, é necessário voltar (backwards) */        
        if (cor == -1) {
            /* Remove coloracao atual do indice */
            debug("{action: 'moveBackwards', value:'noValidColors'} ", logStream);

            i--;
            debug("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);  

            /* Otimizacao, pois o primeiro vertice nao precisa ser testado com as cores restantes */
            if (i == 0) {
                debug("{action: 'stop'}", logStream);
                break;
            } 
        } 
        /* Se coloracao não é melhor, é necessário voltar (backwards) */
        else if (totalCores > k) {
            debug("{action: 'preventSearchInSubBranches', value:'coloringWorseThanLimit "+ to_string(k) + " '} ", logStream);

            /* Ainda pode existir uma ou mais cores para o vertices melhores do que a combinacao atual, portanto
            deve-se aguardar mais uma iteracao no mesmo i (nao ha backtracking). 
            Esse if tambem ajuda a evitar o proximo caso, ou seja, para de explorar ou sub-ramos para a iteracao atual
            Caso esse else if seja comentado, a solucao final tera varias coloracoes nao-otimas */
        }
        else {
            /* Se tiver mais vertices, continua (forward) */
            if (i < G.n - 1) {
                debug("{action: 'moveForward'} ", logStream);                

                i++;       
                debug("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);     

            } else {
                /* Chegou em uma nova coloracao */
                debug("{action: 'foundColoring', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

                melhorColoracao = vectorUtils::copiarVetor(coloracaoAtual);
                debug("{action: 'set', key: 'melhorColoracao', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);

                k = totalCores - 1;
                debug("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);
            }
        }
    }

    debug("{action: 'finalResult', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);
    return melhorColoracao;
}

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
    vector<int> cores3 = greedyBacktracking(grafo, greedyBacktrackingLog);
    grafo::verificaColoracao(grafo, cores3);
    cout << "Cores totais: " << (*max_element(cores3.begin(), cores3.end()) + 1) << "\n";
    greedyBacktrackingLog << "]";
    greedyBacktrackingLog.close();

    return 0;
}