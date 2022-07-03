#include "../../utils/debug.h"
#include "../heuristics/dsatur.h"
#include <set>
#include <map>
#include "../../csp/csp.h"
#include "../../csp/ac3.h"
#include "../../utils/clique.h"
#include <math.h>       /* floor */

#ifndef DSATURGACH
#define DSATURGACH

namespace dsaturGAC {

    using namespace std;

    map< string, vector<int> > getCSPDomains(vector<int>& ordenacao, vector<int>& coloracaoAtual, int upperBound) {
        map<string, vector<int>> domains;

        for (int i = 0; i < ordenacao.size(); i++) {
            int v = ordenacao[i];
            if (coloracaoAtual[v] != -1) {
                /* Adds a single color to the domain */
                domains.insert({ to_string(v), {coloracaoAtual[v]} });
            } else {
                vector<int> ordenacaoParcial = vectorUtils::copiarSubvetor(ordenacao, 0, i);
                /* Analyzes the color of each vertex in the partial ordering */
                vector<int> coloracaoParcial;
                for (int a: ordenacaoParcial) {
                    coloracaoParcial.push_back(coloracaoAtual[a]);
                }
                int ub = std::min(std::min(upperBound, i + 1), (grafo::obterTotalCores(coloracaoParcial) + 1));

                vector<int> values = vectorUtils::vetorCrescente(ub);

                domains.insert({ to_string(v), values });
            }
            
        }
        return domains;
    }

    vector<vector<string>> getCSPConstraints(grafo::Grafo& G) {
        vector<vector<string>> lines;
        for (int i = 0; i < G.listaAdj.size(); i++) {
            for (int j: G.listaAdj[i]) {
                if (i < j) {
                    vector<string> c {to_string(i), "!=", to_string(j)};
                    lines.push_back(c);
                }
            }
        }
        return lines;      
    }

    /* DSATUR with GAC Algorithm */
    vector<int> dsaturGAC (grafo::Grafo& G, fstream& logStream, int & backtrackingVertices) {

        /* Flag for enabling generalized arc consistency checks */
        bool gacEnabled = false;

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

        dsatur::reordenarProximoIndice(ordenacao, i, G, coloracaoAtual);            
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
            dsatur::reordenarProximoIndice(ordenacao, i, G, coloracaoAtual);

            /* This must be done before reordering tempCor, otherwise it could be that i < totalCores */
            /* Brown's tight coloring definition, which avoids searching in unnecessary branches (excludes permutations) */
            int boundary = min(k, grafo::obterTotalCores(coloracaoAtual) + 1);

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

            int totalCores = grafo::obterTotalCores(coloracaoAtual);
            DEBUG("{action: 'getColoringNumber', value: " + to_string(totalCores) + "}", logStream);       

            /* Stores whether all gac domains are not empty */ 
            bool gacValidDomains = true;

            /* Runs GAC to check if a full pruning is possible */
            /* k - totalCores is usually 0 (very common), 1 (common) or 2 (rare) */
            if (gacEnabled && (k - totalCores == 2 )) {
                /* Creates the CSP instance for the current state */
                map< string, vector<int> > domains = dsaturGAC::getCSPDomains(ordenacao, coloracaoAtual, k);
                vector< vector<string> > constraints = getCSPConstraints(G);
                csp::CSP test(domains, constraints);
                gacValidDomains = ac3::ac3(test);
            }

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
            /* Skip branch if some GAC domain is empty */
            else if (!gacValidDomains) {
                backtrackingVertices++;

                DEBUG("{action: 'preventSearchInSubBranches', value:'gacEmptyDomain "+ to_string(k) + " '} ", logStream);
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

                    /* Activates GAC after the first solution is found */
                    gacEnabled = true;
                }

            }
        }

        DEBUG("{action: 'finalResult', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);
        return melhorColoracao;
    }

}

#endif