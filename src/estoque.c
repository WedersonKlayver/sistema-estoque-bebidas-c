#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/estoque.h"

static int dataAnterior(Data a, int dia, int mes, int ano){
    if(a.ano != ano){
        return a.ano < ano;
    }
    if(a.mes != mes){
        return a.mes < mes;
    }
    return a.dia < dia;
}
Bebida *buscarPorCodigo(Bebida bebidas[], int quantidade, int codigoBuscado){
    for(int i = 0; i < quantidade; i++){
        if (codigoBuscado == bebidas[i].codigo){
            return &bebidas[i];
        }
        
    }
    return NULL;
}

int removerBebida(Bebida **bebidas, int *quantidade, int codigo){
    int indice = -1;
    for(int i= 0; i < *quantidade; i++){
        if((*bebidas)[i].codigo == codigo){
            indice = i;
            break;
        }
    }

    if(indice == -1){
        return 0;
    }

    for(int i = indice; i < *quantidade - 1; i++){
        (*bebidas)[i] = (*bebidas)[i + 1];
    }
    (*quantidade)--;

    if(*quantidade == 0){
        free(*bebidas);
        *bebidas = NULL;
    } else {
        Bebida *temp = realloc((*bebidas), (*quantidade) * sizeof(Bebida));
        if(temp != NULL){
            *bebidas = temp;
        }
    }
    return 1;
}

void salvarBebidas(Bebida bebidas[], int quantidade, char *nomeArquivo){
    FILE *arq = fopen(nomeArquivo, "w" );
    if(arq == NULL){
        printf("Erro ao abrir o arquivo. Tente novamente!\n");
        return;
    }
    for(int i = 0; i < quantidade; i++){
        fprintf(arq, "%d;%s;%s;%.2f;%d;%d;%d;%d;%d\n", bebidas[i].codigo, bebidas[i].nome, bebidas[i].categoria, bebidas[i].preco ,bebidas[i].volumeMl, bebidas[i].quantidadeEstoque, bebidas[i].validade.dia, bebidas[i].validade.mes, bebidas[i].validade.ano);
    }
    fclose(arq);
}

int carregarBebidas(Bebida bebidas[], char *nomeArquivo){
    int n = 0;

    FILE *arq = fopen(nomeArquivo, "r");
    if(arq == NULL){
        return 0;
    }
    while(fscanf(arq, "%d;%49[^;];%19[^;];%f;%d;%d;%d;%d;%d\n", &bebidas[n].codigo, bebidas[n].nome, bebidas[n].categoria, &bebidas[n].preco, &bebidas[n].volumeMl, &bebidas[n].quantidadeEstoque, &bebidas[n].validade.dia, &bebidas[n].validade.mes, &bebidas[n].validade.ano) == 9){
        n++;
    }
    fclose (arq);
    return n;
    
}

void liberarBebidas(Bebida **bebidas, int *quantidade){
    free(*bebidas);
    *bebidas = NULL;
    *quantidade = 0;
}

void gerarPedidoReposicaoAutomatico(Bebida bebidas[], int quantidade, int quantidadeMinima){
     int itens = 0;
 
    printf("\n===== PEDIDO DE REPOSICAO (minimo: %d) =====\n", quantidadeMinima);
    for(int i = 0; i < quantidade; i++){
        if(bebidas[i].quantidadeEstoque < quantidadeMinima){
            int pedir = quantidadeMinima * 2 - bebidas[i].quantidadeEstoque;
            printf("Codigo %d - %s (%d ml) | estoque: %d | pedir: %d unidades\n",
                   bebidas[i].codigo, bebidas[i].nome, bebidas[i].volumeMl,
                   bebidas[i].quantidadeEstoque, pedir);
            itens++;
        }
    }
    if(itens == 0){
        printf("Nenhuma bebida abaixo do minimo.\n");
    } else {
        printf("Total de itens no pedido: %d\n", itens);
    }
}

void relatorioBebidasVencidas(Bebida bebidas[], int quantidade, int diaAtual, int mesAtual, int anoAtual){
     int vencidas = 0;
 
    printf("\n===== BEBIDAS VENCIDAS em %02d/%02d/%d =====\n", diaAtual, mesAtual, anoAtual);
    for(int i = 0; i < quantidade; i++){
        if(dataAnterior(bebidas[i].validade, diaAtual, mesAtual, anoAtual)){
            printf("Codigo %d - %s | validade: %02d/%02d/%d | estoque: %d\n",
                   bebidas[i].codigo, bebidas[i].nome,
                   bebidas[i].validade.dia, bebidas[i].validade.mes, bebidas[i].validade.ano,
                   bebidas[i].quantidadeEstoque);
            vencidas++;
        }
    }
    if(vencidas == 0){
        printf("Nenhuma bebida vencida.\n");
    }
}

float valorTotalPorCategoria(Bebida bebidas[], int quantidade, char *categoria){
    float total = 0;
 
    for(int i = 0; i < quantidade; i++){
        if(strcmp(bebidas[i].categoria, categoria) == 0){
            total += bebidas[i].preco * bebidas[i].quantidadeEstoque;
        }
    }
    return total;
}

void listarTodos(Bebida bebidas[], int quantidade){
    if(quantidade == 0){
        printf("Nao ha nenhuma Bebida Cadastrada\n");
        return;
    }
    for(int i = 0; i < quantidade; i++){
        printf("--------------------------------------------------\n");
        printf("Codigo da Bebida: %d\n", bebidas[i].codigo);
        printf("Nome: %s\n", bebidas[i].nome);
        printf("Categoria: %s\n", bebidas[i].categoria);
        printf("Volume em Ml: %d\n", bebidas[i].volumeMl);
        printf("Preco: %.2f\n", bebidas[i].preco);
        printf("Quantidade em estoque: %d\n", bebidas[i].quantidadeEstoque);
        printf("Validade: %02d/%02d/%d\n", bebidas[i].validade.dia, bebidas[i].validade.mes, bebidas[i].validade.ano);
    }
}

int adicionarAoVetor(Bebida **bebidas, int *quantidade, Bebida novoBebida){
    Bebida *temp = realloc(*bebidas, (*quantidade + 1) * sizeof(Bebida));
    if(temp == NULL){
        return 0;
    }
    *bebidas = temp;

    (*bebidas)[*quantidade] = novoBebida;
    (*quantidade)++;
    return 1;
}

int contarLinhas(char *nomeArquivo){
    FILE *arq = fopen(nomeArquivo, "r");
    if(arq == NULL){
        return 0;
    }
    int linhas = 0;
    int c;
    int ultimo = '\n';
 
    while((c = fgetc(arq)) != EOF){
        if(c == '\n'){
            linhas++;
        }
        ultimo = c;
    }
    if(ultimo != '\n'){
        linhas++;
    }
    fclose (arq);
    return linhas;
}
