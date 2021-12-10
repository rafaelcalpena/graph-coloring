#ifndef VECTORUTILSH
#define VECTORUTILSH

namespace vectorUtils {
    using namespace std;

    void trocar(vector<int>& dsatur, int indice, int novo) {
        int itemAnterior = dsatur[indice];
        dsatur[indice] = dsatur[novo];
        dsatur[novo] = itemAnterior;
    }

    void imprimeVetor(vector<int> qualquer) {
        for (int item: qualquer) {
            cout << item << " ";
        }
        cout << "\n";
    }

    /* Fonte: https://www.techiedelight.com/copy-vector-cpp/ */
    template<typename T>
    std::vector<T> copiarVetor(std::vector<T> const &vec)
    {
        std::vector<T> v(vec);
        return v;
    }

    string serializarVetor(vector<int> v) {
        string myfile;
        myfile += "[";
        for (int k=0; k < v.size(); k++) {
            myfile += to_string(v[k]);
            if (k != v.size() - 1) {
                myfile += ","; 
            }
        }
        myfile += "]";    
        return myfile;
    }

    vector<int> vetorCrescente(int qtd) {
        vector<int> resultado(qtd, 0);

        for (int y = 0; y < qtd; y++) {
            resultado[y] = y;
        }
        return resultado;
    }

}

#endif