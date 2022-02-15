#include <tuple>

#ifndef VECTORUTILSH
#define VECTORUTILSH

namespace vectorUtils {

    using namespace std;

    void trocar(vector<int>& dsatur, int indice, int novo) {
        int itemAnterior = dsatur[indice];
        dsatur[indice] = dsatur[novo];
        dsatur[novo] = itemAnterior;
    }

    void imprimeVetor(vector<int>& qualquer) {
        for (int item: qualquer) {
            cout << item << " ";
        }
        cout << "\n";
    }

    void printTuples(vector<std::tuple<int, string>> & qualquer, int size = 2) {
        for (std::tuple<int, string> item: qualquer) {
            cout << "(";
            cout << get<0>(item) << ", ";
            cout << "\"" << get<1>(item) << "\"";
            cout << ")" << endl;
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

    /* https://stackoverflow.com/questions/421573/best-way-to-extract-a-subvector-from-a-vector */
    template<typename T>
    std::vector<T> copiarSubvetor(std::vector<T> const &vec, int from, int to)
    {
        auto first = vec.begin() + from;
        auto last = vec.begin() + to;
        vector<T> newVec(first, last);

        return newVec;
    }    

    string serializarVetor(vector<int> & v) {
        string myfile;
        const std::string flag = getenv("DEBUG");
        if (flag == "1" || flag == "2") {
            myfile += "[";
            for (int k=0; k < v.size(); k++) {
                myfile += to_string(v[k]);
                if (k != v.size() - 1) {
                    myfile += ","; 
                }
            }
            myfile += "]";
        }    
        return myfile;
    }

    string serializarSet(set<int> & v) {
        string myfile;
        const std::string flag = getenv("DEBUG");
        if (flag == "1" || flag == "2") {        
            myfile += "[";
            auto it = v.begin();
            for (int k=0; k < v.size(); k++) {
                myfile += to_string(*it);
                if (k != v.size() - 1) {
                    myfile += ","; 
                }
                it++;
            }
            myfile += "]";    
        }
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