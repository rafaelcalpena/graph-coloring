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


    int reordenarProximoIndice(vector<int>& ordenacao, int i, grafo::Grafo& G, vector<int> vCores) {

        /* Reordenação: */
        /* Encontrar o item com maior grau de saturacao na lista de adjacencia */
        int indiceVencedor = i;
        int vencedorSat = dsatur::grauDeSaturacao(ordenacao[indiceVencedor], G, vCores);

        for (int prox = i+1; prox < G.n; prox++) {
            int proxSat = dsatur::grauDeSaturacao(ordenacao[prox], G, vCores);

            if (
                (proxSat > vencedorSat) || 
                /* Desempate por maior grau do vertice */
                ((proxSat == vencedorSat) && (grauDoVertice(ordenacao[prox], G) > grauDoVertice(ordenacao[indiceVencedor], G))) ||
                /* Mantem os vertices em ordem crescente */
                ((proxSat == vencedorSat) && (grauDoVertice(ordenacao[prox], G) == grauDoVertice(ordenacao[indiceVencedor], G)) && (ordenacao[prox] < ordenacao[indiceVencedor]))
            ) {

                indiceVencedor = prox;
                vencedorSat = proxSat;

            }
        }

        /* Reordena apenas o item atual em DSATUR */

        int indice = ordenacao[indiceVencedor];

        vectorUtils::trocar(ordenacao, i, indiceVencedor);

        return indice;
    }


    /* TODO: Usar o greedy atual */
    /* Algoritmo DSATUR (apenas heurística): */
    vector<int> DSATUR (grafo::Grafo& G) {
        /* Associa indice do vertice com cor usada; Inicia com cor -1 (inexistente) */
        vector<int> vCores(G.n, -1);
        /* Inicialmente, o numero de cores é 0 */
        int nCores = 0;

        /* IMPORTANTE: */
        /* Diferentemente do greedy, DSATUR ordena por grau de saturacao, com desempate pelo grau do vertice */
        /* Iniciamos com dsatur = indices da lista de adjacencia, e reordenamos a cada passo */
        vector<int> dsatur = vectorUtils::vetorCrescente(G.n);

        /* para cada vertice do grafo, tenta adicionar uma cor */
        
        /* dsi representa o indice na lista dsatur acima */
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