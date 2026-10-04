#ifndef BEBIDA_H
#define BEBIDA_H


typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int codigo;
    char nome[50];
    char categoria[20];
    int volumeMl;
    float preco;
    int quantidadeEstoque;
    Data validade;
} Bebida;

Bebida *cadastrarBebida(int codigo, const char *nome, const char *categoria, int volumeMl, float preco, int quantidade, Data validade);
void atualizarQuantidadeEstoque(Bebida *item, int quantidadeEstoque);
int venderBebida(Bebida *b, int quantidade);
void receberCarregamento(Bebida *b, int quantidadeRecebida, int dia, int mes, int ano);
float aplicarDescontoAtacado(Bebida *b, int quantidade);

#endif