#include <fstream>
#include "./grafo.h"

using namespace std;

namespace salvarArquivo {

    void salvarEmArquivo (string filename, grafo::Grafo& grafo, vector<int> cores) {
        fstream myfile;
        myfile.open(filename,fstream::out);
        myfile << "dataResult = ";
        myfile << "{\"adjList\":[";
        for (int j=0; j < grafo.listaAdj.size(); j++) {
            myfile << "[";
            for (int k=0; k < grafo.listaAdj[j].size(); k++) {
                myfile << grafo.listaAdj[j][k];
                if (k != grafo.listaAdj[j].size() - 1) {
                    myfile << ","; 
                }
            }
            myfile << "]";
            if (j != grafo.listaAdj.size() - 1) {
                myfile << ","; 
            }
        }
        myfile << "], \"colors\": [";
            for (int k=0; k < cores.size(); k++) {
                myfile << cores[k];
                if (k != cores.size() - 1) {
                    myfile << ","; 
                }
            }
        myfile << "]}";
        myfile.close();
    }

}

