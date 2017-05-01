#include <stdio.h>
#include <stdlib.h>
#include "listagoogle.h"

int main(int argc, char *argv[]){
	LISTA * lista = criar_lista();
	leArquivo(lista);

 	inserir_site(lista);

 	exibir_lista(lista);

	free(lista);		

	return 0;
}



