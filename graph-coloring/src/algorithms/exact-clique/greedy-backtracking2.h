#include "../../utils/debug.h"
#include "../../utils/clique.h"

#ifndef GREEDYBACKTRACKING2H
#define GREEDYBACKTRACKING2H

namespace greedyBacktracking2 {

    using namespace std;

    /* Uses backtracking for an arbitrarily ordered vertex sequence */
    vector<int> greedyBacktracking2 (grafo::Grafo& G, fstream& logStream, int & backtrackingVertices) {

        backtrackingVertices = 0;

        /* Associates used vertices and colors; Begins with -1 color (non-existent) */
        vector<int> coloracaoAtual(G.n, -1);
        DEBUG("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);   

        /* Stores best coloring found; Begins with -1 color (non-existent) */
        vector<int> melhorColoracao(G.n, -1);
        DEBUG("{action: 'set', key: 'melhorColoracao', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);     

        /* Stores the current minimum number of colors; Begins with n (worst-case) */
        int k = G.n; 
        DEBUG("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);

        /* Every graph has an upper bound of maximum degree + 1 */
        int grauMaximo = grafo::grauMaximoVertices(G);
        DEBUG("{action: 'getMaximumDegree', value:" + to_string(grauMaximo) + "}", logStream);

        if (grauMaximo + 1 < k) {
            k = grauMaximo + 1;
            DEBUG("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);
        }

        /* Current ordering, begins with [0, 1, ..., n - 1]; */
        vector<int> ordenacao = vectorUtils::vetorCrescente(G.n);
        DEBUG("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);

        /* Stores current position of ordering to be analyzed */
        int i = 0;
        DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);

        int cliqueIndex = -1;
        bool enableCliqueSearch = true;

        DEBUG("{action: 'finishInitialSetup'}", logStream); 

        /* Repeats until there are no more branches to be analyzed */
        while (i != cliqueIndex) {
            DEBUG("{action: 'iteration', value: " + to_string(i) + "}", logStream);

            int indice = ordenacao[i];
            DEBUG("{action: 'set', key: 'indice', value: " + to_string(indice) + "}", logStream); 

            /* Checks if the current vertex belongs to the first clique found */
            if (enableCliqueSearch) {
                if (clique::detectClique(G, ordenacao, i, indice)) {
                    cliqueIndex = i;
                } else {
                    enableCliqueSearch = false;
                }
            }

            int tempCor = coloracaoAtual[ordenacao[i]];
            coloracaoAtual[ordenacao[i]] = -1;
            /* Brown's tight coloring definition, which avoids searching in unnecessary branches (excludes permutations) */
            int boundary = min(k, grafo::obterTotalCores(coloracaoAtual) + 1);
            coloracaoAtual[ordenacao[i]] = tempCor;             

            /* Tries to obtain a current coloring for the vertex */
            int cor = grafo::obterCorDisponivelParaVertice(G, coloracaoAtual, i, coloracaoAtual[i] + 1, boundary);
            DEBUG("{action: 'set', key: 'cor', value: " + to_string(cor) + "}", logStream);

            coloracaoAtual[indice] = cor;
            DEBUG("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

            int totalCores = grafo::obterTotalCores(coloracaoAtual);
            DEBUG("{action: 'getColoringNumber', value: " + to_string(totalCores) + "}", logStream);

            /* If no colorings are valid, it is necessary to go backwards */        
            if (cor == -1) {
                /* Removes current index coloring */
                DEBUG("{action: 'moveBackwards', value:'noValidColors'} ", logStream);

                i--;
                DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);  

                /* Optimization: first vertex will always be assigned a single color */
                if (i == cliqueIndex) {
                    DEBUG("{action: 'stop'}", logStream);
                    break;
                } 
            } 
            /* If coloring found is worse, skip branch */
            else if (totalCores > k) {
                backtrackingVertices++;

                DEBUG("{action: 'preventSearchInSubBranches', value:'coloringWorseThanLimit "+ to_string(k) + " '} ", logStream);

                /* There could stil be one or more better colorings for the vertices, so 
                we should wait for one more iteration in the same i vertex (there is no backtracking in this case) 
                This if condition also stops exploring sub-branches for the current iteration, 
                and when uncommented will lead to many more non-optimal solutions being found.  */
            }
            else {
                /* If there are more vertices, continue forward */
                backtrackingVertices++;

                if (i < G.n - 1) {
                    DEBUG("{action: 'moveForward'} ", logStream);                

                    i++;       
                    DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);     

                } else {
                    /* It has found a new coloring */
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