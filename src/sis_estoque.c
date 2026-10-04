#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/estoque.h"

#define ARQUIVO "estoque_bebidas.txt"

static void lerTexto(const char *msg, char *dest, size_t tam){
    printf("%s", msg);
    if(fgets(dest, (int)tam, stdin) == NULL){
        printf("\nEntrada encerrada.\n");
        exit(1);
    }
    char *fim = strchr(dest, '\n');
    if(fim != NULL){
        *fim = '\0';
    } else {
        int ch;
        while((ch = getchar()) != '\n' && ch != EOF);
    }
}

static int lerInt(const char *msg){
    char buf[64];
    int valor;
 
    while(1){
        lerTexto(msg, buf, sizeof(buf));
        if(sscanf(buf, "%d", &valor) == 1){
            return valor;
        }
        printf("Valor invalido. Digite um numero inteiro.\n");
    }
}

static float lerFloat(const char *msg){
    char buf[64];
    float valor;
 
    while(1){
        lerTexto(msg, buf, sizeof(buf));
        if(sscanf(buf, "%f", &valor) == 1){
            return valor;
        }
        printf("Valor invalido. Use ponto como separador (ex: 9.50).\n");
    }
}

static int dataValida(Data d){
    int diasDoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
 
    if(d.ano < 2000 || d.ano > 2100 || d.mes < 1 || d.mes > 12 || d.dia < 1){
        return 0;
    }
    int bissexto = (d.ano % 4 == 0 && d.ano % 100 != 0) || d.ano % 400 == 0;
    int maximo = diasDoMes[d.mes - 1] + (d.mes == 2 && bissexto);
    return d.dia <= maximo;
}
 
static Data lerData(const char *titulo){
    Data d;
 
    printf("%s\n", titulo);
    do{
        d.dia = lerInt("  Dia: ");
        d.mes = lerInt("  Mes: ");
        d.ano = lerInt("  Ano: ");
        if(!dataValida(d)){
            printf("Data invalida. Tente novamente.\n");
        }
    } while(!dataValida(d));
    return d;
}
 
static int categoriaValida(const char *c){
    return strcmp(c, "refrigerante") == 0 || strcmp(c, "suco") == 0 ||
           strcmp(c, "agua") == 0 || strcmp(c, "cerveja") == 0;
}
 
static void lerCategoria(char *dest, size_t tam){
    do{
        lerTexto("Categoria (refrigerante, suco, agua ou cerveja): ", dest, tam);
        if(!categoriaValida(dest)){
            printf("Categoria invalida. Use letras minusculas e sem acento.\n");
        }
    } while(!categoriaValida(dest));
}

static void imprimirBebida(const Bebida *b){
    printf("--------------------------------------------------\n");
    printf("Codigo da Bebida: %d\n", b->codigo);
    printf("Nome: %s\n", b->nome);
    printf("Categoria: %s\n", b->categoria);
    printf("Volume em Ml: %d\n", b->volumeMl);
    printf("Preco: %.2f\n", b->preco);
    printf("Quantidade em estoque: %d\n", b->quantidadeEstoque);
    printf("Validade: %02d/%02d/%d\n", b->validade.dia, b->validade.mes, b->validade.ano);
    printf("--------------------------------------------------\n");
}

static Bebida *pedirEBuscar(Bebida *estoque, int qtd){
    int codigo = lerInt("Codigo da bebida: ");
    Bebida *b = buscarPorCodigo(estoque, qtd, codigo);
 
    if(b == NULL){
        printf("Bebida nao encontrada.\n");
    }
    return b;
}

static void opCadastrar(Bebida **estoque, int *qtd){
    char nome[100], categoria[100];
 
    int codigo = lerInt("Codigo: ");
    if(codigo <= 0){
        printf("O codigo deve ser maior que zero.\n");
        return;
    }
    if(buscarPorCodigo(*estoque, *qtd, codigo) != NULL){
        printf("Ja existe uma bebida com esse codigo.\n");
        return;
    }
    lerTexto("Nome: ", nome, sizeof(nome));
    if(nome[0] == '\0'){
        printf("O nome nao pode ficar vazio.\n");
        return;
    }
    if(strchr(nome, ';') != NULL){
        printf("O nome nao pode conter ponto e virgula (;).\n");
        return;
    }
    lerCategoria(categoria, sizeof(categoria));
    int volume = lerInt("Volume (ml): ");
    float preco = lerFloat("Preco (ex: 9.50): ");
    int quantidade = lerInt("Quantidade em estoque: ");
    if(volume <= 0 || preco <= 0 || quantidade < 0){
        printf("Valores invalidos. Cadastro cancelado.\n");
        return;
    }
    Data validade = lerData("Validade:");
 
    Bebida *nova = cadastrarBebida(codigo, nome, categoria, volume, preco, quantidade, validade);
    if(nova == NULL){
        printf("Erro: memoria insuficiente.\n");
        return;
    }
    if(adicionarAoVetor(estoque, qtd, *nova)){
        printf("Bebida cadastrada com sucesso!\n");
    } else {
        printf("Erro: nao foi possivel adicionar ao estoque.\n");
    }
    free(nova);
}
 
static void opBuscar(Bebida *estoque, int qtd){
    Bebida *b = pedirEBuscar(estoque, qtd);
    if(b != NULL){
        imprimirBebida(b);
    }
}

static void opAtualizar(Bebida *estoque, int qtd){
    Bebida *b = pedirEBuscar(estoque, qtd);
    if(b == NULL){
        return;
    }
    int nova = lerInt("Nova quantidade em estoque: ");
    atualizarQuantidadeEstoque(b, nova);
    if(nova >= 0){
        printf("Estoque atualizado.\n");
    }
}

static void opRemover(Bebida **estoque, int *qtd){
    int codigo = lerInt("Codigo da bebida a remover: ");
    if(removerBebida(estoque, qtd, codigo)){
        printf("Bebida removida.\n");
    } else {
        printf("Bebida nao encontrada.\n");
    }
}
 
static void opVender(Bebida *estoque, int qtd){
    Bebida *b = pedirEBuscar(estoque, qtd);
    if(b == NULL){
        return;
    }
    int quantidade = lerInt("Quantidade a vender: ");
    if(quantidade <= 0){
        printf("Quantidade invalida.\n");
        return;
    }
    float valor = aplicarDescontoAtacado(b, quantidade);
 
    if(venderBebida(b, quantidade) == -1){
        printf("Estoque insuficiente. Disponivel: %d unidades.\n", b->quantidadeEstoque);
    } else {
        printf("Venda realizada! Total: R$ %.2f\n", valor);
    }
}
 
static void opReceber(Bebida *estoque, int qtd){
    Bebida *b = pedirEBuscar(estoque, qtd);
    if(b == NULL){
        return;
    }
    int recebida = lerInt("Quantidade recebida: ");
    if(recebida <= 0){
        printf("Quantidade invalida.\n");
        return;
    }
    Data val = lerData("Validade do novo lote:");
    receberCarregamento(b, recebida, val.dia, val.mes, val.ano);
    printf("Carregamento registrado. Estoque atual: %d\n", b->quantidadeEstoque);
}
 
static void opAtacado(Bebida *estoque, int qtd){
    Bebida *b = pedirEBuscar(estoque, qtd);
    if(b == NULL){
        return;
    }
    int quantidade = lerInt("Quantidade da compra: ");
    if(quantidade <= 0){
        printf("Quantidade invalida.\n");
        return;
    }
    printf("Valor sem desconto: R$ %.2f\n", b->preco * quantidade);
    printf("Valor final:        R$ %.2f\n", aplicarDescontoAtacado(b, quantidade));
}
 
static void opReposicao(Bebida *estoque, int qtd){
    int minimo = lerInt("Quantidade minima desejada: ");
    if(minimo < 0){
        printf("Valor invalido.\n");
        return;
    }
    gerarPedidoReposicaoAutomatico(estoque, qtd, minimo);
}
 
static void opVencidas(Bebida *estoque, int qtd){
    Data hoje = lerData("Data de hoje:");
    relatorioBebidasVencidas(estoque, qtd, hoje.dia, hoje.mes, hoje.ano);
}
 
static void opValorCategoria(Bebida *estoque, int qtd){
    char categoria[100];
    lerCategoria(categoria, sizeof(categoria));
    printf("Valor total em estoque (%s): R$ %.2f\n", categoria,
           valorTotalPorCategoria(estoque, qtd, categoria));
}
 
int main(void){
    Bebida *estoque = NULL;
    int qtd = 0;             
 
    int n = contarLinhas(ARQUIVO);
    if(n > 0){
        estoque = malloc(n * sizeof(Bebida));
        if(estoque == NULL){
            printf("Erro: memoria insuficiente para carregar o estoque.\n");
            return 1;
        }
        qtd = carregarBebidas(estoque, ARQUIVO);
        printf("%d bebida(s) carregada(s) do arquivo.\n", qtd);
        if(qtd < n){
            printf("Aviso: o arquivo tem %d linha(s), mas so %d puderam ser lidas.\n", n, qtd);
        }
    }
 
    int opcao;
    do{
        printf("\n=========== SERTAO REFRESCANTE ===========\n");
        printf(" 1. Cadastrar bebida\n");
        printf(" 2. Listar todas as bebidas\n");
        printf(" 3. Buscar bebida por codigo\n");
        printf(" 4. Atualizar quantidade em estoque\n");
        printf(" 5. Remover bebida\n");
        printf(" 6. Vender bebida\n");
        printf(" 7. Receber carregamento\n");
        printf(" 8. Calcular compra com desconto de atacado\n");
        printf(" 9. Gerar pedido de reposicao automatico\n");
        printf("10. Relatorio de bebidas vencidas\n");
        printf("11. Valor total em estoque por categoria\n");
        printf(" 0. Salvar e sair\n");
        printf("==========================================\n");
        opcao = lerInt("Escolha uma opcao: ");
 
        switch(opcao){
            case 1:  
                opCadastrar(&estoque, &qtd);       
                break;
            case 2:  
                listarTodos(estoque, qtd);        
                break;
            case 3:  
                opBuscar(estoque, qtd);            
                break;
            case 4:  
                opAtualizar(estoque, qtd);         
                break;
            case 5:  
                opRemover(&estoque, &qtd);        
                break;
            case 6:  
                opVender(estoque, qtd);            
                break;
            case 7:  
                opReceber(estoque, qtd);     
                break;
            case 8:  
                opAtacado(estoque, qtd);           
                break;
            case 9:  
                opReposicao(estoque, qtd);         
                break;
            case 10:
                opVencidas(estoque, qtd);          
                break;
            case 11: 
                opValorCategoria(estoque, qtd);    
                break;
            case 0:                                      
                break;
            default: 
                printf("Opcao invalida!\n");
        }
    } while(opcao != 0);
 
    salvarBebidas(estoque, qtd, ARQUIVO);
    liberarBebidas(&estoque, &qtd);
    printf("Estoque salvo. Ate logo!\n");
    return 0;
}
 