#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct huffman
{
	struct huffman *esq, *dir;
	int simbolo;
};
typedef struct huffman Huffman;

struct registro
{
	int simbolo;
	char palavra[20];
	int frequencia;
	char codigo_huffman[20];
	struct registro *prox;
};
typedef struct registro Registro;

void Inserir(Registro **reg, Registro *novo)
{
	if(*reg == NULL)
		*reg=novo;
	else
	{
		Registro *aux = *reg;
		while(aux->prox!=NULL)
			aux = aux->prox;
		aux->prox=novo;
		
	}
	
}

void LerArquivo(Registro **aux)
{
	Registro *novo;
	FILE *arq = fopen("Reg.dat", "rb");
	while(!feof(arq))
	{
		novo = (Registro*)malloc(sizeof(Registro));
		fread(novo, sizeof(Registro),1 , arq);
		novo->prox = NULL;
		Inserir(&*aux,novo);
		
	}
	fclose(arq);
}

Huffman *CriaNo(int simbolo,Huffman *dir, Huffman *esq)
{
	Huffman *novo;
	novo = (Huffman*)malloc(sizeof(Huffman));
	novo->simbolo = simbolo;
	novo->dir = dir;
	novo->esq = esq;
	
	return novo;
}

void CriaArvore(Registro *reg, Huffman **raiz)
{
	Huffman *aux;
	if(*raiz == NULL)
		*raiz = CriaNo(-1,NULL,NULL);
	while(reg!=NULL)
	{
		aux = *raiz;
		int i=0;
		while(i<strlen(reg->codigo_huffman))
		{
			if(reg->codigo_huffman[i]=='0')
			{
				if(aux->esq==NULL)
					aux->esq = CriaNo(reg->simbolo,NULL,NULL);
				aux = aux->esq;
			}
			else
			{
				if(aux->dir==NULL)
					aux->dir = CriaNo(reg->simbolo,NULL,NULL);
				aux = aux->dir;
			}
		}
		reg = reg->prox;
	}
	
}

void Busca(int simbolo, Registro *reg)
{
	if(reg!=NULL)
	{
		while(reg->prox!=NULL && reg->simbolo!=simbolo)
			reg = reg->prox;
		if(reg!=NULL)
			printf("%s",reg->palavra);
	}
}

void Transforma(Huffman *raiz, Registro *reg)
{
	Huffman *aux= raiz;
	char string[100];
	FILE *ptr = fopen("palavra.txt", "r");
	fscanf(ptr, "%s", &string);
	int i=0;
	while(i<strlen(string))
	{
		if(string[i]=='0')
			aux = aux->esq;
		else
			aux = aux->dir;
		if(aux->dir == aux->esq == NULL)
		{
			Busca(aux->simbolo,reg);
			aux = raiz;
		}
		i++;
	}
}

int main()
{
	Huffman *raiz = NULL;
	Registro *aux = NULL;
	LerArquivo(&aux);
	CriaArvore(aux,&raiz);
	Transforma(raiz,aux);
	
	return 1;
}
