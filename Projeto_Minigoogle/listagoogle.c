/*
	AUTORES:  GABRIEL HENRIQUE CAMPOS SCALICI -- 9292970
			  KEITH TSUKADA SASAKI            -- 9293414
	PROGRAMA: Programa que simula o funcionamento do algoritmo de pesquisa do google
	DATA:     01-09-2015

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listagoogle.h"

//Algumas definicoes para auxiliar na implementacao
#define boolean int
#define TRUE 1
#define FALSE 0

//Funcao para mostrar o nome do programa
void introducao(){
	printf("\n\n\t**************************************************\n");
 	printf("\t--------------------------------------------------\n");
 	printf("\t-----------------> MINIGOOGLE <-------------------\n");
 	printf("\t--------------------------------------------------\n");
 	printf("\t**************************************************\n\n");
}

//Funcao que faz o menu para acesso do usuario
void menu(LISTA *lista){
	int op = 0;
	int id;
	char * palavrachave = NULL;

	while(op != 7){
		id = 0;
		palavrachave = NULL;

	//Enfeite para o menu ficar mais interativo	
	printf("\n\n\t--------------------------------------------------\n");
 	printf("\t--------------------> MENU <----------------------\n");
 	printf("\t--------------------------------------------------\n");
 	printf("\t(1) : INSERIR NOVO SITE NA LISTA\n\t(2) : INSERIR PALAVRA-CHAVE\n\t(3) : REMOVER UM SITE\n");
 	printf("\t(4) : ATUALIZAR RELEVANCIA\n\t(5) : EXIBIR LISTA DE SITES\n\t(6) : BUSCA POR PALAVRA-CHAVE\n\t(7) : SAIR\n\n");

 	printf("\tDIGITE A OPCAO DESEJADA:");
 		scanf("%d", &op);
		if(op == 1){
			inserir_site(lista);
		}else if(op == 2){
			int tam = 0;
			printf("\tDIGITE O CODIGO DO SITE:");
			scanf("%d", &id);
			printf("\tDIGITE A PALAVRA-CHAVE PARA SER INSERIDA:");
			getchar();
			palavrachave = lerNome(&tam);
			inserir_palavra_chave(lista, id, palavrachave,tam);
		}else if(op == 3){
			printf("\tDIGITE O CODIGO DO SITE:");
			scanf("%d", &id);
			remover_site(lista, id);
		}else if(op == 4){
			printf("\tDIGITE O CODIGO DO SITE:");
			scanf("%d", &id);
			atualizar_relevancia(lista, id);
		}else if(op == 5){
			exibir_lista(lista);
		}else if(op == 6){
			int t = 0;
			printf("\tDIGITE A PALAVRA-CHAVE PARA SER BUSCADA: ");
			getchar();
			palavrachave = lerNome(&t);
			busca_palavra_chave(lista, palavrachave);
		}
	}
	destroi_lista(lista);
}

//FUncao para ler strings (tratando) digitadas pelo usuario
char *lerNome(int *tam){
	char *str = NULL;
	int i = 0;
	char ch;
	scanf("%c",&ch);
	while(ch != 10){//enquanto o char lido for diferente de '\n'
		str = (char *) realloc(str, (i+1) * sizeof(char)); //aloca espaco na memoria
		str[i] = ch; //insere o char lido no espaco alocado
		i++;
		scanf("%c",&ch);
	}
	*(tam) = i;
	str[i] = '\0'; //coloca o caractere nulo na ultima posicao do vetor indicando o fim da string
	return str;
}

//FUncao para ler os de um novo site (separando a cada virgula)
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

//Funcao que le do arquivo e coloca os dados em memoria alocada dinamicamente
boolean leArquivo(LISTA *lista){
	FILE *file = fopen("googlebot.csv","r");

	//VErificando se o arquivo exite
	if(file == NULL){
		printf("Arquivo nulo");
		return FALSE;
	}
	else {
	
 		char ch;
		int linhas = 0;

		//COntagem de linhas para facilitar algumas funcoes
		while(!feof(file)){
			ch = fgetc(file);
			if(ch == '\n')
				linhas++;
		}

		rewind(file); //move o cursor para o inicio do arquivo

		int count = 0;

		char *palavraChave = NULL;

		//Ate percorrer todas as linhas do arquivo 
		while(count < linhas){
			//CRiando um novo site para cada linha lida do arquivo
			SITE *site = (SITE *) malloc(sizeof(SITE));
			
			site->palavra = (char **) realloc(site->palavra, sizeof(char*)*(count+1));

			fscanf(file,"%d",&site->id);
			fgetc(file);
			site->nome = leStrArq(file);
			fscanf(file,"%d",&site->rel);
			fgetc(file);
			site->end = leStrArq(file);

			int ind = 0;

			ch = fgetc(file);
			int x = 0;

			//Caso especial para anotar as palavras-chave pois nao sabe quantas sao 
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
				palavraChave[x] = '\0';
				x=0;
				site->palavra[ind] = palavraChave;
				ind++;
				palavraChave = NULL;
			}
			site->i = ind;
			
			//Chamando funcao que insere o site ordenado em relacao aos que ja estao na memoria
			insere_ordenado(site,lista);

		 	count++;
		}

		fclose(file);
		free(palavraChave);
	}

	return TRUE;
}

//FUncao para criar a lista
LISTA *criar_lista(){
	//Alocando as structs necessarias
	LISTA *lista = (LISTA *) malloc(sizeof(LISTA)); 
	lista->sentinela = (NO *) malloc(sizeof(NO));
	lista->sentinela->prox = NULL;
	lista->tam = 0;

	return lista;
}

//FUncao usada para inserir o site ordenado
boolean insere_ordenado(SITE *site, LISTA *lista){
	NO * novoNo = (NO*)	malloc(sizeof(NO));
	
	//Enquanto nao chegar no ultimo no
	if(novoNo != NULL){	
		NO * aux = lista->sentinela; 
		
		//VErificando o rel de todos os nos
		while((aux->prox != NULL) && (aux->prox->site->rel > site->rel)){
			//PEgando os proximos nos salvos na memoria
			aux = aux->prox;
		}

		//Inserindo o novo na posicao correta
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
		
		//ENfeite para tornar mais interativo
		printf("\n\n\t-------------------------------------------------------\n");
 		printf("\t------------------> SITES CADASTRADOS <----------------\n");
 		printf("\t-------------------------------------------------------\n");

		NO * busca = NULL;
		int indice, j;

		//PEganbdo o primeiro valor depois da sentinela
		busca = lista->sentinela->prox;

		//PErcorrendo ate o fim do arquivo
		while(busca != NULL){
			//Imprimindo em uma linha igual o arquivo csv
			printf("\t%d,%s,%d,%s,", busca->site->id, busca->site->nome, busca->site->rel, busca->site->end);
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
	
	//Verificando se o no eh nulo
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

//Funcao para inserir uma palavra-chave a um site ja existente
boolean inserir_palavra_chave(LISTA * lista,int id_analise, char * nova,int tam){
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
				busca->site->palavra = (char**)realloc(busca->site->palavra, sizeof(char *) * (busca->site->i + 1));
				busca->site->palavra[busca->site->i] = (char*) realloc(busca->site->palavra[busca->site->i], sizeof(char) * tam);
				busca->site->palavra[busca->site->i] = nova;
				busca->site->i++;
				
				return TRUE;
			}

		}else
			busca = busca->prox; //PErcorrendo toda a lista
	}

	return FALSE;
}

//Funcao para atualizar a relevancia do site
boolean atualizar_relevancia(LISTA * lista, int id){
	NO * busca = NULL;
	int valor;
	
	//DAndo o valor da sentinela para fazer a busca
	busca = lista->sentinela->prox;

	//Verificando se nao voltou para o sentinela pois eh circular
	while(busca != NULL){
		//caso encontre o id anotado
		if(busca->site->id == id){
			//pedir para o usuario o valor para trocar a relevancia
			printf("\tDIGITE O VALOR:");
			scanf("%d", &valor);
			//Analisando se o valor é maior que o valor 1000
			if(valor > 1000){
				printf("\tO VALOR DIGITADO E MAIOR QUE O PERMITIDO");
				//Encerra a funcao
				return FALSE;
			}else{
				SITE *site = busca->site;
				//Anotando o novo valor da relevancia
				site->rel = valor;
				//CHamando funcoes para que insira corretamente
				remover_site(lista,id);
				insere_ordenado(site,lista);
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

//Funcao para destuir a lista, liberando a memoria alocada
void destroi_lista(LISTA * lista){
	NO * aux = lista->sentinela->prox;
	NO * p = NULL;

	//PErcorrendo todos e liberando um por um
	while(aux->prox != NULL){
		p = aux->prox;
		free(aux);
		aux= p;
	}
	//Liberando os demais 
	free(aux);
	aux = NULL;
	free(lista->sentinela);
	lista->sentinela = NULL;
	free(lista);
	lista = NULL;
}

//Funcao que realiza uma busca dependendo de determinada palavra-chave
void busca_palavra_chave(LISTA * lista, char * palavra_busca){
	NO * busca = lista->sentinela->prox;
	//Variaveis de auxilio na logia para pegar sites relacionados
	char * aux_relacionado = NULL, * aux_relacionado2 = NULL, * aux_relacionado3 = NULL;
	char * nome1, * nome2, * nome3;
	int j, help=0;

	printf("\t--------------------------------\n\t------ Sites encontrados -------\n\t--------------------------------\n");

	while(busca != NULL){
		for(j = 0; j < busca->site->i; j++){

			//printf("%s ---> %s\n\n", busca->site->palavra[j], palavra_busca);
			if(strcmp(palavra_busca,busca->site->palavra[j]) == 0){
				//Se achou, mostrar o site
				printf("\tNOME DO SITE: %s\n\tENDERECO NA WEB: %s\n\n", busca->site->nome, busca->site->end);

				//Como encontrou um site como o da pesquisa, anotar outras palavras chave para relacionados
				if(j < (busca->site->i - 1) && help == 0){
					//Salvando a proxima palavra-chave para usar nos sites relacionados
					aux_relacionado = busca->site->palavra[j+1];
					//Salvando o nome para que nos relacionados nao tenham sites repetidos 
					nome1 = busca->site->nome;
					//AUmentar o help para pegar mais de uma palavra para sites relacionados
					help++;
				}else if(j < (busca->site->i - 1) && help == 1 && strcmp(aux_relacionado,busca->site->palavra[j+1]) != 0){
					//Armazenando outra palavra para usar na busca por sites relacionados
					aux_relacionado2 = busca->site->palavra[j+1];
					nome2 = busca->site->nome;
					help++;
				}else if(j < (busca->site->i - 1) && help == 2 && strcmp(aux_relacionado,busca->site->palavra[j+1]) != 0 && strcmp(aux_relacionado2,busca->site->palavra[j+1]) != 0){
					aux_relacionado3 = busca->site->palavra[j+1];
					nome3 = busca->site->nome;
					help++;
				}
			}	
		}
		//Passando para a proxima palavra
		busca = busca->prox;
	}

	//Mostrando sites relacionados
	printf("\t--------------------------------\n------ Sites relacionados ------\n--------------------------------\n");

	//VOltando a busca ao primeiro no pára percorrer tudo novamente
	busca = lista->sentinela->prox;

	//Enquanto houver no na lista continue percorrendo
	while(busca != NULL){
		for(j = 0; j < busca->site->i; j++){

			//printf("%s ---> %s\n\n", busca->site->palavra[j], palavra_busca);
			if(strcmp(aux_relacionado,busca->site->palavra[j]) == 0 || strcmp(aux_relacionado2,busca->site->palavra[j]) == 0 || strcmp(aux_relacionado3,busca->site->palavra[j]) == 0){
				if(strcmp(nome1,busca->site->nome) != 0 && strcmp(nome2,busca->site->nome) != 0 && strcmp(nome3,busca->site->nome) != 0){
					//Se achou, mostrar o site
					printf("\tNOME DO SITE: %s\n\tENDERECO NA WEB: %s\n\n", busca->site->nome, busca->site->end);
				}
			}	
		}
		//Passando para a proxima palavra
		busca = busca->prox;
	}
}

//FUncao para ler uma string e separar na virgula
char *readString(){
	char *str = NULL;
	int i = 0;
	char ch;
	scanf("%c",&ch);
	while(ch != ','){//enquanto o char lido for diferente de '\n'
		str = (char *) realloc(str, (i+1) * sizeof(char)); //aloca espaco na memoria
		str[i] = ch; //insere o char lido no espaco alocado
		i++;
		scanf("%c",&ch);
	}
	str[i] = '\0'; //coloca o caractere nulo na ultima posicao do vetor indicando o fim da string
	return str;
}

//FUncao para inseirir um novo site na lista
void inserir_site(LISTA* lista){
	//Criando um novo no
	NO * busca = lista->sentinela->prox;
	SITE * novo = (SITE*)malloc(sizeof(SITE));
	int maior_id = 0;

	//Achando o maior id
	while(busca != NULL){
		//BUscar o maior id para que coloque incrementado no novo no
		if(busca->site->id > maior_id){
			//Troca toda vez que achar um maior
			maior_id = busca->site->id;
		}
		//Percorrer todos
		busca = busca->prox;
	}
	//Interacao com o usuario
	printf("\tDIGITE OS VALORES SEPARADOS POR VIRGULA:");
	//PEgando valores do novo site;
	novo->id = maior_id+1;
	getchar();

	//Ler os dados que o usuario deseja inserir
	novo->nome = readString();
	scanf("%d", &novo->rel);
	getchar();
	novo->end = readString();

	char ch;

	char *palavraChave = NULL;

	scanf("%c",&ch);

	int ind = 0,x=0;

	//LOgica para pegar todas as palavras-chave que o usuario digitar (maximo de 10)
	while(ch != 10){
		novo->palavra = (char **) realloc(novo->palavra, sizeof(char*)*(ind+1));
		while((ch != ',') && (ch != 10)){
			//Alocando na memoria heap
			palavraChave = (char *) realloc(palavraChave,sizeof(char)*(x+1));
			//Atribuind o valor na posicao correta da string 
			palavraChave[x] = ch;
			//Incremente parasalvar em outro local da memoria e aumentar um na alocacao
			x++;
			//PEgando o digitado
			scanf("%c",&ch);				
		}
		//alocando as palvras com base na quantidade ja cadastrada
		novo->palavra[ind] = (char *) malloc(sizeof(char)*x);
		if(ch != 10)
			scanf("%c",&ch);
		palavraChave[x] = '\0';				
		//ZErando para nao interfir nas demais vezes que rodar
		x=0;
		novo->palavra[ind] = palavraChave;
		ind++;
		//Atribuindo NULL para nao interferir nas proximas vezes que o programa rodar
		palavraChave = NULL;
	}
	//Atribuindo um auxliar para saber quantas alavras-chave tem
	novo->i = ind;

	//CHamando a funcao de inserir ordenamente na lista
	insere_ordenado(novo,lista);
}