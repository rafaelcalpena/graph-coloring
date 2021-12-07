#ifndef DSATURH
#define DSATURH

namespace dsatur {
    using namespace std;

    int grauDeSaturacao(int indice, grafo::Grafo& G, vector<int> vCores) {
        vector<int> adjacentes = G.listaAdj[indice];
        int saturacao = 0;
        /* Cores encontradas nos vizinhos do indice */
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

/* Algoritmo DSATUR (apenas heurística): */
vector<int> DSATUR (grafo::Grafo& G) {
    /* Associa indice do vertice com cor usada; Inicia com cor -1 (inexistente) */
    vector<int> vCores(G.n, -1);
    /* Inicialmente, o numero de cores é 0 */
    int nCores = 0;

    /* IMPORTANTE: */
    /* Diferentemente do greedy, DSATUR ordena por grau de saturacao, com desempate pelo grau do vertice */
    /* Iniciamos com dsatur = indices da lista de adjacencia, e reordenamos a cada passo */
    vector<int> dsatur(G.n, 0);
        for (int y = 0; y < G.n; y++) {
            dsatur[y] = y;
        }

        /* para cada vertice do grafo, tenta adicionar uma cor */
        /* dsi representa o indice na lista dsatur acima */
        for (int dsi = 0; dsi < G.n; dsi++) {

            /* Reordenação: */
            /* Encontrar o item com maior grau de saturacao na lista de adjacencia */
            int indiceVencedor = dsi;
            int vencedorSat = dsatur::grauDeSaturacao(dsatur[indiceVencedor], G, vCores);

            for (int prox = dsi+1; prox < G.n; prox++) {
                int proxSat = dsatur::grauDeSaturacao(dsatur[prox], G, vCores);

                if (
                    (proxSat > vencedorSat) || 
                    /* Desempate por maior grau do vertice */
                    ((proxSat == vencedorSat) && (grauDoVertice(dsatur[prox], G) > grauDoVertice(dsatur[indiceVencedor], G)))
                ) {
                    indiceVencedor = prox;
                    vencedorSat = proxSat;
                }
            }

            /* Reordena apenas o item atual em DSATUR */

            int i = dsatur[indiceVencedor];

            vectorUtils::trocar(dsatur, dsi, indiceVencedor);

            /* Lista que indica se uma cor ja foi usada.
            No pior caso, n cores serao necessarias */ 
            vector<bool> coresUsadas(nCores, false);
            vector<int> adj = G.listaAdj[i];

            /* Passamos pela lista de adjacencia do vertice e decidimos quais cores podem ser utilizadas */
            for (int z = 0; z < adj.size(); z++) {
                int j = adj[z];
                
                /* Assinalar cor utilizada */
                if (vCores[j] != -1) {
                    coresUsadas[vCores[j]] = true;
                }
            }

            /* Procuramos uma cor que não tenha sido usada; Se não houver, criamos uma */
            bool usouCor = false;
            for (int c = 0; c < coresUsadas.size(); c++) {
                if (coresUsadas[c] == false) {
                    vCores[i] = c;
                    usouCor = true;
                    break;
                }
            }

            if (!usouCor) {
                nCores++;
                vCores[i] = nCores - 1;
            }
        }
        return vCores;
    }    
}

#endif 