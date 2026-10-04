#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/bebida.h"

#define QTD_MIN_ATACADO 24
#define DESCONTO_ATACADO 0.9f


Bebida *cadastrarBebida(int codigo, const char *nome, const char *categoria, int volumeMl, float preco, int quantidade, Data validade){
    Bebida *newBebida = (Bebida *)malloc(sizeof(Bebida));
    if(newBebida != NULL) {
        newBebida->codigo = codigo;
        strncpy(newBebida->nome, nome, 49);
        newBebida->nome[49] = '\0';
        strncpy(newBebida->categoria, categoria, 19);
        newBebida->categoria[19] = '\0'; 
        newBebida->volumeMl = volumeMl;
        newBebida->preco = preco;
        newBebida->quantidadeEstoque = quantidade;
        newBebida->validade = validade;
    }
    return newBebida;
}

void atualizarQuantidadeEstoque(Bebida *item, int novaEstoque){
    if(item == NULL){
        return;
    }
    if(novaEstoque < 0){
        printf("Quantidade invalida!! Tente novamente.\n");
        return;
    }
    item->quantidadeEstoque = novaEstoque;
}

int venderBebida(Bebida *b, int quantidade){
    if(b == NULL || quantidade <= 0){
        return -1;
    }
    if(b->quantidadeEstoque < quantidade){
        return -1;
    }
    b->quantidadeEstoque -= quantidade;
    return (int)(b->preco * quantidade);
}

void receberCarregamento(Bebida *b, int quantidadeRecebida, int dia, int mes, int ano){
    if(b == NULL || quantidadeRecebida <= 0){
        return;
    }
    b->quantidadeEstoque += quantidadeRecebida;
    b->validade.dia = dia;
    b->validade.mes = mes;
    b->validade.ano = ano;
}

float aplicarDescontoAtacado(Bebida *b, int quantidade){
    float valor = b->preco * quantidade;
    if(quantidade > QTD_MIN_ATACADO){
        float valDesconto = valor * DESCONTO_ATACADO;
        return valDesconto;
    } else 
        return valor;
    
}