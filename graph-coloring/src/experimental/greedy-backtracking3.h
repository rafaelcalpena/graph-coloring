#include "../debug.h"
#include "./v3utils.h"

#ifndef GREEDYBACKTRACKING3H
#define GREEDYBACKTRACKING3H


void undoAssignments() {

}

struct removeColor {
    int color;
    int maxColorUsed;
    std::vector<int> vertices;
};

namespace greedyBacktracking3 {

    using namespace std;

    /* Algoritmo Brown */
    /* Contém o Backtracking para uma ordem de vértices arbitrária */
    vector<int> greedyBacktracking3 (grafo::Grafo& G, fstream& logStream, int & backtrackingVertices) {

        backtrackingVertices = 0;

        /* Associa indice do vertice com cor usada; Inicia com cor -1 (inexistente) */
        vector<int> coloracaoAtual(G.n, -1);  

        /* Guarda a melhor coloracao encontrada; Inicia com cor -1 (inexistente) */
        vector<int> melhorColoracao(G.n, -1);

        /* Guarda o numero de cores minimo atual; Inicia com n (pior caso) */
        int k = G.n; //grau maximo + 1 e um limitante; + DSATUR

        /* Todo grafo possui limitante superior grau maximo + 1 */
        int grauMaximo = grafo::grauMaximoVertices(G);

        if (grauMaximo + 1 < k) {
            k = grauMaximo + 1;
        }

        /* ordenacao atual, comeca com [0, 1, ..., n - 1]; */
        vector<int> ordenacao = vectorUtils::vetorCrescente(G.n);
        vector<int> ordenacaoInversa = vectorUtils::vetorCrescente(G.n);

        /* Keeps list of invalidated colors by propagation */
        vector< vector<int> > colorsInvalidated = initColorsInvalidated(G);
        /* Shortcut to avoid traversing the vector above */
        vector<int> nextCandidateColor(G.n, -1);

        /* Max color used up to the current iteration level */
        int maxColorUsed = -1;

        /* Initial size must be 0 */
        vector<removeColor> removalHistory;

        /* Guarda a posicao atual dentro da ordenacao a ser analisada */
        int i = 0;
    
        /* Repete ate o final (i = -1) */
        while (i != -1) {

                // std::cout << "removalHistory size " << removalHistory.size() << " i " << i << endl;
                if (removalHistory.size() != i) {
                    std::cout << "ERRO SIZE" << endl;
                     break;
                }            

            // std::cout << "i " << i;

            /* Retrieve proper ordering depending on heuristic.
            For greedy, there's no difference between indice and i */
            int indice = ordenacao[i];

            /* Definicao de tight coloring do Brown, para evitar buscas em branches desnecessárias (permutações) */
            int upperBound = min(k, (maxColorUsed + 1) + 1);

            /* 
                Tenta obter uma cor para o vertice.
                A cor candidata não deverá ultrapassar o upper bound, senão retorna -1, obrigando o backtrack
            */            
            int cor = getNextCandidateColor(colorsInvalidated[indice], nextCandidateColor[indice], upperBound);
            if (cor >= k) {
                std::cout << "ERRO K" << endl;
                break;
            }
            
            // std::cout << ", cor: " << cor << endl;
            nextCandidateColor[indice] = cor;

            coloracaoAtual[indice] = cor;

            /* Total number of colors */
            /* TODO: Looks weird to have 2 similar vars here */
            int totalCores = max(maxColorUsed + 1, cor + 1);
            // std::cout << "totalCores = " << totalCores << endl;

            /* Se nenhuma cor é válida, é necessário voltar (backwards) */        
            if ((cor == -1)) {

                if (totalCores >= k) {
                    backtrackingVertices++;
                }

                /* Go "upwards" */
                i--;

                /* Adiciona de volta as cores que haviam sido canceladas no ultimo passo */
                removeColor lastHistory = removalHistory[i];
                maxColorUsed = lastHistory.maxColorUsed;

                //std::cout << "desfaz c:" << lastHistory.color << endl;
                //std::cout << "vetor";
                // vectorUtils::imprimeVetor(lastHistory.vertices);
                
                for (int v: lastHistory.vertices) {
                    //std::cout << "- v: " << v << ", reenable: " << lastHistory.color;
                    remove_sorted(colorsInvalidated[v], lastHistory.color);
                };

                /* TODO: Might not be necessary */
                removalHistory.pop_back();                

                /* Otimizacao, pois o primeiro vertice nao precisa ser testado com as cores restantes */
                if (i == 0) {
                    break;
                }
            }

            else if (totalCores > k) {
                backtrackingVertices++;
            }

            else {
                /* Se tiver mais vertices, continua (forward) */
                backtrackingVertices++;

                /* Prepara o historico */
                removeColor historyItem;
                historyItem.color = cor;
                historyItem.maxColorUsed = maxColorUsed;

                /* TODO: Check should not be necessary */
                if (binary_search(colorsInvalidated[i].begin(), colorsInvalidated[i].end(), cor)) {
                    std::cout << "ERRO" << endl;
                     break;
                }

                maxColorUsed = max(maxColorUsed, cor);

                /* Propaga as cores para os vizinhos */
                for (int v: G.listaAdj[i]) {
                    /* TODO: Usar ordenacao inversa */                      
                    /* Apenas adiciona para vizinhos nao explorados */
                    if (v > i) {

                        bool colorWasTaken = binary_search(colorsInvalidated[v].begin(), colorsInvalidated[v].end(), cor);

                       /* Adiciona items no removal history apenas quando realmente adicionar remocao */                   
                        if (!colorWasTaken) {
                            //std::cout << "- insert v: " << v << ", c: " << cor << endl;
                            historyItem.vertices.push_back(v);

                            auto itr2 = insert_sorted(colorsInvalidated[v], cor);
                        }
                    }
                };

                removalHistory.push_back(
                    historyItem
                );                
                //std::cout << "removal history size " << removalHistory.size() << endl;

                if (i < G.n - 1) {
                    i++;         

                } else {
                    //std:://cout << "Found coloring" << endl;
                    /* Chegou em uma nova coloracao */
                    melhorColoracao = vectorUtils::copiarVetor(coloracaoAtual);
                    // std::cout << "melhor ";
                    // vectorUtils::imprimeVetor(melhorColoracao);
                    //std::cout << endl;
                    k = totalCores - 1;
                    // std::cout << "totalCores = " << totalCores << endl;
                    // std::cout << " k = " << k << endl;
                    /* Importante */
                    removalHistory.pop_back();
                }
            }

        }

        return melhorColoracao;
    }

}

#endif