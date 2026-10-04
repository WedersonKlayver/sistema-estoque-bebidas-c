# Sistema de Estoque — Distribuidora de Bebidas Sertão Refrescante

Projeto I da disciplina de Estrutura de Dados (EDA) — IFBA, 2026.2.
Sistema de controle de estoque em linguagem C, executado no terminal.

**Autores:** Wederson 

## O que o sistema faz

- Cadastra, lista, busca e remove bebidas
- Atualiza a quantidade em estoque
- Registra vendas (valida estoque insuficiente)
- Registra recebimento de carregamentos (com nova validade)
- Calcula compras com desconto de atacado (10% acima de 24 unidades)
- Gera pedido de reposição automático
- Lista bebidas vencidas a partir de uma data informada
- Calcula o valor total em estoque por categoria
- Salva e carrega os dados automaticamente em arquivo texto

## Os quatro pilares do projeto

| Pilar | Como foi aplicado |
|---|---|
| **Structs** | `Bebida` com a struct aninhada `Data` (validade) |
| **Alocação dinâmica** | `malloc`, `realloc` e `free`; o vetor cresce e diminui conforme o uso, sem vazamentos |
| **Modularização** | Código dividido em módulos `.h` / `.c` |
| **Persistência** | Dados salvos em `estoque_bebidas.txt`, uma bebida por linha, campos separados por `;` |

## Estrutura de pastas

```
projeto_V4/
├── src/
│   ├── includes/
│   │   ├── bebida.h      # structs Data e Bebida + operações sobre UMA bebida
│   │   └── estoque.h     # operações sobre o VETOR de bebidas
│   ├── bebida.c
│   ├── estoque.c
│   └── sis_estoque.c     # main: menu e leitura segura de dados
├── .gitignore
└── README.md
```

- **bebida.c**: cadastrar, atualizar estoque, vender, receber carregamento, desconto de atacado.
- **estoque.c**: buscar, remover, adicionar ao vetor, salvar, carregar, listar, relatórios, liberar memória.
- **sis_estoque.c**: menu principal e validação das entradas do usuário.

## Como compilar

É preciso ter o **gcc** instalado (por exemplo, via MinGW/MSYS2 no Windows).

Dentro da pasta do projeto, liste todos os arquivos `.c` (o PowerShell não expande `src/*.c`):

```powershell
mkdir src/output
gcc -Wall -Wextra -Wpedantic -g src/sis_estoque.c src/bebida.c src/estoque.c -o src/output/sis_estoque.exe
```

No Linux/macOS:

```bash
mkdir -p src/output
gcc -Wall -Wextra -Wpedantic -g src/*.c -o src/output/sis_estoque
```

## Como executar

```powershell
.\src\output\sis_estoque.exe
```

O arquivo `estoque_bebidas.txt` é criado na pasta **de onde o programa é executado**. Se ele já existir, os dados são carregados ao iniciar.

## Menu

```
 1. Cadastrar bebida
 2. Listar todas as bebidas
 3. Buscar bebida por codigo
 4. Atualizar quantidade em estoque
 5. Remover bebida
 6. Vender bebida
 7. Receber carregamento
 8. Calcular compra com desconto de atacado
 9. Gerar pedido de reposicao automatico
10. Relatorio de bebidas vencidas
11. Valor total em estoque por categoria
 0. Salvar e sair
```

Os dados são gravados em arquivo ao escolher a opção **0**. Fechar o programa de outra forma não salva as alterações.

## Formato do arquivo

Uma bebida por linha:

```
codigo;nome;categoria;preco;volumeMl;quantidadeEstoque;dia;mes;ano
```

Exemplo:

```
1;Coca-Cola 2L;refrigerante;9.50;2000;48;15;12;2026
```

## Regras e decisões

- Categorias aceitas: `refrigerante`, `suco`, `agua`, `cerveja` (minúsculas, sem acento).
- O código da bebida deve ser positivo e único.
- O nome não pode ficar vazio nem conter `;` (o ponto e vírgula é o separador do arquivo).
- Nome com até 49 caracteres e categoria com até 19 (o excedente é cortado com segurança).
- Desconto de atacado: 10% quando a quantidade é **maior que 24** unidades.
- Reposição: para cada bebida abaixo do mínimo informado, sugere pedir `mínimo × 2 − estoque atual`.
- Entradas lidas com `fgets` + `sscanf` (em vez de `scanf`), para o programa não travar com texto inválido.
- Ao iniciar, o programa conta as linhas do arquivo, aloca o vetor com esse tamanho e então carrega os dados.
- O vetor é manipulado por ponteiro duplo (`Bebida **`) quando precisa crescer ou diminuir, pois `realloc` pode mudar o endereço.

## Testes realizados

- Compilação com `-Wall -Wextra -Wpedantic` sem avisos.
- Valgrind e AddressSanitizer sem erros de memória e sem vazamentos.
- Ciclo salvar → carregar com 300 registros.
- Arquivos inexistente, vazio, sem quebra de linha final e com linha corrompida.
- Entradas inválidas (texto no lugar de número, datas inexistentes, estoque insuficiente).
