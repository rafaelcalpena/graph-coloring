#include "../../utils/debug.h"
#include "../heuristics/dsatur.h"
#include "../../utils/clique.h"
#include <set>

#ifndef DSATURPASSCONDITIONALH
#define DSATURPASSCONDITIONALH

int THRESHOLD = 3;

namespace dsaturPassConditional {

    using namespace std;

    /* Break ties by choosing vertex with the maximum number of 
    common available colors in the neighborhood of tied vertices. */
    int passRule(int vertexIndex, grafo::Grafo& G, vector<int> & cores, set<int> & tied, int k, fstream& logStream) {
        DEBUG("{action: 'begin_pass', value: " + to_string(vertexIndex) + "}", logStream);

        /*  Get available colors for tied vertex */
        set<int> coresDisponiveis = grafo::obterCoresDisponiveisParaVertice(G, cores, vertexIndex, k);
        DEBUG("{action: 'get_available_colors', key: " + to_string(vertexIndex) + ", value: " + vectorUtils::serializarSet(coresDisponiveis) + " }", logStream);

        int sum = 0;

        for (int pos: G.listaAdj[vertexIndex]) {

            /* Only apply for tied */
            if (tied.find(pos) == tied.end()) {
                continue;
            }

            set<int> coresDisponiveisVizinho = grafo::obterCoresDisponiveisParaVertice(G, cores, pos, k);
            DEBUG("{action: 'get_available_colors', key: " + to_string(pos) + ", value: " + vectorUtils::serializarSet(coresDisponiveisVizinho) + " }", logStream);


            /* set_intersection requires a vector */
            vector<int> intersection;

            set_intersection(
                coresDisponiveis.begin(),
                coresDisponiveis.end(),
                coresDisponiveisVizinho.begin(),
                coresDisponiveisVizinho.end(), 
                std::back_inserter(intersection)
            );
            DEBUG("{action: 'get_intersection', value: " + to_string(intersection.size()) + " }", logStream);
            sum += intersection.size();
        }

        /* for each of its neighbors in the tied set
            sum the length of intersection between both sets of available colorings */

        DEBUG("{action: 'end_pass', key: " + to_string(vertexIndex) + ", value: " + to_string(sum) +  "}", logStream);
        return sum;
    }

    /* Reordering adapted to Pass heuristic
    Does not use vertex degree for breaking ties */
    int reordenarProximoIndice(vector<int>& ordenacao, int i, grafo::Grafo& G, vector<int> vCores, int k, fstream& logStream, int totalCores) {

        /* Reordering:*/
        /* Find item with highest saturation degree in adjacency list */
        int indiceVencedor = i;
        int vencedorSat = dsatur::grauDeSaturacao(ordenacao[indiceVencedor], G, vCores);

        /* Separates original vertex indices that have not been colored */
        set<int> tied;
        DEBUG("{action: 'get_tied', value: " + to_string(tied.size()) + " }", logStream);

        /* Stores the maximum value for Pass rule, beginning with value for winner index */
        int empateMax = passRule(ordenacao[indiceVencedor], G, vCores, tied, k, logStream);


        for (int prox = i+1; prox < G.n; prox++) {
            int proxSat = dsatur::grauDeSaturacao(ordenacao[prox], G, vCores);

            /* mi = number of colors in coloring - saturation degree for tied vertices */
            int mi = totalCores - proxSat;

            if (proxSat > vencedorSat) {

                indiceVencedor = prox;
                vencedorSat = proxSat;

                tied.clear();

                empateMax = passRule(ordenacao[prox], G, vCores, tied, k, logStream);

                tied.insert(ordenacao[prox]);

            } else if (proxSat == vencedorSat) {
                DEBUG("{action: 'tie' , key: " + to_string(ordenacao[prox]) + ", value: " + to_string(ordenacao[indiceVencedor]) + "}", logStream);

                tied.insert(ordenacao[prox]);

                if (mi <= THRESHOLD) {
                    /* Break ties using Pass rule */
                    int passProx = passRule(ordenacao[prox], G, vCores, tied, k, logStream);
                    /* Verify if pass rule for each tied vertex occurrence
                    is greater than pass rule result for current winner.
                    If positive, then candidate becomes the new winner
                    */
                    if (
                        passProx > empateMax
                    ) {
                        DEBUG("{action: 'use_pass_heuristic'}", logStream);
                        indiceVencedor = prox;
                        empateMax = passProx;
                    }
                } else {
                    /* Break ties by vertex degree, and keep vertices in ascending order */
                    if (
                        ((proxSat == vencedorSat) && (grauDoVertice(ordenacao[prox], G) > grauDoVertice(ordenacao[indiceVencedor], G))) ||
                        /* Keep vertices in ascending order */
                        ((proxSat == vencedorSat) && (grauDoVertice(ordenacao[prox], G) == grauDoVertice(ordenacao[indiceVencedor], G)) && (ordenacao[prox] < ordenacao[indiceVencedor]))                        
                    ) {
                        DEBUG("{action: 'use_dsatur_heuristic'}", logStream);
                        indiceVencedor = prox;                      
                    }
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

        int cliqueIndex = -1;
        bool enableCliqueSearch = true;

        DEBUG("{action: 'finishInitialSetup'}", logStream); 

        /* Repeats until there are no more branches to be analyzed */
        while (i != cliqueIndex) {
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

            if (enableCliqueSearch) {
                if (clique::detectClique(G, ordenacao, i, indice)) {
                    cliqueIndex = i;
                } else {
                    enableCliqueSearch = false;
                }
            }

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