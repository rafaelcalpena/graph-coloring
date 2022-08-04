#ifndef LERARQUIVOH
#define LERARQUIVOH

namespace lerArquivo {

#include <vector>
#include "grafo.h"
#include <string.h>

grafo::Grafo lerArquivo(std::string nomeArquivo) {
    using namespace std;

	/* This code is slightly similar to the one in the book */
	/* Creates a stream to read the graph file */
	ifstream inStream;
	inStream.open(nomeArquivo);
	if (inStream.fail()) {
        cout << "Nao foi possível abrir o arquivo";
        exit(1);
    }

	char c, str[1000];
	int line = 0, u, v, edgeCnt = 0;
	vector<vector<bool> > A;
	grafo::Grafo G;
	try {
        /* For each line in the file */
		while (!inStream.eof()) {
			line++;
			inStream.get(c);
			if (inStream.eof()) break;
            /* Analyze type of line (1st char c, p, or e) */
			switch (c) {
			case 'c':
				/* Ignore comments */
				inStream.putback('c');
				inStream.get(str, 999, '\n');
				break;
			case 'p':
                /* Obtain information about the problem */
				inStream.get(c);
				inStream.getline(str, 999, ' ');
				if (G.n != 0 || G.m != 0) {
                    cout << "O problema p foi definido duas vezes";
                    exit(1);
                }
				if (strcmp(str, "edge")) {
                    cout << "O arquivo do problema deve conter a palavra edge";
                    exit(1);
                }
                /* Apply number of vertices and edges to G.n and G.m, respectively */
				inStream >> G.n >> G.m;
                /* Remove os elementos da matriz de adjacencia */
				A.clear();
                /* Resize adjacency matrix to obtain number of vertices */
				A.resize(G.n, vector<bool>(G.n, false));
                /* Assign diagonals with 1 values  */
				for (u = 0; u < G.n; u++) A[u][u] = true;
				break;
			case 'e':
				/* Read an edge */
				inStream >> u >> v;

				if (u < 1 || u > G.n || v < 1 || v > G.n || u == v) {
                    cout << "Aresta Inválida na linha " + to_string(line);
                    exit(1);
                };
				
                edgeCnt++;
				
                if (A[u - 1][v - 1]) {
					/* Edge had already been defined */
                }

				A[u - 1][v - 1] = true;
				A[v - 1][u - 1] = true;
				break;
			default:
				cout << "Problema na linha " + to_string(line);
			}
			inStream.get();
		}

        /* Close file read stream */
		inStream.close();
		if (edgeCnt != G.m) {
            cout << "Número de arestas do problema incompatível com a definicao";
            exit(1);
        }
	}
	catch (...) {
		cout << "Arquivo inválido";
        exit(1);
	}
	inStream.close();

	/* If graph has not edges, exit */
	if (G.m <= 0) {
		cout << "Grafo nao possui arestas";
        exit(1);
	}

	/* Create adjacency list and degrees from adjacency matrix */
	G.graus.clear();
	G.listaAdj.clear();
	G.graus.resize(G.n, 0);
	G.listaAdj.resize(G.n, vector<int>());
	for (u = 0; u < G.n; u++) {
		for (v = 0; v < G.n; v++) {
			if (A[u][v] && u != v) {
				G.listaAdj[u].push_back(v);
				G.graus[u]++;
			}
		}
	}
	G.grauMaximo = *std::max_element(G.graus.begin(), G.graus.end());
	return(G);
}


}

#endif