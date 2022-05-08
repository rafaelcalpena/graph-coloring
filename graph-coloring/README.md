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

Edite o testbench/config.js

Para rodar na cloud (no diretório `pgc`, root folder)
docker build -t test-pulumi --progress=plain .
```docker run -e AWS_ACCESS_KEY_ID={} AWS_SECRET_ACCESS_KEY={} -e PULUMI_CONFIG_PASSPHRASE="" -v `pwd`/.pulumi:/root/.pulumi -v /var/run/docker.sock:/var/run/docker.sock -v `pwd`/../graph-coloring:/pulumi/projects/graph-coloring test-pulumi```

- Será necessario rodar `pulumi up` de dentro do container para configurar o primeiro stack
- Pre requisito: Criar uma conta na AWS e criar um usuario para acesso programatico
- Abrir o IAM e adicionar usuarios
- Habilitar chave de acesso selecionando: "Chave de acesso: acesso programático"
- Criar um grupo EC2FullAccess com política AmazonEC2FullAccess, S3All com AmazonS3FullAccess
Adicionar também iam:CreateRole, ecr:CreateRepository, logs:CreateLogGroup, AmazonECS_FullAccess
usei IAMFullAccess, CloudWatchLogsFullAccess, AmazonEC2ContainerRegistryFullAccess, AWSLambda_FullAccess, AmazonAPIGatewayAdministrator, AmazonElasticFileSystemFullAccess, AWSBatchFullAccess, AWSDataSyncFullAccess
- Guardar as chaves geradas e passar no docker run acima
