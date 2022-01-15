#include "debug.h"

#ifndef GREEDYBACKTRACKING2H
#define GREEDYBACKTRACKING2H

namespace greedyBacktracking2 {

    using namespace std;

    /* Algoritmo Brown */
    /* Contém o Backtracking para uma ordem de vértices arbitrária */
    vector<int> greedyBacktracking2 (grafo::Grafo& G, fstream& logStream) {
        /* Associa indice do vertice com cor usada; Inicia com cor -1 (inexistente) */
        vector<int> coloracaoAtual(G.n, -1);
        debug::debug("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);   

        /* Guarda a melhor coloracao encontrada; Inicia com cor -1 (inexistente) */
        vector<int> melhorColoracao(G.n, -1);
        debug::debug("{action: 'set', key: 'melhorColoracao', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);     

        /* Guarda o numero de cores minimo atual; Inicia com n (pior caso) */
        int k = G.n; //grau maximo + 1 e um limitante; + DSATUR
        debug::debug("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);

        /* Todo grafo possui limitante superior grau maximo + 1 */
        int grauMaximo = grafo::grauMaximoVertices(G);
        debug::debug("{action: 'getMaximumDegree', value:" + to_string(grauMaximo) + "}", logStream);

        if (grauMaximo + 1 < k) {
            k = grauMaximo + 1;
            debug::debug("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);
        }


        /* ordenacao atual, comeca com [0, 1, ..., n - 1]; */
        vector<int> ordenacao = vectorUtils::vetorCrescente(G.n);
        debug::debug("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);

        /* Guarda a posicao atual dentro da ordenacao a ser analisada */
        int i = 0;
        debug::debug("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);      

        debug::debug("{action: 'finishInitialSetup'}", logStream); 


        /* Repete ate o final (i = -1) */
        while (i != -1) {
            debug::debug("{action: 'iteration', value: " + to_string(i) + "}", logStream);

            int indice = ordenacao[i];
            debug::debug("{action: 'set', key: 'indice', value: " + to_string(indice) + "}", logStream); 

            int tempCor = coloracaoAtual[ordenacao[i]];
            coloracaoAtual[ordenacao[i]] = -1;
            /* Definicao de tight coloring do Brown, para evitar buscas em branches desnecessárias (permutações) */
            int boundary = min(k, grafo::obterTotalCores(coloracaoAtual) + 1);
            coloracaoAtual[ordenacao[i]] = tempCor;             

            /* Tenta obter uma cor para o vertice */
            int cor = grafo::obterCorDisponivelParaVertice(G, coloracaoAtual, i, coloracaoAtual[i] + 1, boundary);
            debug::debug("{action: 'set', key: 'cor', value: " + to_string(cor) + "}", logStream);

            coloracaoAtual[indice] = cor;
            debug::debug("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

            int totalCores = grafo::obterTotalCores(coloracaoAtual);
            debug::debug("{action: 'getColoringNumber', value: " + to_string(totalCores) + "}", logStream);

            /* Se nenhuma cor é válida, é necessário voltar (backwards) */        
            if (cor == -1) {
                /* Remove coloracao atual do indice */
                debug::debug("{action: 'moveBackwards', value:'noValidColors'} ", logStream);

                i--;
                debug::debug("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);  

                /* Otimizacao, pois o primeiro vertice nao precisa ser testado com as cores restantes */
                if (i == 0) {
                    debug::debug("{action: 'stop'}", logStream);
                    break;
                } 
            } 
            /* Se coloracao é pior, pular a branch */
            else if (totalCores > k) {
                debug::debug("{action: 'preventSearchInSubBranches', value:'coloringWorseThanLimit "+ to_string(k) + " '} ", logStream);

                /* Ainda pode existir uma ou mais cores para o vertices melhores do que a combinacao atual, portanto
                deve-se aguardar mais uma iteracao no mesmo i (nao ha backtracking). 
                Esse if tambem ajuda a evitar o proximo caso, ou seja, para de explorar ou sub-ramos para a iteracao atual
                Caso esse else if seja comentado, a solucao final tera varias coloracoes nao-otimas */
            }
            else {
                /* Se tiver mais vertices, continua (forward) */
                if (i < G.n - 1) {
                    debug::debug("{action: 'moveForward'} ", logStream);                

                    i++;       
                    debug::debug("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);     

                } else {
                    /* Chegou em uma nova coloracao */
                    debug::debug("{action: 'foundColoring', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

                    melhorColoracao = vectorUtils::copiarVetor(coloracaoAtual);
                    debug::debug("{action: 'set', key: 'melhorColoracao', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);

                    k = totalCores - 1;
                    debug::debug("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);
                }
            }
        }

        debug::debug("{action: 'finalResult', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);
        return melhorColoracao;
    }

}

#endif