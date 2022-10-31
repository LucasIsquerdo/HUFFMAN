#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct tree
{
	int simbolo;
	int freq;
	struct tree *esq, *dir;
	
};
typedef struct tree Tree;

struct fila
{
	Tree *no;
	struct fila *prox;
};
typedef struct fila Fila;

struct registro
{
	int simbolo;
	char palavra[20];
	int frequencia;
	char codigo_huffman[20];
};
typedef struct registro Registro;

struct lista
{
	Registro reg;
	struct lista *prox;
};
typedef struct lista Lista;

void InsereFila(Fila **fila, Tree *no)
{
	Fila *novo;
	novo = (Fila*)malloc(sizeof(Fila));
	novo->prox = NULL;
	novo->no = no;
	if(*fila==NULL)
		*fila=novo;
	else
	{
		if((*fila)->no->freq >= no->freq)
		{
			novo->prox = *fila;
			*fila = novo;
		}
		else
		{
			Fila *aux = *fila;
			Fila *anterior;
			while(aux!=NULL && aux->no->freq<=no->freq)
			{
				anterior = aux;
				aux = aux->prox;
			}
			anterior->prox = novo;
			novo->prox = aux;
		}
	}
}

void InsereRegistro(Lista **lista, Lista *l)
{
	Lista *aux;
	if(*lista == NULL)
		*lista = l;
	else
	{
		aux = *lista;
		while(aux->prox!=NULL)
			aux = aux->prox;
		aux->prox = l;
	}
	
}

Tree *CriaNo(int simbolo, int freq, Tree *dir, Tree *esq)
{
	Tree *novo;
	novo = (Tree*)malloc(sizeof(Tree));
	novo->simbolo = simbolo;
	novo->freq = freq;
	novo->dir = dir;
	novo->esq = esq;
	
	return novo;
}

void constroiLista(Lista *lista, Fila **F)
{
	Tree *no;
	while(lista!=NULL)
	{
		no = CriaNo(lista->reg.simbolo, lista->reg.frequencia, NULL, NULL);
		InsereFila(&*F, no);
		lista = lista->prox;
	}
}

Lista *CriaCaixa(int simbolo, char palavra[], int f, char cod[])
{
	Lista *nova;
	nova = (Lista*)malloc(sizeof(Lista));
	nova->reg.frequencia = f;
	nova->reg.simbolo = simbolo;
	strcpy(nova->reg.codigo_huffman,cod);
	strcpy(nova->reg.palavra, palavra);

	return nova;
}
Lista *Busca(Lista *L, char aux[], Lista *palavra)
{
	while(L!=NULL && strcmp(L->reg.palavra,aux)!=0)
		L=L->prox;
	if(L=NULL)
		palavra = NULL;
	else
		palavra = L;
}
void Frequencia(char frase[100], Lista **lista, int *qtd)
{
	int i=0,j=0, cont=0;
	char aux[100];
	Lista *palavra;
	for(i=0;i<strlen(frase);i++)
	{
		if(frase[i]== ' ' )
		{
			strcpy(aux," ");
		}
		else
		{
			aux[0]='\0';
			j=0;
			while(i<strlen(frase) && frase[i]!=' ')
				aux[j++] = frase[i++];
			aux[j]='\0';
		}
		Busca(*lista,aux,&*palavra);
		if(palavra==NULL)
		{
			Lista *novo;
			novo = CriaCaixa(cont++,aux,1," ");
			InsereRegistro(&*lista,novo);
			qtd++;
		}
		else
			palavra->reg.frequencia++;

	}
}

void exibe(Lista *l)
{
	if(l==NULL)
		printf("Lista vazia");
	else
	{
		Lista *aux = l;
		while(aux!=NULL)
		{
			printf("%s",l->reg.palavra);
			aux = aux->prox;
		}
		
		
	}
	
}

void GravarRegistro(Lista *L)
{
	FILE *arq = fopen("Reg.dat","wb");
	if(L!=NULL)
	{
		while(L!=NULL)
		{
			fwrite(L,sizeof(Lista),1,arq);
			L= L->prox;
		}
		
	}
	fclose(arq);
}


void CodHuffman(Tree *raiz, Lista *lista, char cod[], char binario, int cont)
{
	Lista *aux;
	if(raiz!= NULL)
	{
		if(raiz->dir == raiz->esq == NULL)
			cod[cont]='\0';
		aux = lista;
		while(aux->reg.simbolo!=raiz->simbolo)
			aux = aux->prox;
		if(aux!=NULL)
			strcpy(aux->reg.codigo_huffman,cod);	
	}
	else
	{
		if(cont>=0)
			cod[cont]= binario;
		CodHuffman(raiz->esq,lista,cod,'0',cont+1);
		CodHuffman(raiz->dir,lista,cod,'1',cont+1);
	}
}

Fila *dequeue(Fila **F, int *qtd)
{
	Fila *aux = *F;
	*F = aux->prox;
	qtd--;
	return aux;
}

void Arvore(Fila **F, int *qtd)
{
	Fila *aux, *aux2;
	Tree *novo;
	int soma=0;
	while(*qtd>1)
	{
		aux = dequeue(&*F, &*qtd);
		aux2 = dequeue(&*F, &*qtd);
		soma = aux->no->freq + aux2->no->freq;
		novo = CriaNo(-1, soma, aux2->no, aux->no);
		InsereFila(&*F,novo);
		qtd++;
		
	}
}

void Codificar(Lista *l)
{
	Lista *aux = l;
	FILE *ptr = fopen("codigo.txt", "w");
	while(aux!=NULL)
	{
		if(aux->reg.codigo_huffman!=" ")
			fprintf(ptr,"%s",aux->reg.codigo_huffman);
		aux = aux->prox;
	}
	fclose(ptr);
}

int main()
{
	
	Lista *lista = NULL;
	Fila *fila = NULL;
	Tree *raiz= NULL;
	int cont=0;
	char cod[30];
	char binario;
	char frase[200] = "amo como ama o amor nao conheco nenhuma outra razao para amar senao amar que queres que te diga alem de que te amo se o que quero dizer e que te amo";
	Frequencia(frase, &lista,&cont);
	constroiLista(lista,&fila);
	Arvore(&fila,&cont);
	CodHuffman(raiz,lista,cod,binario,cont);
	GravarRegistro(lista);
	Codificar(lista);

	return 1;
}
