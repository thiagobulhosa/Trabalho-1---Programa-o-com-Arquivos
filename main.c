//Thiago Dos Santos
//Thiago Bulhosa
//Marina Bulhosa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

typedef struct Palavra
{
    char letras[50];
    int qtdOcorrencias;
    int *linhas;
    struct Palavra *prox;
} palavra;

palavra *indice = NULL;

void imprimePalavra(palavra *Xindice)
{
    int tamPalavra=0;
    palavra *aux = (palavra *)malloc(sizeof(palavra));
    aux = Xindice;
    printf("\n");
    while(aux != NULL)
    {
        tamPalavra=strlen(aux->letras);
        tamPalavra++;
        printf("Tamanho da palavra:%d, Palavra:%s, numero de ocorrencias:%d nas Linhas:",tamPalavra,aux->letras,aux->qtdOcorrencias);
        for(int i=0; i<aux->qtdOcorrencias; i++)
        {
            printf("%d,",(aux->linhas[i]));
        }
        printf("\n");
        aux = aux->prox;
    }
}

void addOrdem(palavra *NovaPalavra)
{
    palavra *aux = (palavra *)malloc(sizeof(palavra));

    if(indice == NULL)
    {
        NovaPalavra->prox = NULL;
        indice = NovaPalavra;
    }
    else if(strcmp(NovaPalavra->letras,indice->letras)<0)
    {
        NovaPalavra->prox = indice;
        indice = NovaPalavra;
    }
    else
    {
        aux = indice;
        while(aux->prox && (strcmp(NovaPalavra->letras,aux->prox->letras)>0))
            aux= aux->prox;
        NovaPalavra->prox = aux->prox;
        aux->prox = NovaPalavra;
    }
}

void addPalavra(char *palavraArq,int Nlinha)
{
    palavra *NovaPalavra;
    NovaPalavra=(palavra *)malloc(sizeof(palavra));
    int *linha;
    linha=(int *)malloc(sizeof(int));
    linha[0]=Nlinha;
    strcpy(NovaPalavra->letras, palavraArq);
    NovaPalavra->qtdOcorrencias = 1;
    NovaPalavra->linhas = linha;
    NovaPalavra->prox = NULL;
    addOrdem(NovaPalavra);
}

int comparaPalavras(char *palavraArq)
{
    palavra *aux;
    aux=(palavra *)malloc(sizeof(palavra));
    aux=indice;
    {
        while(aux!=NULL)
        {
            int compara=strcmp(palavraArq,aux->letras);
            if (compara==0)
            {
                return 1;
            }
            aux=aux->prox;
        }
    }
    return 0;
}

void addOcorrencia(char *token,int cont)
{
    palavra *aux = (palavra *)malloc(sizeof(palavra));
    int *linha;
    aux=indice;
    while (aux!=NULL)
    {
        if (strcmp(aux->letras,token)==0)
        {
            aux->qtdOcorrencias++;
            linha=(int *)realloc(aux->linhas,aux->qtdOcorrencias*sizeof(int));
            linha[aux->qtdOcorrencias-1]=cont;
            aux->linhas=linha;
        }
        aux=aux->prox;
    }
}

void salvaIndice(int palavrasTotais)
{
    int tamPalavra, ax;
    FILE *arq;
    palavra *aux;
    aux=indice;
    arq=fopen("indice.dat","wb");
    if (arq==NULL)
        printf("Erro!Nao foi possivel abrir o arquivo :(\n");
    else
    {
        fwrite(&palavrasTotais,sizeof(int),1,arq);

        while (aux!=NULL)
        {
            tamPalavra=strlen(aux->letras);
            tamPalavra++;
            fwrite(&tamPalavra,sizeof(int),1,arq);
            fwrite(aux->letras,sizeof(char),tamPalavra,arq);
            fwrite(&aux->qtdOcorrencias,sizeof(int),1,arq);
            for (int i=0; i<aux->qtdOcorrencias; i++)
            {
                ax = aux->linhas[i];
                fwrite(&ax,sizeof(int),1,arq);
            }
            aux=aux->prox;
            tamPalavra=0;
        }
        fclose(arq);
    }
}

void limpaLista()
{
    palavra *aux;
    while(indice!=NULL)
    {
        aux=indice->prox;
        free(indice->linhas);
        free(indice);
        indice=aux;
    }
}

palavra* addPalavra2(palavra *Nindice,palavra *NovaPalavra)
{
    palavra *aux=Nindice;
    if(Nindice == NULL)
        return NovaPalavra;

    while(Nindice->prox!=NULL)
        Nindice=Nindice->prox;
    Nindice->prox=NovaPalavra;
    return aux;
}

palavra *recuperaIndice()
{
    int ocorrencia,palavrasTotais,tamPalavra;
    palavra *NovoIndice = NULL;
    FILE *arq;
    arq=fopen("indice.dat","rb");
    if (arq==NULL)
        printf("Erro!Nao é possivel abrir o arquivo :(\n");
    else
    {
        fread(&palavrasTotais,sizeof(int),1,arq);
        printf("Palavras totais: %d\n",palavrasTotais);
        for (int i=0; i<palavrasTotais; i++)
        {
            palavra *novaPalavra = (palavra*)malloc(sizeof(palavra));
            fread(&tamPalavra,sizeof(int),1,arq);
            fread(novaPalavra->letras,sizeof(char),tamPalavra,arq);
            fread(&ocorrencia,sizeof(int),1,arq);
            novaPalavra->qtdOcorrencias = ocorrencia;
            novaPalavra->linhas = malloc(novaPalavra->qtdOcorrencias * sizeof(int));
            int oc;
            for(int i=0; i<novaPalavra->qtdOcorrencias; i++)
            {
                fread(&oc,sizeof(int),1,arq);
                novaPalavra->linhas[i]=oc;
            }
            novaPalavra->prox=NULL;
            NovoIndice = addPalavra2(NovoIndice, novaPalavra);

        }
        fclose(arq);
    }
    return NovoIndice;
}

void leArquivo (char *nomeArq)
{
    int palavrasTotais=0;
    int compara=0;
    int cont=1;
    FILE *arq;
    arq=fopen(nomeArq,"r");
    char palavraArq[1000],*token;
    if (arq==NULL)
    {
        printf("Erro! Nao foi possivel abrir o arquivo :(\n");
    }
    else
    {
        printf("Arquivo lido e indice criado.\n");
        while (fgets(palavraArq, 1000,arq)!=NULL)
        {
            token=strtok(palavraArq," ,!.\n\0");
            while (token!=NULL)
            {
                for(int i = 0; i<strlen(token); i++)
                {
                    token[i] = tolower(token[i]);
                }
                compara=comparaPalavras(token);
                if (compara==0)
                {
                    addPalavra(token,cont);
                    palavrasTotais++;
                }
                else
                    addOcorrencia(token,cont);
                token=strtok(NULL," ,!.\n\0");
            }
            cont++;
        }
        fclose(arq);
    }
    salvaIndice(palavrasTotais);
}

void printMenu()
{
    printf("1-Criar indice\n");
    printf("2-Buscar indice\n");
    printf("3-sair\n");
    printf("Digite uma opcao:");
}

void procuraPalavra(char *pv, palavra *Xindice)
{
    while(Xindice != NULL)
    {
        if(strcmp(pv, Xindice->letras)==0)
        {
            printf("a palavra:%s tem %d ocorrencias nas linhas :",Xindice->letras,Xindice->qtdOcorrencias);
            for(int i=0; i<Xindice->qtdOcorrencias; i++)
                printf("%d,",Xindice->linhas[i]);
            printf("\n");

            return;
        }
        Xindice=Xindice->prox;
    }
    printf("\nPalavra NAO encontrada\n");
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    char palavraUsuario[20];
    int op=0;
    char nomeArq[100];
    palavra *IndiceArq;
    while (op!=4)
    {
        switch (op)
        {
        case 1:
            printf("Digite o nome do arquivo:");
            scanf("%s",nomeArq);
            leArquivo(nomeArq);
            op=0;
            break;
        case 2:
            limpaLista();
            IndiceArq = recuperaIndice();
            imprimePalavra(IndiceArq);
            printf("\nDigite a palavra que deseja pesquisar: ");
            scanf(" %[^\n]s", palavraUsuario);
            procuraPalavra (palavraUsuario,IndiceArq);
            op=0;
            break;
        case 3:
            printf ("Programa finalizado\n");
            limpaLista();
            printf ("Listas limpas\n");
            exit(1);
        default:
            printMenu();
            scanf("%d", &op);
        }
    }
    return 0;
}
