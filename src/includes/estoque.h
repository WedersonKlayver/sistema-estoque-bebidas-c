#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "bebida.h"

Bebida *buscarPorCodigo(Bebida bebidas[], int quantidade, int codigoBuscado);
int removerBebida(Bebida **bebidas, int *quantidade, int codigo);
void salvarBebidas(Bebida bebidas[], int quantidade, char *nomeArquivo);
int carregarBebidas(Bebida bebidas[], char *nomeArquivo);
void liberarBebidas(Bebida **bebidas, int *quantidade);
void gerarPedidoReposicaoAutomatico(Bebida bebidas[], int quantidade, int quantidadeMinima);
void relatorioBebidasVencidas(Bebida bebidas[], int quantidade, int diaAtual, int mesAtual, int anoAtual);
float valorTotalPorCategoria(Bebida bebidas[], int quantidade, char *categoria);
void listarTodos(Bebida bebidas[], int quantidade);
int adicionarAoVetor(Bebida **bebidas, int *quantidade, Bebida novoBebida);
int contarLinhas(char *nomeArquivo);

#endif