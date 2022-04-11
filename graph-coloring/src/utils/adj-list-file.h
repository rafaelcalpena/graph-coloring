#ifndef ADJLISTFILEH
#define ADJLISTFILEH

#include "./grafo.h"
#include <fstream>

using namespace std;

namespace adjlist {

    void adicionaListaDeAdjAoArquivo(fstream& arquivo, grafo::Grafo& grafo) {
        arquivo << "adjList=[";
        for (int j=0; j < grafo.listaAdj.size(); j++) {
            arquivo << "[";
            for (int k=0; k < grafo.listaAdj[j].size(); k++) {
                arquivo << grafo.listaAdj[j][k];
                if (k != grafo.listaAdj[j].size() - 1) {
                    arquivo << ","; 
                }
            }
            arquivo << "]";
            if (j != grafo.listaAdj.size() - 1) {
                arquivo << ","; 
            }
        }
        arquivo << "]; \n";
    }

}

#endif