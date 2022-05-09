#include "../../utils/debug.h"
#include "../heuristics/dsatur.h"
#include <set>

#ifndef DSATURPASSCONDITIONALH
#define DSATURPASSCONDITIONALH

int THRESHOLD = 3;

namespace dsaturPassConditional {

    using namespace std;


    /* Reordenação adaptada a heurística Pass
    Como criterio de desempate não utiliza grau do vértice */
    int reordenarProximoIndice(vector<int>& ordenacao, int i, grafo::Grafo& G, vector<int> vCores, int k, fstream& logStream, int totalCores) {

        /* Reordenação: */
        /* Encontrar o item com maior grau de saturacao na lista de adjacencia */
        int indiceVencedor = i;
        int vencedorSat = dsatur::grauDeSaturacao(ordenacao[indiceVencedor], G, vCores);

        /* Separa os indices de vértices empatados */
        set<int> tied;
        DEBUG("{action: 'get_tied', value: " + to_string(tied.size()) + " }", logStream);

        for (int prox = i+1; prox < G.n; prox++) {
            int proxSat = dsatur::grauDeSaturacao(ordenacao[prox], G, vCores);

            if (proxSat > vencedorSat) {

                indiceVencedor = prox;
                vencedorSat = proxSat;

                tied.clear();
                tied.insert(ordenacao[prox]);

            }
            else if (proxSat == vencedorSat) {
                DEBUG("{action: 'tie' , key: " + to_string(ordenacao[prox]) + ", value: " + to_string(ordenacao[indiceVencedor]) + "}", logStream);

                tied.insert(ordenacao[prox]);
            }
        }

        /* mi = numero de cores na coloracao - grau de saturacao dos vertices empatados */
        /* totalCores - proxSat */
        int mi = totalCores - vencedorSat;

        if (mi > THRESHOLD) {
            return dsatur::reordenarProximoIndice(ordenacao, i, G, vCores);
        }

        /* mi <= THRESHOLD então usar Pass */
        /* Guarda o valor máximo da regra Pass obtida, comecando com o indice vencedor */
        int empateMax = dsaturPassAlways::passRule(ordenacao[indiceVencedor], G, vCores, tied, k, logStream);

        for (int prox = i+1; prox < G.n; prox++) {
            int proxSat = dsatur::grauDeSaturacao(ordenacao[prox], G, vCores);

            if (proxSat == vencedorSat) {
                /* Desempate pela regra Pass */
                int passProx = dsaturPassAlways::passRule(ordenacao[prox], G, vCores, tied, k, logStream);
                /* Para cada empate, verificar se passRule para o vertice 
                em analise supera passRule para o vencedor atual.
                Se superar, o candidato se torna o novo vencedor e atualiza o recorde pass 
                */
                if (
                    passProx > empateMax
                ) {
                    indiceVencedor = prox;
                    empateMax = passProx;
                }
            }
        }

        /* Reordena apenas o item atual em DSATUR */

        int indice = ordenacao[indiceVencedor];

        vectorUtils::trocar(ordenacao, i, indiceVencedor);

        return indice;
    }


    /* Algoritmo Brélaz */
    /* Contém o Backtracking para uma ordem de vértices arbitrária */
    vector<int> dsaturPassConditional (grafo::Grafo& G, fstream& logStream, int & backtrackingVertices) {
        backtrackingVertices = 0;

        /* Associa indice do vertice com cor usada; Inicia com cor -1 (inexistente) */
        vector<int> coloracaoAtual(G.n, -1);
        DEBUG("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);   

        /* Guarda a melhor coloracao encontrada; Inicia com cor -1 (inexistente) */
        vector<int> melhorColoracao(G.n, -1);
        DEBUG("{action: 'set', key: 'melhorColoracao', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);     

        /* Guarda o numero de cores minimo atual; Inicia com n (pior caso) */
        int k = G.n; //grau maximo + 1 é um limitante
        DEBUG("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);

        /* Todo grafo possui limitante superior grau maximo + 1 */
        int grauMaximo = grafo::grauMaximoVertices(G);
        DEBUG("{action: 'getMaximumDegree', value:" + to_string(grauMaximo) + "}", logStream);

        if (grauMaximo + 1 < k) {
            k = grauMaximo + 1;
            DEBUG("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);
        }

        /* ordenacao atual, comeca com [0, 1, ..., n - 1]; */
        vector<int> ordenacao = vectorUtils::vetorCrescente(G.n);
        DEBUG("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);

        /* Guarda a posicao atual dentro da ordenacao a ser analisada */
        int i = 0;
        DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream); 

        int totalCores = grafo::obterTotalCores(coloracaoAtual);

        /* Diferença para o DSATUR original */
        dsaturPassConditional::reordenarProximoIndice(ordenacao, i, G, coloracaoAtual, k, logStream, totalCores);            

        DEBUG("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);             

        DEBUG("{action: 'finishInitialSetup'}", logStream); 

        /* Repete ate o final (i = -1) */
        while (i != -1) {
            DEBUG("{action: 'iteration', value: " + to_string(i) + "}", logStream);

            /* TODO: Usar coloracaoAtual ao inves de tempColoracao */
            /* Para que a ordenacao seja feita de forma correta, é necessário ignorar a coloracao atual para o index i */
            int tempCor = coloracaoAtual[ordenacao[i]];
            coloracaoAtual[ordenacao[i]] = -1;

            totalCores = grafo::obterTotalCores(coloracaoAtual);

            /* Diferença para o DSATUR original */
            dsaturPassConditional::reordenarProximoIndice(ordenacao, i, G, coloracaoAtual, k, logStream, totalCores);

            /* Precisa ser feita antes de retornar tempCor no DSATUR, senao seria possível que i < totalCores */
            /* Definicao de tight coloring do Brown, para evitar buscas em branches desnecessárias (permutações) */
            int boundary = min(k, totalCores + 1);
            coloracaoAtual[ordenacao[i]] = tempCor;            
            DEBUG("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);

            int indice = ordenacao[i];
            DEBUG("{action: 'set', key: 'indice', value: " + to_string(indice) + "}", logStream); 

            /* Tenta obter uma cor para o vertice */
            int cor = grafo::obterCorDisponivelParaVertice(G, coloracaoAtual, indice, coloracaoAtual[indice] + 1, boundary);
            DEBUG("{action: 'set', key: 'cor', value: " + to_string(cor) + "}", logStream);

            coloracaoAtual[indice] = cor;
            DEBUG("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

            totalCores = grafo::obterTotalCores(coloracaoAtual);
            DEBUG("{action: 'getColoringNumber', value: " + to_string(totalCores) + "}", logStream);          

            /* Se nenhuma cor é válida, é necessário voltar (backwards) */        
            if (cor == -1) {
                /* Remove coloracao atual do indice */
                DEBUG("{action: 'moveBackwards', value:'noValidColors'} ", logStream);

                i--;
                DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);  

                /* Otimizacao, pois o primeiro vertice nao precisa ser testado com as cores restantes */
                if (i == 0) {
                    DEBUG("{action: 'stop'}", logStream);
                    break;
                } 
            } 
            /* Se coloracao é pior, pular a branch */
            else if (totalCores > k) {
                backtrackingVertices++;

                DEBUG("{action: 'preventSearchInSubBranches', value:'coloringWorseThanLimit "+ to_string(k) + " '} ", logStream);

                /* Ainda pode existir uma ou mais cores para o vertices melhores do que a combinacao atual, portanto
                deve-se aguardar mais uma iteracao no mesmo i (nao ha backtracking). 
                Esse if tambem ajuda a evitar o proximo caso, ou seja, para de explorar ou sub-ramos para a iteracao atual
                Caso esse else if seja comentado, a solucao final tera varias coloracoes nao-otimas */
            }
            else {
                /* Se tiver mais vertices, continua (forward) */
                backtrackingVertices++;

                if (i < G.n - 1) {
                    DEBUG("{action: 'moveForward'} ", logStream);                

                    i++;       
                    DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);     

                } else {
                    /* Chegou em uma nova coloracao */
                    DEBUG("{action: 'foundColoring', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

                    melhorColoracao = vectorUtils::copiarVetor(coloracaoAtual);
                    DEBUG("{action: 'set', key: 'melhorColoracao', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);

                    k = totalCores - 1;
                    DEBUG("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);
                }
            }
        }

        DEBUG("{action: 'finalResult', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);
        return melhorColoracao;
    }

}

#endif