#ifndef DSATURH
#define DSATURH

namespace dsatur {
    using namespace std;

    int grauDeSaturacao(int indice, grafo::Grafo& G, vector<int> vCores) {
        vector<int> adjacentes = G.listaAdj[indice];
        int saturacao = 0;
        /* Colors found on vertex index neighbors */
        vector<bool> coresUsadas(G.n, false);    

        for (int i = 0; i < adjacentes.size(); i++) {
            int vizinho = adjacentes[i];
            int corVizinho = vCores[vizinho];
            if ( (corVizinho != -1) && !coresUsadas[corVizinho] ) {
                coresUsadas[corVizinho] = true;
                saturacao++;
            }
        }
        return saturacao;
    }


    int reordenarProximoIndice(vector<int>& ordenacao, int i, grafo::Grafo& G, vector<int> vCores) {

        /* Reordering:*/
        /* Find item with highest saturation degree in adjacency list */
        int indiceVencedor = i;
        int vencedorSat = dsatur::grauDeSaturacao(ordenacao[indiceVencedor], G, vCores);

        for (int prox = i+1; prox < G.n; prox++) {
            int proxSat = dsatur::grauDeSaturacao(ordenacao[prox], G, vCores);

            if (
                (proxSat > vencedorSat) || 
                /* Break ties by largest vertex degree */
                ((proxSat == vencedorSat) && (grauDoVertice(ordenacao[prox], G) > grauDoVertice(ordenacao[indiceVencedor], G))) ||
                /* Keep vertices in ascending order */
                ((proxSat == vencedorSat) && (grauDoVertice(ordenacao[prox], G) == grauDoVertice(ordenacao[indiceVencedor], G)) && (ordenacao[prox] < ordenacao[indiceVencedor]))
            ) {

                indiceVencedor = prox;
                vencedorSat = proxSat;

            }
        }

        /* Only reorder current item for DSATUR */

        int indice = ordenacao[indiceVencedor];

        vectorUtils::trocar(ordenacao, i, indiceVencedor);

        return indice;
    }


    /* DSATUR Heuristic */
    vector<int> DSATUR (grafo::Grafo& G) {
        /* Associates used vertices and colors; Begins with -1 color (non-existent) */
        vector<int> vCores(G.n, -1);
        /* Initial number of colors used is zero */
        int nCores = 0;

        /* Unlike greedy, DSATUR orders by saturation degree, breaking ties by vertex degree  */
        /* We begin with dsatur = adjacency list indexes, and reorder on each step */
        vector<int> dsatur = vectorUtils::vetorCrescente(G.n);

        /* Tries to add a color for each vertex of the graph */
        
        /* dsi represents the index in the dsatur list */
        for (int dsi = 0; dsi < G.n; dsi++) {

            int indice = reordenarProximoIndice(dsatur, dsi, G, vCores);

            int cor = grafo::obterCorDisponivelParaVertice(G, vCores, indice, 0, nCores);

            if (cor == -1)
            {
                vCores[indice] = nCores;                
                nCores++;
            } else {
                vCores[indice] = cor;
            }
        }
        return vCores;
    }    
}

#endif 