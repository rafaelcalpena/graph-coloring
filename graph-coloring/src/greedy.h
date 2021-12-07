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

            /* Lista que indica se uma cor ja foi usada.
        No pior caso, n cores serao necessarias */
            vector<bool> coresUsadas(nCores, false);
            vector<int> adj = G.listaAdj[i];

            /* Passamos pela lista de adjacencia do vertice e decidimos quais cores podem ser utilizadas */
            for (int z = 0; z < adj.size(); z++)
            {
                int j = adj[z];

                /* Assinalar cor utilizada */
                if (vCores[j] != -1)
                {
                    coresUsadas[vCores[j]] = true;
                }
            }

            /* Procuramos uma cor que não tenha sido usada; Se não houver, criamos uma */
            bool usouCor = false;
            for (int c = 0; c < coresUsadas.size(); c++)
            {
                if (coresUsadas[c] == false)
                {
                    vCores[i] = c;
                    usouCor = true;
                    break;
                }
            }

            if (!usouCor)
            {
                nCores++;
                vCores[i] = nCores - 1;
            }
        }
        return vCores;
    }
}

#endif