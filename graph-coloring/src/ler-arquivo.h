#ifndef LERARQUIVOH
#define LERARQUIVOH

namespace lerArquivo {

#include <vector>
#include "grafo.h"

grafo::Grafo lerArquivo(char nomeArquivo[]) {
    using namespace std;

	/* Cria uma stream para o ler o arquivo do grafo */
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
        /* Para cada linha do arquivo */
		while (!inStream.eof()) {
			line++;
			inStream.get(c);
			if (inStream.eof()) break;
            /* Analisa o tipo da linha (1o caractere c, p, ou e) */
			switch (c) {
			case 'c':
				/* Ignorar comentarios */
				inStream.putback('c');
				inStream.get(str, 999, '\n');
				break;
			case 'p':
                /* Obter informacoes do problema */
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
                /* Atribui os valores de numero de vertices e arestas para G.n e G.m, respectivamente */
				inStream >> G.n >> G.m;
                /* Remove os elementos da matriz de adjacencia */
				A.clear();
                /* Redimensiona a matriz de adjacencia para ter o numero de vertices */
				A.resize(G.n, vector<bool>(G.n, false));
                /* Marca as diagonais com 1 */
				for (u = 0; u < G.n; u++) A[u][u] = true;
				break;
			case 'e':
				/* Ler uma aresta */
				inStream >> u >> v;

				if (u < 1 || u > G.n || v < 1 || v > G.n || u == v) {
                    cout << "Aresta Inválida na linha " + to_string(line);
                    exit(1);
                };
				
                edgeCnt++;
				
                if (A[u - 1][v - 1]) {
                    cout << "Aviso: Aresta já havia sido definida " <<  u << " " << v << " \n";
                }

				A[u - 1][v - 1] = true;
				A[v - 1][u - 1] = true;
				break;
			default:
				cout << "Problema na linha " + to_string(line);
			}
			inStream.get();
		}

        /* Fechar leitura do arquivo */
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

	/* Se grafo nao tiver arestas, a solucao e apenas 1 cor */
	if (G.m <= 0) {
		cout << "Grafo nao possui arestas";
        exit(1);
	}

	/* Constroi a lista de adjacencia e graus a partir da matriz de adjacencia */
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
	G.grauMaximo = *max_element(G.graus.begin(), G.graus.end());
	return(G);
}


}

#endif