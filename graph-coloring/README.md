# Instruções

Para compilar e rodar o programa, utilize o seguinte comando

```g++ src/graph-coloring.cpp -o ./run && clear && ./run```

Exemplo: 
```g++ src/graph-coloring.cpp -o ./run && clear && DEBUG=1 FILE="dimacs/queen6_6.col" ./run```

Usar a versão mais recente:
```c++ -std=c++17 -stdlib=libc++ src/graph-coloring.cpp -o ./run && clear && DEBUG=1 FILE="dimacs/queen6_6.col" ./run```