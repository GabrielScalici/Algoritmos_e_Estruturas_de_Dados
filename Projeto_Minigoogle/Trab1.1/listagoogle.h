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

//funcoes
char *leStrArq(FILE *file);
int leArquivo();
boolean insere_ultimo(SITE *site, LISTA *lista);
boolean insere_ordenado(SITE *site, LISTA *lista);
LISTA *criar_lista();
void exibir_lista(LISTA * lista);
boolean remover_site(LISTA * lista, int id);
boolean atualizar_relevancia(LISTA * lista, int id);
boolean inserir_palavra_chave(LISTA * lista,int id_analise, char * nova);
void busca_palavra_chave(LISTA * lista, char * palavra_busca);
char *lerNome(FILE *digitado);
char *readString();
void inserir_site(LISTA * lista);

#endif