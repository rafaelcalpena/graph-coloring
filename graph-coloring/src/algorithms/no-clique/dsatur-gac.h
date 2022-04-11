#include "../../utils/debug.h"
#include "./dsatur.h"
#include <set>
#include <map>
#include "../../csp/csp.h"
#include "../../csp/ac3.h"
#include <math.h>       /* floor */

#ifndef DSATURGACH
#define DSATURGACH

namespace dsaturGAC {

    using namespace std;

    map< string, vector<int> > getCSPDomains(vector<int>& ordenacao, vector<int>& coloracaoAtual, int upperBound) {
        map<string, vector<int>> domains;

        for (int i = 0; i < ordenacao.size(); i++) {
            int v = ordenacao[i];
            if (coloracaoAtual[v] != -1) {
                /* Adiciona apenas uma cor no dominio */
                domains.insert({ to_string(v), {coloracaoAtual[v]} });
            } else {
                /* TODO: Valores aqui */
                vector<int> ordenacaoParcial = vectorUtils::copiarSubvetor(ordenacao, 0, i);
                // vectorUtils::imprimeVetor(ordenacaoParcial);
                /* Analisa a cor de cada vertice na ordenacao parcial */
                vector<int> coloracaoParcial;
                for (int a: ordenacaoParcial) {
                    coloracaoParcial.push_back(coloracaoAtual[a]);
                }
                int ub = std::min(std::min(upperBound, i + 1), (grafo::obterTotalCores(coloracaoParcial) + 1));
                // int ub = std::min(upperBound, i + 1);

                vector<int> values = vectorUtils::vetorCrescente(ub);

                domains.insert({ to_string(v), values });
            }
            
        }
        return domains;
    }

    vector<vector<string>> getCSPConstraints(grafo::Grafo& G) {
        vector<vector<string>> lines;
        for (int i = 0; i < G.listaAdj.size(); i++) {
            for (int j: G.listaAdj[i]) {
                if (i < j) {
                    vector<string> c {to_string(i), "!=", to_string(j)};
                    lines.push_back(c);
                }
            }
        }
        return lines;      
    }

    /* Algoritmo com GAC */
    /* Contém o Backtracking para uma ordem de vértices arbitrária */
    vector<int> dsaturGAC (grafo::Grafo& G, fstream& logStream, int & backtrackingVertices) {

        /* Consistencia de arco generalizada */
        bool gacEnabled = false;

        backtrackingVertices = 0;

        /* Associa indice do vertice com cor usada; Inicia com cor -1 (inexistente) */
        vector<int> coloracaoAtual(G.n, -1);
        DEBUG("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);   

        /* Guarda a melhor coloracao encontrada; Inicia com cor -1 (inexistente) */
        vector<int> melhorColoracao(G.n, -1);
        DEBUG("{action: 'set', key: 'melhorColoracao', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);     

        /* Guarda o numero de cores minimo atual; Inicia com n (pior caso) */
        int k = G.n; //grau maximo + 1 é um limitante
        DEBUG("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);

        /* Todo grafo possui limitante superior grau maximo + 1 */
        int grauMaximo = grafo::grauMaximoVertices(G);
        DEBUG("{action: 'getMaximumDegree', value:" + to_string(grauMaximo) + "}", logStream);

        if (grauMaximo + 1 < k) {
            k = grauMaximo + 1;
            DEBUG("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);
        }

        /* ordenacao atual, comeca com [0, 1, ..., n - 1]; */
        vector<int> ordenacao = vectorUtils::vetorCrescente(G.n);
        DEBUG("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);

        /* Guarda a posicao atual dentro da ordenacao a ser analisada */
        int i = 0;
        DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream); 

        dsatur::reordenarProximoIndice(ordenacao, i, G, coloracaoAtual);            
        DEBUG("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);             

        DEBUG("{action: 'finishInitialSetup'}", logStream); 

        /* Repete ate o final (i = -1) */
        while (i != -1) {
            DEBUG("{action: 'iteration', value: " + to_string(i) + "}", logStream);

            /* TODO: Usar coloracaoAtual ao inves de tempColoracao */
            /* Para que a ordenacao seja feita de forma correta, é necessário ignorar a coloracao atual para o index i */
            int tempCor = coloracaoAtual[ordenacao[i]];
            coloracaoAtual[ordenacao[i]] = -1;
            dsatur::reordenarProximoIndice(ordenacao, i, G, coloracaoAtual);

            /* Precisa ser feita antes de retornar tempCor no DSATUR, senao seria possível que i < totalCores */
            /* Definicao de tight coloring do Brown, para evitar buscas em branches desnecessárias (permutações) */
            int boundary = min(k, grafo::obterTotalCores(coloracaoAtual) + 1);

            coloracaoAtual[ordenacao[i]] = tempCor;            
            DEBUG("{action: 'set', key: 'ordenacao', value: " + vectorUtils::serializarVetor(ordenacao) + "}", logStream);

            int indice = ordenacao[i];
            DEBUG("{action: 'set', key: 'indice', value: " + to_string(indice) + "}", logStream); 

            /* Tenta obter uma cor para o vertice */
            int cor = grafo::obterCorDisponivelParaVertice(G, coloracaoAtual, indice, coloracaoAtual[indice] + 1, boundary);
            DEBUG("{action: 'set', key: 'cor', value: " + to_string(cor) + "}", logStream);

            coloracaoAtual[indice] = cor;
            DEBUG("{action: 'set', key: 'coloracaoAtual', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

            int totalCores = grafo::obterTotalCores(coloracaoAtual);
            DEBUG("{action: 'getColoringNumber', value: " + to_string(totalCores) + "}", logStream);       

            /* Guarda se todos os dominio sao nao vazios no gac */ 
            bool gacValidDomains = true;
            /* Roda o GAC para verificar */
            /* Usa o meio do caminho para o pruning */
            if (gacEnabled && (k - totalCores == 2 )) {
            // if (gacEnabled && (k - totalCores == 1 )) {
            // if (gacEnabled) {
            // if (gacEnabled && (i == floor(G.n / 2))) {
            // if (gacEnabled && (i == floor(G.n * 0.75))) {
            // if (gacEnabled && (i == floor(G.n * 0.25))) {
                /* Cria o csp para o estado atual */
                /* TODO: Optimizar, criar apenas uma vez se possivel */
                map< string, vector<int> > domains = dsaturGAC::getCSPDomains(ordenacao, coloracaoAtual, k);
                vector< vector<string> > constraints = getCSPConstraints(G);
                csp::CSP test(domains, constraints);
                gacValidDomains = ac3::ac3(test);
                if (!gacValidDomains) {
                // if (gacEmptyDomain && (totalCores < k)) {
                    // cout << "cancelar totalCores=" << totalCores << ", k=" << k << endl;
                }
                // gacEmptyDomain = false;
                // cout << "gac ativado para " << i << endl;

                /* Checa se a cor estava no dominio do gac */
                // if (cor != -1) {
                //     cout << "GAC enabled, cor=" << cor << ", dominio: "; //<< endl;
                //     for (int n: test.domains.at(to_string(indice))) {
                //         cout << n << ";";
                //     }
                //     cout << ", indice " << indice << " => " << (test.domains.at(to_string(indice)).find(cor) != test.domains.at(to_string(indice)).end() ? "s" : "n") << endl;
                // }

            }

            /* Se nenhuma cor é válida, é necessário voltar (backwards) */        
            if (cor == -1) {
                /* Remove coloracao atual do indice */
                DEBUG("{action: 'moveBackwards', value:'noValidColors'} ", logStream);

                i--;
                DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);  

                /* Otimizacao, pois o primeiro vertice nao precisa ser testado com as cores restantes */
                if (i == 0) {
                    DEBUG("{action: 'stop'}", logStream);
                    break;
                } 

            } 
            /* Se coloracao é pior, pular a branch */
            else if (totalCores > k) {
                backtrackingVertices++;

                DEBUG("{action: 'preventSearchInSubBranches', value:'coloringWorseThanLimit "+ to_string(k) + " '} ", logStream);

                /* Ainda pode existir uma ou mais cores para o vertices melhores do que a combinacao atual, portanto
                deve-se aguardar mais uma iteracao no mesmo i (nao ha backtracking). 
                Esse if tambem ajuda a evitar o proximo caso, ou seja, para de explorar ou sub-ramos para a iteracao atual
                Caso esse else if seja comentado, a solucao final tera varias coloracoes nao-otimas */

            }
            /* Se o GAC indicou dominio vazio, é necessário pular */
            else if (!gacValidDomains) {
                // cout << "pruned at i=" << i << " of " << G.n << ", " << ((float) i / (float) G.n) * 100 << "%" << ", k=" << k << ", " << ((float) i / (float) k) * 100 << "%" << endl;
                if (k - totalCores > 1) {
                    // cout << "DELTA = " << k - totalCores << " pruned at i=" << i << ", totalCores=" << totalCores << " of " << G.n << ", " << ((float) totalCores / (float) G.n) * 100 << "%" << ", k=" << k << ", " << ((float) totalCores / (float) k) * 100 << "%" << endl;
                }

                backtrackingVertices++;

                DEBUG("{action: 'preventSearchInSubBranches', value:'gacEmptyDomain "+ to_string(k) + " '} ", logStream);
            }
            else {
                /* Se tiver mais vertices, continua (forward) */
                backtrackingVertices++;

                if (i < G.n - 1) {
                    DEBUG("{action: 'moveForward'} ", logStream);                

                    i++;       
                    DEBUG("{action: 'set', key: 'i', value: " + to_string(i) + "}", logStream);     

                } else {
                    /* Chegou em uma nova coloracao */
                    DEBUG("{action: 'foundColoring', value: " + vectorUtils::serializarVetor(coloracaoAtual) + "}", logStream);

                    melhorColoracao = vectorUtils::copiarVetor(coloracaoAtual);
                    DEBUG("{action: 'set', key: 'melhorColoracao', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);

                    k = totalCores - 1;
                    DEBUG("{action: 'set', key: 'k', value: " + to_string(k) + "}", logStream);

                    /* Ativa o GAC a partir da primeira solucao */
                    gacEnabled = true;
                }

            }
        }

        DEBUG("{action: 'finalResult', value: " + vectorUtils::serializarVetor(melhorColoracao) + "}", logStream);
        return melhorColoracao;
    }

}

#endif