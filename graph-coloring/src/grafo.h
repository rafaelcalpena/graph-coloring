#ifndef GRAFOH
#define GRAFOH

namespace grafo {
#include <vector>
    using namespace std;

    /* Carregar Grafo no formato DIMACS */
    struct Grafo {
        /* n representa o numero de vértices */
        int n = 0;
        /* m representa o numero de arestas */
        int m = 0;
        int grauMaximo = 0;
        vector<vector<int> > listaAdj;
        vector<int> graus;
    };

    int grauDoVertice(int indice, grafo::Grafo &G) {
        return G.listaAdj[indice].size();
    }

    /* Verificar */
    void verificaColoracao(grafo::Grafo& G, vector<int> cores) {
        for (int i = 0; i < G.n; i++) {
            vector<int> adj = G.listaAdj[i];
            for (int z = 0; z < adj.size(); z++) {
                if (cores[i] == cores[adj[z]]) {
                    cout << "Coloração Inválida!" << " " << i << " " << adj[z] << "\n";
                    break;
                }
            }
        }
    }

    int grauMaximoVertices(grafo::Grafo& G) {
        int max = 0;
        for (int i = 0; i < G.n; i++) {
            int grauVertice = G.listaAdj[i].size();
            if (grauVertice > max) {
                max = grauVertice;
            }
        }
        return max;
    }
}

#endif
