#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listagoogle.h"

char *leStrArq(FILE *file){
	char ch;
	int tam = 0;
	char *str = NULL;
	ch = fgetc(file);
	while(ch != ','){
		str = (char *) realloc(str,sizeof(char)*(tam+1));
		str[tam] = ch;
		tam++;
		ch = fgetc(file);
	}
	str[tam] = '\0';
	return str;
}

int leArquivo(LISTA *lista){
	FILE *file = fopen("googlebot.csv","r");

	if(file == NULL){
		printf("Arquivo nulo");
		return 0;
	}
	else {
	
 		char ch;
		int linhas = 0;

		while(!feof(file)){
			ch = fgetc(file);
			if(ch == '\n')
				linhas++;
		}

		rewind(file);//move o cursor para o inicio do arquivo

		int count = 0;

		while(count < linhas){
			SITE *site = (SITE *) malloc(sizeof(SITE));
			
			site->palavra = (char **) realloc(site->palavra, sizeof(char*)*(count+1));

			fscanf(file,"%d",&site->id);
			fgetc(file);
			site->nome = leStrArq(file);
			fscanf(file,"%d",&site->rel);
			fgetc(file);
			site->end = leStrArq(file);

			int ind = 0;

			char *palavraChave = NULL;

			ch = fgetc(file);
			int x = 0;

			while(ch != 10){
				while((ch != ',') && (ch != 10)){
					palavraChave = (char *) realloc(palavraChave,sizeof(char)*(x+1));
					palavraChave[x] = ch;
					x++;
					ch = fgetc(file);					
				}
				site->palavra[ind] = (char *) malloc(sizeof(char)*x);
				if(ch != 10)
					ch = fgetc(file);
				x=0;
				site->palavra[ind] = palavraChave;
				ind++;
				palavraChave = NULL;
			}
			site->i = ind;
			insere_ordenado(site,lista);

		 	count++;
		}

		fclose(file);
	}
	return 1;
}

//FUncao para criar a lista
LISTA *criar_lista(){
	LISTA *lista = (LISTA *) malloc(sizeof(LISTA)); 
	lista->sentinela = (NO *) malloc(sizeof(NO));
	lista->sentinela->prox = NULL;
	lista->tam = 0;

	return lista;
}

boolean insere_ultimo(SITE *site, LISTA *lista){
	NO * novo = (NO*)malloc(sizeof(NO));

	if(novo != NULL){
		NO *aux = lista->sentinela;
		while(aux->prox != NULL){	
			aux = aux->prox;
		}

		novo->site = site;
		novo->prox = NULL;
		aux->prox = novo;
		lista->tam++;

		return TRUE;
	}
	return FALSE;
}

boolean insere_ordenado(SITE *site, LISTA *lista){
	NO * novoNo = (NO*)	malloc(sizeof(NO));
	if(novoNo != NULL){	
		NO * aux = lista->sentinela; 
	
		while((aux->prox != NULL) && (aux->prox->site->rel > site->rel)){
			aux = aux->prox;
		}

		novoNo->site = site;
		novoNo->prox = aux->prox;
		aux->prox = novoNo;
		lista->tam++;

		return TRUE;
	}
	else
		return FALSE;
}

//funcao para ver se a lista esta vazia
boolean vazia(LISTA * lista){
	if(lista->sentinela->prox == NULL){
		return TRUE;
	}
	return FALSE;
}

//Exibir lista
void exibir_lista(LISTA * lista){
	
	if(!vazia(lista)){
		NO * busca = NULL;
		int indice, j;

		//PEganbdo o primeiro valor depois da sentinela
		busca = lista->sentinela->prox;

		//PErcorrendo ate o fim do arquivo
		while(busca != NULL){
			//Imprimindo em uma linha igual o arquivo csv
			printf("%d,%s,%d,%s,", busca->site->id, busca->site->nome, busca->site->rel, busca->site->end);
			//Imprimir todas as palavras chave com base no indice
			indice = busca->site->i;
			//laco para imprimir todas as palvras com base no tamanho anotado pelo indice
			for(j=0; j <indice; j++){
				if(j != indice-1)
					printf("%s,", busca->site->palavra[j]);
				else
					printf("%s", busca->site->palavra[j]);
			}
			//Quando sair do for pular uma linha
			printf("\n");
			busca = busca->prox;
		}
	}
}

//FUncao para remover um site da lista
boolean remover_site(LISTA * lista, int id){
	NO * busca = NULL;
	//PAra anotar o valor do anterior
	NO * aux = NULL;

	//Dando o valor do primeiro no depois do sentinela
	busca = lista->sentinela->prox;
	aux = lista->sentinela;
	
	//Verificando se nao voltou para o sentinela pois eh circular
	while(busca != NULL){
		if(busca->site->id == id){
			aux->prox = busca->prox;
			free(busca);
			busca = NULL;
			return TRUE;
		}
		//Passando para os proximos valores da lista
		aux = busca;
		busca = busca->prox;
	}
	//CAso saiu significa que o no nao foi encontrado
	return FALSE;
}

//Funcao para atualizar a relevancia do site
boolean atualizar_relevancia(LISTA * lista, int id){
	NO * busca = NULL;
	int valor;
	
	//DAndo o valor da sentinela para fazer a busca
	busca = lista->sentinela->prox;

	//Verificando se nao voltou para o sentinela pois eh circular
	while(busca != lista->sentinela){
		//caso encontre o id anotado
		if(busca->site->id == id){
			//pedir para o usuario o valor para trocar a relevancia
			scanf("%d", &valor);
			//Analisando se o valor é maior que o valor 1000
			if(valor < 1000){
				printf("O VALOR DIGITADO E MAIOR QUE O PERMITIDO");
				//ENcerra a funcao
				return FALSE;
			}else{
				//Anotando o novo valor da relevancia
				busca->site->rel = valor;
				//Retornar true porque deu certo
				return TRUE;
			}
		}
		//Passando para os proximos valores da lista
		busca = busca->prox;
	}
	//CAso tenha saido, entao nao encontrou o valor do id digitado
	return FALSE;
}

boolean inserir_palavra_chave(LISTA * lista,int id_analise, char * nova){
	NO * busca = lista->sentinela->prox;

	while(busca != NULL){
		//Buscando pelo id informado
		if(busca->site->id == id_analise){
			//Saber se nao tem o maximo de palavras
			if(busca->site->i == 10){
				//Caso tenha o maximo de palavras possivel
				return FALSE;
			}else{
				//Caso contrario adicione a palavra-chave
				busca->site->i += 1;
				busca->site->palavra = (char**)realloc(busca->site->palavra, sizeof(char*) * (busca->site->i));
				busca->site->palavra[busca->site->i] = (char*)realloc(busca->site->palavra[busca->site->i], sizeof(char) * 50);
				busca->site->palavra[busca->site->i] = nova;
				
				printf("\n\n\n\n\n\n\n%s\n\n\n\n\n\n\n\n", busca->site->palavra[busca->site->i] );
				exibir_lista(lista);

				return TRUE;
			}

		}else{
			busca = busca->prox;
		}
	}

	return FALSE;
}

void busca_palavra_chave(LISTA * lista, char * palavra_busca){
	NO * busca = lista->sentinela->prox;
	char * aux_relacionado, * aux_relacionado2, * aux_relacionado3;
	int j, help=0;

	printf("--------------------------------\n------ Sites encontrados -------\n--------------------------------\n");

	while(busca != NULL){
		for(j = 0; j < busca->site->i; j++){

			//printf("%s ---> %s\n\n", busca->site->palavra[j], palavra_busca);
			if(strcmp(palavra_busca,busca->site->palavra[j]) == 0){
				//Se achou, mostrar o site
				printf("NOME DO SITE: %s\nENDERECO NA WEB: %s\n\n", busca->site->nome, busca->site->end);

				//Como encontrou um site como o da pesquisa, anotar outras palavras chave para relacionados
				if(j < (busca->site->i - 1) && help == 0){
					aux_relacionado = busca->site->palavra[j+1];
					help++;
				}else if(j < (busca->site->i - 1) && help == 1 && strcmp(aux_relacionado,busca->site->palavra[j+1]) != 0){
					aux_relacionado2 = busca->site->palavra[j+1];
					help++;
				}else if(j < (busca->site->i - 1) && help == 2 && strcmp(aux_relacionado,busca->site->palavra[j+1]) != 0 && strcmp(aux_relacionado2,busca->site->palavra[j+1]) != 0){
					aux_relacionado3 = busca->site->palavra[j+1];
					help++;
				}
			}	
		}
		//Passando para a proxima palavra
		busca = busca->prox;
	}

	printf("--------------------------------\n------ Sites relacionados ------\n--------------------------------\n");

	busca = lista->sentinela->prox;

	while(busca != NULL){
		for(j = 0; j < busca->site->i; j++){

			//printf("%s ---> %s\n\n", busca->site->palavra[j], palavra_busca);
			if(strcmp(aux_relacionado,busca->site->palavra[j]) == 0 || strcmp(aux_relacionado2,busca->site->palavra[j]) == 0 || strcmp(aux_relacionado3,busca->site->palavra[j]) == 0){
				//Se achou, mostrar o site
				printf("NOME DO SITE: %s\nENDERECO NA WEB: %s\n\n", busca->site->nome, busca->site->end);

			}	
		}
		//Passando para a proxima palavra
		busca = busca->prox;
	}

}