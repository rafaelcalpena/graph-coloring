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

    /* Brown's Algorithm */
    /* Uses backtracking for an arbitrarily ordered vertex sequence */
    vector<int> greedyBacktracking3 (grafo::Grafo& G, fstream& logStream, int & backtrackingVertices) {

        backtrackingVertices = 0;

        /* Associates used vertices and colors; Begins with -1 color (non-existent) */
        vector<int> coloracaoAtual(G.n, -1);  

        /* Stores best coloring found; Begins with -1 color (non-existent) */
        vector<int> melhorColoracao(G.n, -1);

        /* Stores the current minimum number of colors; Begins with n (worst-case) */
        int k = G.n; //grau maximo + 1 e um limitante; + DSATUR

        /* Every graph has an upper bound of maximum degree + 1 */
        int grauMaximo = grafo::grauMaximoVertices(G);

        if (grauMaximo + 1 < k) {
            k = grauMaximo + 1;
        }

        /* Current ordering, begins with [0, 1, ..., n - 1]; */
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

        /* Stores current position of ordering to be analyzed */
        int i = 0;
    
        /* Repeats until there are no more branches to be analyzed */
        while (i != -1) {

            /* Retrieve proper ordering depending on heuristic.
            For greedy, there's no difference between indice and i */
            int indice = ordenacao[i];

            /* Brown's tight coloring definition, which avoids searching in unnecessary branches (excludes permutations) */
            int upperBound = min(k, (maxColorUsed + 1) + 1);

            /* 
                Tries to obtain a current coloring for the vertex.
                A cor candidata não deverá ultrapassar o upper bound, senão retorna -1, obrigando o backtrack
            */            
            int cor = getNextCandidateColor(colorsInvalidated[indice], nextCandidateColor[indice], upperBound);
            if (cor >= k) {
                std::cout << "ERRO K" << endl;
                break;
            }
            
            nextCandidateColor[indice] = cor;

            coloracaoAtual[indice] = cor;

            /* Total number of colors */

            int totalCores = max(maxColorUsed + 1, cor + 1);

            /* If no colorings are valid, it is necessary to go backwards */        
            if ((cor == -1)) {

                if (totalCores >= k) {
                    backtrackingVertices++;
                }

                /* Go "upwards" */
                i--;

                /* Add back colors that had been crossed out in the last step */
                removeColor lastHistory = removalHistory[i];
                maxColorUsed = lastHistory.maxColorUsed;
                
                for (int v: lastHistory.vertices) {
                    remove_sorted(colorsInvalidated[v], lastHistory.color);
                };

                removalHistory.pop_back();                

                /* Optimization: first vertex will always be assigned a single color */
                if (i == 0) {
                    break;
                }
            }

            else if (totalCores > k) {
                backtrackingVertices++;
            }

            else {
                /* If there are more vertices, continue forward */
                backtrackingVertices++;

                /* Prepare history */
                removeColor historyItem;
                historyItem.color = cor;
                historyItem.maxColorUsed = maxColorUsed;

                if (binary_search(colorsInvalidated[i].begin(), colorsInvalidated[i].end(), cor)) {
                    std::cout << "ERRO" << endl;
                     break;
                }

                maxColorUsed = max(maxColorUsed, cor);

                /* Propagate colors to neighbors */
                for (int v: G.listaAdj[i]) {
                    /* Add for unvisited neighbors */
                    if (v > i) {

                        bool colorWasTaken = binary_search(colorsInvalidated[v].begin(), colorsInvalidated[v].end(), cor);

                       /* Adds items to removal history */                   
                        if (!colorWasTaken) {
                            historyItem.vertices.push_back(v);
                            auto itr2 = insert_sorted(colorsInvalidated[v], cor);
                        }
                    }
                };

                removalHistory.push_back(
                    historyItem
                );                

                if (i < G.n - 1) {
                    i++;         

                } else {
                    /* It has found a new coloring */
                    melhorColoracao = vectorUtils::copiarVetor(coloracaoAtual);
                    k = totalCores - 1;
                    removalHistory.pop_back();
                }
            }

        }

        return melhorColoracao;
    }

}

#endif