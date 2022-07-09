#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "./utils/grafo.h"
#include "./utils/ler-arquivo.h"
#include "./utils/vector-utils.h"
#include "./utils/adj-list-file.h"
#include "./utils/salvar-arquivo.h"
#include "./utils/benchmark.h"

// #include "./experimental/greedy-backtracking3.h"
#include "./algorithms/heuristics/greedy.h"
#include "./algorithms/heuristics/dsatur.h"
#include "./algorithms/exact/greedy-backtracking2.h"
#include "./algorithms/exact/dsatur-backtracking.h"
#include "./algorithms/exact/dsatur-sewell.h"
#include "./algorithms/exact/dsatur-pass-always.h"
#include "./algorithms/exact/dsatur-pass-conditional.h"
#include "./algorithms/exact/dsatur-gac-1.h"


using namespace std;
using namespace grafo;

/* Função principal */
int main(int argc, char** argv) {

    if ( (*getenv("FILE") == NULL) || (*getenv("ALG") == NULL) ) {
        throw std::invalid_argument("Program requires both FILE and ALG environment variables to be set");
    }

    grafo::Grafo grafo = lerArquivo::lerArquivo(getenv("FILE"));

    string algorithm = getenv("ALG");

    /* We cannot use switch for string (algorithm), so we use if/elses instead */
    if (algorithm == "greedy") {
        benchmarkUtils::runHeuristicBenchmark("greedy", grafo, & (greedy::greedy));
    }

    else if (algorithm == "dsatur") {
        benchmarkUtils::runHeuristicBenchmark("dsatur", grafo, & (dsatur::DSATUR));
    }

    else if (algorithm == "dsatur-backtracking") {
        benchmarkUtils::runExactBenchmark("dsatur-backtracking", grafo, & (dsaturBacktracking::dsaturBacktracking) );
    }

    else if (algorithm == "dsatur-sewell") {
        benchmarkUtils::runExactBenchmark("dsatur-sewell", grafo, & (dsaturSewell::dsaturSewell) ); 
    }

    else if (algorithm == "dsatur-pass-always") {
        benchmarkUtils::runExactBenchmark("dsatur-pass-always", grafo, & (dsaturPassAlways::dsaturPassAlways) );
    }    

    else if (algorithm == "dsatur-pass-conditional") {
        benchmarkUtils::runExactBenchmark("dsatur-pass-conditional", grafo, & (dsaturPassConditional::dsaturPassConditional) );
    }        

    else if (algorithm == "dsatur-gac-0") {
        benchmarkUtils::runExactBenchmark("dsatur-gac-0", grafo, & (dsaturGAC::dsaturGAC0) );
    }        

    else if (algorithm == "dsatur-gac-1") {
        benchmarkUtils::runExactBenchmark("dsatur-gac-1", grafo, & (dsaturGAC::dsaturGAC1) );
    }        

    else if (algorithm == "dsatur-gac-2") {
        benchmarkUtils::runExactBenchmark("dsatur-gac-2", grafo, & (dsaturGAC::dsaturGAC2) );
    }

    else if (algorithm == "greedy-backtracking") {
        /* Greedy backtracking 2 contains an ordering array compatible with our implementation
        Greedy backtracking (1) is a bit faster, but not compatible */
        benchmarkUtils::runExactBenchmark("greedy-backtracking", grafo, & (greedyBacktracking2::greedyBacktracking2) );
    }

    return 0;
}