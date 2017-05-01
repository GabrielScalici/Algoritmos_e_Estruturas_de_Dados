/*
	AUTORES:  GABRIEL HENRIQUE CAMPOS SCALICI -- 9292970
			  KEITH TSUKADA SASAKI            -- 9293414
	PROGRAMA: Programa que simula o funcionamento do algoritmo de pesquisa do google
	DATA:     01-09-2015

*/

#ifndef LISTAGOOGLE_H
#define LISTAGOOGLE_H	 

//Definicoes para ajudar o programa
#define boolean int
#define TRUE 1
#define FALSE 0

//informacao do site
typedef struct info_sites{
	int id;
	char *nome;
	int rel;
	char *end;
	char **palavra;
	int i;
}SITE;

//informacao do no
typedef struct node{
	SITE * site; 
	struct node* prox;
}NO;

//informacao da lista
typedef struct listaordenada{
	NO * sentinela;
	int tam;
}LISTA;

//Funcoes
LISTA *criar_lista();
//Retornam boolean
boolean leArquivo();
boolean insere_ordenado(SITE *site, LISTA *lista);
boolean remover_site(LISTA * lista, int id);
boolean atualizar_relevancia(LISTA * lista, int id);
boolean inserir_palavra_chave(LISTA * lista,int id_analise, char * nova, int tam);
//Nao retornam
void exibir_lista(LISTA * lista);
void destroi_lista(LISTA * lista);
void free_site(SITE * site);
void busca_palavra_chave(LISTA * lista, char * palavra_busca);
void introducao();
void menu(LISTA * lista);
void inserir_site(LISTA* lista);
//Retornam uma string
char *readString();
char *leStrArq(FILE *file);
char *lerNome(int * tam);

#endif