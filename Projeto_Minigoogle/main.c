/*
	AUTORES:  GABRIEL HENRIQUE CAMPOS SCALICI -- 9292970
			  KEITH TSUKADA SASAKI            -- 9293414
	PROGRAMA: Programa que simula o funcionamento do algoritmo de pesquisa do google
	DATA:     01-09-2015

*/

#include <stdio.h>
#include <stdlib.h>
#include "listagoogle.h"

int main(int argc, char *argv[]){
	//Imprimindo a indroducao do programa (enfeite)
	introducao();

	LISTA *lista = criar_lista();

	//Ler os dados do arquivo csv
	leArquivo(lista);

	//Chamando o menu com as opcoes do minigoogle
	menu(lista);

	return 0;
}