#ifndef GREEDYH
#define GREEDYH

namespace greedy {
    using namespace std;
    
    /* Algoritmo Greedy: */
    /* Recebe um grafo (referencia) e retorna um vetor de colorações */
    vector<int> greedy(grafo::Grafo &G) {
        /* Associa indice do vertice com cor usada; Inicia com cor -1 (inexistente) */
        vector<int> vCores(G.n, -1);
        /* Inicialmente, o numero de cores é 0 */
        int nCores = 0;

        /* para cada vertice do grafo, tenta adicionar uma cor */
        for (int i = 0; i < G.n; i++)
        {

            int cor = grafo::obterCorDisponivelParaVertice(G, vCores, i, 0, nCores);

            if (cor == -1)
            {
                vCores[i] = nCores;                
                nCores++;
            } else {
                vCores[i] = cor;
            }
        }
        return vCores;
    }
}

#endif