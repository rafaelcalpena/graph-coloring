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

        /* TODO: Juntar com codigo greedy */
    int obterCorDisponivelParaVertice(grafo::Grafo& G, vector<int> coloracaoAtual, int i, int from, int to) {
        /* Lista que indica se uma cor ja foi usada.
        No pior caso, n cores serao necessarias */
        vector<bool> coresUsadas(to, false);
        vector<int> adj = G.listaAdj[i];

        /* Passamos pela lista de adjacencia do vertice e marcamos quais cores podem ser utilizadas */
        for (int z = 0; z < adj.size(); z++)
        {
            int j = adj[z];

            /* Assinalar cor utilizada */
            if (coloracaoAtual[j] != -1)
            {
                coresUsadas[coloracaoAtual[j]] = true;
            }
        }

        /* Procuramos uma cor que não tenha sido usada */
        for (int c = from; c < to; c++) {
            if (coresUsadas[c] == false) {
                return c;
            }
        }

        /* Retornamos -1 em caso de ausencia de cores válidas */
        return -1;
    }    

    /* Retorna o numero total de cores de uma coloracao */
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
