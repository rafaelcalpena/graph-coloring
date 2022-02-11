# Instruções

Para compilar e rodar o programa, utilize o seguinte comando

```g++ src/graph-coloring.cpp -o ./run && clear && ./run```

Exemplo: 
```g++ src/graph-coloring.cpp -o ./run && clear && DEBUG=1 FILE="dimacs/queen6_6.col" ./run```

Usar a versão mais recente:
```c++ -std=c++17 -stdlib=libc++ src/graph-coloring.cpp -o ./run && clear && ALG="dsatur-backtracking" DEBUG=1 FILE="dimacs/queen6_6.col" ./run```

Docker Build Image:
```docker build -t 'pgc' .```
```docker run -v `pwd`/output:/~/output pgc```


Instruções:
Configure o Docker para usar um numero de CPUs >= 6 para paralelismo
```docker-compose build```
```docker-compose up```
