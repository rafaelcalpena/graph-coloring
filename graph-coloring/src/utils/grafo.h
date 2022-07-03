#ifndef GRAFOH
#define GRAFOH

#include <set>

namespace grafo {
#include <vector>
    using namespace std;

    /* Loads graph from DIMACS format */
    struct Grafo {
        /* n represents the number of vertices */
        int n = 0;
        /* m represents the number of edges */
        int m = 0;
        int grauMaximo = 0;
        vector<vector<int> > listaAdj;
        vector<int> graus;
    };

    int grauDoVertice(int indice, grafo::Grafo &G) {
        return G.listaAdj[indice].size();
    }

    /* Checks that some given coloring is valid */
    void verificaColoracao(grafo::Grafo& G, vector<int> & cores) {
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

    int obterCorDisponivelParaVertice(grafo::Grafo& G, vector<int> & coloracaoAtual, int i, int from, int to) {
        /* List that indicates if some color has already been used
        In the worst case n colors will be required */
        vector<bool> coresUsadas(to, false);
        vector<int> adj = G.listaAdj[i];

        /* We loop through the adjacency list and select which colors may be used */
        for (int z = 0; z < adj.size(); z++)
        {
            int j = adj[z];

            /* Assigns used color */
            if (coloracaoAtual[j] != -1)
            {
                coresUsadas[coloracaoAtual[j]] = true;
            }
        }

        /* Looks for a color that has not been used yet */
        for (int c = from; c < to; c++) {
            if (coresUsadas[c] == false) {
                return c;
            }
        }

        /* Return -1 is there are no valid colors */
        return -1;
    }    

    /* Used mostly for Pass and Sewell heuristics */
    std::set<int> obterCoresDisponiveisParaVertice(grafo::Grafo& G, vector<int> & coloracaoAtual, int i, int to) {
        /* List that indicates if some color has already been used
        In the worst case n colors will be required */
        vector<bool> coresUsadas(to, false);
        vector<int> adj = G.listaAdj[i];
        set<int> availableColors;

        /* We loop through the adjacency list and select which colors may be used */
        for (int z = 0; z < adj.size(); z++)
        {
            int j = adj[z];

            /* Assigns used color */
            if (coloracaoAtual[j] != -1)
            {
                coresUsadas[coloracaoAtual[j]] = true;
            }
        }

        /* Looks for a color that has not been used yet */
        for (int c = 0; c < to; c++) {
            if (coresUsadas[c] == false) {
                availableColors.insert(c);
            }
        }

        return availableColors;
    }

    /* Returns total number of colors used in some coloring */
    int obterTotalCores (vector<int> coloracao) {
        vector<bool> coresUsadas(coloracao.size(), false);
        int total = 0;

        for (int cor: coloracao) {
            if (cor >= 0) {
                coresUsadas[cor] = true;
            };

        }

        for (bool c: coresUsadas) {
            if (c == true) {
                total++;
            }
        }    
        return total;
    }

}

#endif
