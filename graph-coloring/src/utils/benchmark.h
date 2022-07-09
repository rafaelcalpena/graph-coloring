#ifndef BENCHMARKUTILSH
#define BENCHMARKUTILSH

namespace benchmarkUtils {

    void runExactBenchmark(string algorithm, grafo::Grafo & grafo, std::vector<int> (*func)(grafo::Grafo &, std::fstream &, int &) ) {
        int backtrackingVertices = 0;

        std::cout << "{\"" << algorithm << "\": {\"colors\": ";
        
        /* Creates a log file for backtracking */
        fstream backtrackingLog;    

        /* Directory must exist for open file call to work */
        backtrackingLog.open("output/" + algorithm + "-output.js", fstream::out);    
        adjlist::adicionaListaDeAdjAoArquivo(backtrackingLog, grafo);

        backtrackingLog << "graphFileName = '" << getenv("FILE") << "';\n";    
        backtrackingLog << "logs = ";
        backtrackingLog << "[";    

        auto start = std::chrono::high_resolution_clock::now();

        /* Call algorithm function */
        vector<int> cores = func(grafo, backtrackingLog, backtrackingVertices);

        auto stop = std::chrono::high_resolution_clock::now();
        /* Calculate total duration */
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        backtrackingLog << "]";

        std::cout << (*max_element(cores.begin(), cores.end()) + 1);
        std::cout << ", \"time\":" << duration.count();
        std::cout << ", \"backtrackingVertices\":" << backtrackingVertices;

        grafo::verificaColoracao(grafo, cores);
        std::cout << "}}" << std::endl;
        backtrackingLog.close();       
    }

    void runHeuristicBenchmark(string algorithm, grafo::Grafo & grafo, std::vector<int> (*func)(grafo::Grafo &)) {
        std::cout << "{\"" << algorithm << "\": {\"colors\": ";

        /* https://www.geeksforgeeks.org/measure-execution-time-function-cpp/ */
        auto start = std::chrono::high_resolution_clock::now();
        vector<int> cores = func(grafo);
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        std::cout << (*max_element(cores.begin(), cores.end()) + 1);
        std:: cout << ", \"time\":" << duration.count();
        grafo::verificaColoracao(grafo, cores);
        std::cout << "}}";
        salvarArquivo::salvarEmArquivo("output/" + algorithm + "-heuristic-output.js", grafo, cores);
    }

}



#endif