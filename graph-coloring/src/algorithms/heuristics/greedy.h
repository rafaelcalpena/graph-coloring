#ifndef GREEDYH
#define GREEDYH

namespace greedy {
    using namespace std;
    
    /* Greedy Algorithm: */
    /* Receives a graph and returns a coloring vector */
    vector<int> greedy(grafo::Grafo &G) {
        /* Associates used vertices and colors; Begins with -1 color (non-existent) */
        vector<int> vCores(G.n, -1);
        /* Initial number of colors used is zero */
        int nCores = 0;

        /* Tries to add a color for each vertex of the graph */
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