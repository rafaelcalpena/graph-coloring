#ifndef CLIQUEH
#define CLIQUEH

#include "./grafo.h"

namespace clique {
    using namespace std;

    bool detectClique(grafo::Grafo & G, vector<int> & ordenacaoAtual, int i, int indice) {
        // cout << "detecting clique " << i << endl;

        for (int j = 0; j < i; j++) {
            int jIndex = ordenacaoAtual[j];
            vector<int> adj = G.listaAdj[indice];
            /* Se não estiver, na lista de adjacencia, nao e clique */
            
            if (find(adj.begin(), adj.end(), jIndex) == adj.end()) {
                return false;
            }
            // cout << "compared " << jIndex << " " << indice << endl;
        }

        // cout << "add to clique " << indice << endl;
        return true;
    }
}

#endif