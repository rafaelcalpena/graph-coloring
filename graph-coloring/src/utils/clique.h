#ifndef CLIQUEH
#define CLIQUEH

#include "./grafo.h"

namespace clique {
    using namespace std;

    bool detectClique(grafo::Grafo & G, vector<int> & ordenacaoAtual, int i, int indice) {

        for (int j = 0; j < i; j++) {
            int jIndex = ordenacaoAtual[j];
            vector<int> adj = G.listaAdj[indice];
            /* If not in adjacency list, it is not a clique */
            
            if (find(adj.begin(), adj.end(), jIndex) == adj.end()) {
                return false;
            }
        }

        return true;
    }
}

#endif