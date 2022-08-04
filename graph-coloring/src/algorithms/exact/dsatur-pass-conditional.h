#include "../../utils/debug.h"
#include "../heuristics/dsatur.h"
#include <set>

#ifndef DSATURPASSCONDITIONALH
#define DSATURPASSCONDITIONALH

int THRESHOLD = 3;

namespace dsaturPassConditional {

    using namespace std;


    /* Reordering adapted to Pass heuristic
    Does not use vertex degree for breaking ties */
    int reordenarProximoIndice(vector<int>& ordenacao, int i, grafo::Grafo& G, vector<int> vCores, int k, fstream& logStream, int totalCores) {

        /* Reordering:*/
        /* Find item with highest saturation degree in adjacency list */
        int indiceVencedor = i;
        int vencedorSat = dsatur::grauDeSaturacao(ordenacao[indiceVencedor], G, vCores);

        /* Separates indices for tied vertices */
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

        /* mi = number of colors in coloring - saturation degree for tied vertices */
        int mi = totalCores - vencedorSat;

        if (mi > THRESHOLD) {
            return dsatur::reordenarProximoIndice(ordenacao, i, G, vCores);
        }

        /* if mi <= THRESHOLD then use Pass */
        /* Stores the maximum value for Pass rule, beginning with value for winner index */
        int empateMax = dsaturPassAlways::passRule(ordenacao[indiceVencedor], G, vCores, tied, k, logStream);

        for (int prox = i+1; prox < G.n; prox++) {
            int proxSat = dsatur::grauDeSaturacao(ordenacao[prox], G, vCores);

            if (proxSat == vencedorSat) {
                /* Break ties using Pass rule */
                int passProx = dsaturPassAlways::passRule(ordenacao[prox], G, vCores, tied, k, logStream);
                /* Verify if pass rule for each tied vertex occurrence
                is greater than pass rule result for current winner.
                If positive, then candidate becomes the new winner
                */
                if (
                    passProx > empateMax
                ) {
                    indiceVencedor = prox;
                    empateMax = passProx;
                }
            }
        }

        /* Only reorder current item for DSATUR */

        int indice = ordenacao[indiceVencedor];

        vectorUtils::trocar(ordenacao, i, indiceVencedor);

        return indice;
    }


    /* Uses DSATUR Pass conditionally based on "mi" parameter */
    vector<int> dsaturPassConditional (grafo::Grafo& G, fstream& logStream, int & backtrackingVertices) {
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

        int totalCores = grafo::obterTotalCores(coloracaoAtual);

        /* Different than original DSATUR */
        dsaturPassConditional::reordenarProximoIndice(ordenacao, i, G, coloracaoAtual, k, logStream, totalCores);            

        DEBUG("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);             

        DEBUG("{action: 'finishInitialSetup'}", logStream); 

        /* Repeats until there are no more branches to be analyzed */
        while (i != -1) {
            DEBUG("{action: 'iteration', value: " + to_string(i) + "}", logStream);


            /* It is necessary to ignore the current coloring for i so that reordering vertices is done correctly */
            int tempCor = coloracaoAtual[ordenacao[i]];
            coloracaoAtual[ordenacao[i]] = -1;

            totalCores = grafo::obterTotalCores(coloracaoAtual);

            /* Different than original DSATUR */
            dsaturPassConditional::reordenarProximoIndice(ordenacao, i, G, coloracaoAtual, k, logStream, totalCores);

            /* This must be done before reordering tempCor, otherwise it could be that i < totalCores */
            /* Brown's tight coloring definition, which avoids searching in unnecessary branches (excludes permutations) */
            int boundary = min(k, totalCores + 1);
            coloracaoAtual[ordenacao[i]] = tempCor;            
            DEBUG("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);

            int indice = ordenacao[i];
            DEBUG("{action: 'set', key: 'indice', value: " + to_string(indice) + "}", logStream); 

            /* Tries to obtain a current coloring for the vertex */
            int cor = grafo::obterCorDisponivelParaVertice(G, coloracaoAtual, indice, coloracaoAtual[indice] + 1, boundary);
            DEBUG("{action: 'set', key: 'cor', value: " + to_string(cor) + "}", logStream);

            coloracaoAtual[indice] = cor;
            DEBUG("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

            totalCores = grafo::obterTotalCores(coloracaoAtual);
            DEBUG("{action: 'getColoringNumber', value: " + to_string(totalCores) + "}", logStream);          

            /* If no colorings are valid, it is necessary to go backwards */        
            if (cor == -1) {
                /* Removes current index coloring */
                DEBUG("{action: 'moveBackwards', value:'noValidColors'} ", logStream);

                i--;
                DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);  

                /* Optimization: first vertex will always be assigned a single color */
                if (i == 0) {
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