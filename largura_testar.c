/*
ObservaÃ§Ã£o:
fazer 2 execuÃ§Ãµes
em uma delas Ã© para observar o comportamento (a cada 50 execuÃ§Ãµes imprime o tabuleiro)
na outra Ã© para comentar a parte da impressÃ£o (eu indiquei por "COMENTAR" no cÃ³digo)
para medir o tempo, pois o tempo imprimindo o tabuleiro a cada 50 execuÃ§Ãµes Ã© muito diferente
do tempo sem imprimir
*/

//inclusÃ£o de bibliotecas
//inclui funÃ§Ãµes de arquivos, exibiÃ§Ã£o e leitura
#include <stdio.h>
//inclui funÃ§Ãµes de alocaÃ§Ã£o dinÃ¢mica de memÃ³ria
#include <stdlib.h>
#include <time.h>
int vezes=0;
//uso de fila para armazenar as coordenadas
//estrutura usada na fila Ã© definida como Elemf
struct Elemf
{
    struct lista** li;
    //ligaÃ§Ã£o entre os elementos da pilha
    struct Elemf* next;
};

//estrutura que armazena o inÃ­cio e o fim da fila
typedef struct fila
{
    struct Elemf* inic;
    struct Elemf* fim;
}
Fila;

//uso de lista duplamente encadeada para armazenar as coordenadas
//estrutura usada na lista Ã© definida como Eleml
typedef struct lista
{
    //nÃºmero da linha do elemento
    int el;
    //nÃºmero da coluna do elemento
    int ec;
    //ligaÃ§Ã£o entre os elementos da lista
    struct lista* next;
    struct lista* ant;
}
Eleml;

//o ponteiro para a estrutura Ã© definido por Lista
typedef Eleml* Lista;

//estrutura que serÃ¡ usada para armazenar as informaÃ§Ãµes da matriz
struct info
{
    //nÃºmero de linhas da matriz
    int l;
    //nÃºmero de colunas da matriz
    int c;
    //nÃºmero da linha do inicial
    int il;
    //nÃºmero da coluna do inicial
    int ic;
};

//cabeÃ§alhos das funÃ§Ãµes
//cria-se uma estrutura info com atributos nulos
struct info* cria_info();
//transforma um arquivo texto em uma matriz e armazena as informaÃ§Ãµes numa estrutura
char** matriz(struct info* info);
//inicializador da busca em largura, seus parÃ¢metros sÃ£o uma matriz e uma estrutura info
void busca_larg(char** mat, struct info* info);
//cabeÃ§alhos das funÃ§Ãµes da fila
//cria uma fila para armazenar as coordenadas
Fila* cria_fila();
//insere um elemento no fim da fila
//os parÃ¢metros sÃ£o uma fila e as coordenadas do elemento
void insere_fila(Fila* fi, int el, int ec);
//retorna a linha do elemento do inÃ­cio da fila
int linha_fila(Fila* fi,struct info* info);
//retorna a coluna do elemento do inÃ­cio da fila
int coluna_fila(Fila* fi,struct info* info);
//remove o elemento do inÃ­cio da fila
void remove_fila(Fila* fi);
//libera o espaÃ§o alocado para uma fila
void destruir_fila(Fila* fi);
//cria uma lista duplamente encadeada vazia para armazenar as coordenadas
Lista* cria_lista();
//insere um elemento no inÃ­cio da lista
//os parÃ¢metros sÃ£o o inÃ­cio da lista e as coordenadas do elemento
void insere_lista(Lista* li, int el, int ec);
//gera uma lista igual a outra
Lista* copia_lista(Lista* li);
//exibe uma lista do elemento inicial ao final
void exibe_lista(Lista* li);
//libera o espaÃ§o alocado para uma lista
void destruir_lista(Lista* li);

//implementaÃ§Ãµes das funÃ§Ãµes
//cria uma estrutura info com valores iniciais 0 para armazenar as informaÃ§Ãµes da matriz
struct info* cria_info()
{
    //declara-se e aloca-se memÃ³ria para um ponteiro de estrutura info
    struct info* info = (struct info*)malloc(sizeof(struct info));
    //se nÃ£o foi possÃ­vel alocar memÃ³ria, retorna-se
    if(!info) return 0;
    //se nÃ£o, atribui-se 0 para todos os atributos
    info->l = 0;
    info->c = 0;
    info->il = 0;
    info->ic = 0;
    //retorna-se o ponteiro para estrutura inicial
    return info;
}

//transforma um arquivo texto em uma matriz
//o parÃ¢metro Ã© uma estrutura na qual serÃ£o armazenadas as informaÃ§Ãµes do elemento inicial
char** matriz(struct info* info)
{
    //declara-se uma string para armazenar o nome do arquivo
    char nomarq[50];
    //pede-se para o usuÃ¡rio digitar o nome do arquivo
    printf("Nome do arquivo: ");
    //lÃª o nome do arquivo pelo teclado e armazena em nomarq
    scanf("%s", nomarq);
    //declara-se um ponteiro do tipo FILE que Ã© usado para abrir o arquivo para leitura
    FILE* fp = fopen(nomarq, "r");
    //lÃª-se o nÃºmero de linhas e armazena em l
    fscanf(fp, "%d", &(info->l));
    //lÃª-se o nÃºmero de colunas e armazena em c
    fscanf(fp, "%d", &(info->c));
    //declara-se e aloca-se memÃ³ria para um ponteiro de ponteiro de caracter do tamanho de uma coluna da matriz
    char** mat = (char**)malloc((info->l)*sizeof(char*));
    //para cada coluna aloca-se memÃ³ria para uma linha da matriz
    for(register int i = 0; i < (info->l); i++) mat[i] = (char*)malloc((info->c)*sizeof(char));
    //lÃª-se e armazena-se os dados
    for(register int i=0; i<(info->l); i++)
    {
        fscanf(fp, "\n");
        for(register int j=0; j<(info->c); j++) fscanf(fp, "%c", &(mat[i][j]));
    }
    //usa-se funÃ§Ã£o de busca em matriz para encontrar o elemento inicial
    //percorre-se o arquivo procurando o elemento inicializador
    int i,j;
    for(i=0,j=0;i<(info->l); i++) for(j=0; j<=(info->c); j++)if(mat[i][j]=='#')
    {
        //armazena-se o nÃºmero da linha do inicial em il
        info->il=i;
        //armazena-se o nÃºmero da coluna do inicial em ic
        info->ic=j;
    }
    //se o usuÃ¡rio iniciar fora da matriz, dÃ¡ erro
    if((info->il) > (info->l) || (info->ic) > (info->c)) return 0;
    //fim da leitura, fecha-se o arquivo
    fclose(fp);
    //retorna-se o endereÃ§o do primeiro item da matriz
    return mat;
}

//busca em largura, seus parÃ¢metros sÃ£o uma matriz e uma estrutura info
void busca_larg(char** mat, struct info* info)
{
    //declara-se um ponteiro de estrutura descritora de fila
    Fila* fila = cria_fila();
    //insere-se o elemento inicial na fila
    insere_fila(fila,info->il,info->ic);
    //troca-se o caracter do inicial para nÃ£o ser visitado novamente
    (mat[info->il][info->ic]) = '-';
    //as coordenadas iniciais do laÃ§o serÃ£o o elemento inicial
    int al=info->il,ac=info->ic;
    while(fila->inic && mat[linha_fila(fila,info)][coluna_fila(fila,info)]!='$')
    {
        vezes+=1;
        //COMENTAR
        /*
        if(vezes%50==0)
        {
            for(int i=0;i<(info->l); i++)
            {
                for(int j=0; j<(info->c); j++)printf("%c", mat[i][j]);
                printf("\n");
            }
            printf("\n");
        }*/
        //COMENTAR
        al=linha_fila(fila,info);
        ac=coluna_fila(fila,info);
        //verificaÃ§Ã£o para norte
        //verificaÃ§Ã£o de borda: se a linha atual for a primeira, nÃ£o se pode ir para norte
        //verificaÃ§Ã£o de caracter: se o elemento a norte nÃ£o for visitÃ¡vel, nÃ£o serÃ¡ aplicada a recursÃ£o
        if(al-1 >= 0) if((mat[al-1][ac]) != '-') 
        {
            //troca-se o caracter se ele nÃ£o for o destino para nÃ£o ser visitado mais de uma vez
            if(mat[al-1][ac] != '$')(mat[al-1][ac]) = '-';            
            insere_fila(fila, al-1, ac);
        }
        //verificaÃ§Ã£o para leste
        //verificaÃ§Ã£o de borda: se a coluna atual for a Ãºltima, nÃ£o se pode ir para leste
        //verificaÃ§Ã£o de caracter: se o elemento a leste nÃ£o for visitÃ¡vel, nÃ£o serÃ¡ aplicada a recursÃ£o
        if((ac+1)<(info->c)) if((mat[al][ac+1])!='-') 
        {
            //troca-se o caracter se ele nÃ£o for o destino para nÃ£o ser visitado mais de uma vez
            if(mat[al][ac+1] != '$')(mat[al][ac+1]) = '-';        
            insere_fila(fila, al, ac + 1);
        }
        //verificaÃ§Ã£o para sul
        //verificaÃ§Ã£o de borda: se a linha atual for a Ãºltima nÃ£o se pode ir para sul
        //verificaÃ§Ã£o de caracter: se o elemento a sul nÃ£o for visitÃ¡vel, nÃ£o serÃ¡ aplicada a recursÃ£o
        if(al+1 < info->l) if((mat[al+1][ac]) != '-') 
        {
            //troca-se o caracter se ele nÃ£o for o destino para nÃ£o ser visitado mais de uma vez
            if(mat[al+1][ac]!='$')(mat[al+1][ac]) = '-';
            insere_fila(fila, al+1, ac);
        }
        //verificaÃ§Ã£o para oeste
        //verificaÃ§Ã£o de borda: se a coluna atual for a primeira, nÃ£o se pode ir para oeste
        //verificaÃ§Ã£o de caracter: se o elemento a oeste nÃ£o for visitÃ¡vel, nÃ£o serÃ¡ aplicada a recursÃ£o
        if(ac-1 >= 0) if ((mat[al][ac-1]) != '-') 
        {
            //troca-se o caracter se ele nÃ£o for o destino para nÃ£o ser visitado mais de uma vez
            if(mat[al][ac-1] != '$')(mat[al][ac-1]) = '-';
            insere_fila(fila, al, ac - 1);
        }
        //terminadas as verificaÃ§Ãµes, retira-se o elemento atual da fila
        remove_fila(fila);
    }
    
    //se nÃ£o encontrou o objetivo, envia-se mensagem de erro e retorna
    if(!(fila->inic))
    {
        printf("O objetivo nao foi encontrado\n");
        return;
    }
    //imprime-se a lista de coordenadas
    exibe_lista(fila->inic->li);
    //libera-se a memÃ³ria alocada pela fila
    destruir_fila(fila);
    //fim da execuÃ§Ã£o
}

//implementaÃ§Ãµes das funÃ§Ãµes da fila
//cria uma fila vazia para armazenar as coordenadas
Fila* cria_fila()
{
    //declara-se e aloca memÃ³ria para um ponteiro de estrutura descritora de fila
    Fila* fi = (Fila*)malloc(sizeof(Fila));
    //se nÃ£o foi possÃ­vel alocar memÃ³ria, retorna-se 0 para sinalizar erro
    if (!fi) return 0;
    //se nÃ£o, o inÃ­cio e o fim da fila aponta para uma lista vazia
    fi->inic= NULL;
    fi->fim=NULL;
    //retorna-se o ponteiro de ponteiro de elemento de pilha que indica o inÃ­cio dela
    return fi;
}


//insere os elementos no fim da fila
//os parÃ¢metros sÃ£o a estrutura descritora da fila e as coordenadas do elemento
void insere_fila(Fila* fi, int el, int ec)
{
    //se a fila nÃ£o existe, retorna-se
    if(!fi) return;
    //cria-se uma nova lista para ser inserida na fila
    Lista* nova_lista=cria_lista();
    //cria-se um ponteiro auxiliar para percorrer a lista atual
    if(fi->inic)
    {
        //primeiro se chega ao final da lista
        Lista aux=*(fi->inic->li),ant=NULL;
        while(aux)
        {
            ant=aux;
            aux=aux->next;
        }
        //entÃ£o copia-se seu conteÃºdo Ã  nova lista
        while(ant)
        {
            insere_lista(nova_lista,ant->el,ant->ec);
            ant=ant->ant;
        }
    }
    //por fim, insere-se a coordenada desejada
    insere_lista(nova_lista,el,ec);
    //cria-se um novo elemento de fila
    struct Elemf* novo_fila=(struct Elemf*)malloc(sizeof(struct Elemf));
    //como a inserÃ§Ã£o Ã© no fim, next Ã© vazio
    novo_fila->next=NULL;
    //a lista desse elemento de fila Ã© a nova lista
    novo_fila->li=nova_lista;
    if((fi->inic)==NULL)fi->inic=novo_fila;
    //adiciona o novo elemento Ã  fila 
    else fi->fim->next=novo_fila;
    //agora o fim aponta para o novo elemento
    fi->fim=novo_fila;
    //se a fila estiver vazia, o inicio tambem sera o novo elemento
}

//retorna a linha do primeiro elemento da fila
int linha_fila(Fila* fi, struct info* info)
{
    if(!fi||!(fi->inic))return info->il;
    return (*(fi->inic->li))->el;
}

//retorna a coluna do primeiro elemento da fila
int coluna_fila(Fila* fi, struct info* info)
{
    if(!fi||!(fi->inic))return info->ic;
    return (*(fi->inic->li))->ec;
}

//remove o primeiro elemento da fila
void remove_fila(Fila* fi)
{
    //se a fila nÃ£o existe ou estÃ¡ vazia, retorna-se
    if(!fi||!(fi->inic))return;
    //se nÃ£o, um elemento auxiliar aponta para o inÃ­cio da fila
    struct Elemf* aux=(fi->inic);
    //agora o inÃ­cio da fila Ã© o prÃ³ximo em relaÃ§Ã£o ao inÃ­cio
    (fi->inic)=(fi->inic)->next;
    //destroi-se a lista contida no auxiliar
    destruir_lista(aux->li);
    //libera-se a memÃ³ria alocada para o auxiliar
    free(aux);
}

//destrÃ³i uma fila, o parÃ¢metro Ã© a estrutura descritora da fila
void destruir_fila(Fila* fi)
{
    //se a fila nÃ£o existe, retorna-se
    if(!fi) return;
    //se nÃ£o, declara-se um ponteiro auxiliar chamado dtc
    //que serÃ¡ usado para destruir a fila
    struct Elemf* dtc;
    //enquanto a fila nÃ£o for vazia
    while(fi->inic)
    {
        //dtc aponta para o inÃ­cio da fila
        dtc = fi->inic;
        //o inÃ­cio da pilha aponta para seu prÃ³ximo elemento
        fi->inic = fi->inic->next;
        //destrÃ³i a lista 
        destruir_lista(dtc->li);
        free(dtc);
    }
    //libera-se a memÃ³ria alocada pelo inÃ­cio da fila
    free(fi);
    //fim da destruiÃ§Ã£o
}

//implementaÃ§Ãµes das funÃ§Ãµes da lista duplamente encadeada
//cria uma lista encadeada vazia para armazenar as coordenadas
Lista* cria_lista()
{
    //declara-se e aloca memÃ³ria para um ponteiro de ponteiro de elemento de lista
    Lista* li = (Lista*)malloc(sizeof(Lista));
    //se nÃ£o foi possÃ­vel alocar memÃ³ria, retorna-se 0 para sinalizar erro
    if (!li) return 0;
    //se nÃ£o, o inÃ­cio da lista aponta para o vazio
    *li = NULL;
    //retorna-se o ponteiro de ponteiro de elemento de lista que indica o inÃ­cio dela
    return li;
}

//insere um elemento numa lista
//os parÃ¢metros sÃ£o o inÃ­cio da lista e as coordenadas do elemento
void insere_lista(Lista* li, int el, int ec)
{
    //se a lista nÃ£o existe, retorna-se
    if(!li) return;
    //se nÃ£o, declara-se e aloca-se memÃ³ria para um ponteiro elemento de lista
    Lista novo = (Lista)malloc(sizeof(Eleml));
    //se nÃ£o foi possÃ­vel alocar memÃ³ria, retorna-se
    if (!novo) return;
    //se nÃ£o, ao novo elemento se atribui as coordenadas passadas como parÃ¢metros
    novo->el = el;
    novo->ec = ec;
    novo->ant=NULL;
    //se a lista estiver vazia
    if(!(*li))
    {
        //o ponteiro de ligaÃ§Ã£o do novo elemento aponta para o vazio
        novo->next = NULL;
        //o inÃ­cio da lista aponta para o novo elemento
        *li = novo;
        //fim da inserÃ§Ã£o
        return;
    }
    //se a lista nÃ£o estiver vazia
    //o ponteiro de ligaÃ§Ã£o do novo elemento aponta para cmp
    novo->next = *li;
    (*li)->ant=novo;
    //o ponteiro de ligaÃ§Ã£o de ant aponta para o novo elemento
    *li=novo;
    //fim da inserÃ§Ã£o
}

//exibe uma lista do elemento inicial ao final
void exibe_lista(Lista* li)
{
    //se a lista nÃ£o existe ou estÃ¡ vazia, retorna-se
    if(!li || !(*li)) return;
    //se nÃ£o, declara-se um ponteiro auxiliar para elemento de pilha chamado prt
    //que serÃ¡ usado para acessar a pilha e imprimi-la
    //o ponteiro prox auxilia prt
    Lista prox = *li, prt=NULL;
    int passos=0;
    //como queremos imprimir do elemento inicial ao final, primeiro chegamos ao fim da pilha
    while(prox)
    {
        prt=prox;
        prox=prox->next;
    }
    printf("[");
    //enquanto nÃ£o se chegar ao inÃ­cio da lista
    while(prt)
    {
        passos+=1;
        //imprime-se as coordenadas do elemento atual
        printf("(%d, %d)", prt->el, prt->ec);
        if(prt->ant)printf(",");
        //prt aponta para o elemento anterior da pilha, que Ã© o prÃ³ximo elemento do trajeto
        prt = prt->ant;
    }
    //fim da exibiÃ§Ã£o
    printf("]\n");
    printf("\nPassos: %d\n",passos);
}
    
//destrÃ³i uma lista, o parÃ¢metro Ã© o inÃ­cio de uma lista
void destruir_lista(Lista* li)
{
    //se a lista nÃ£o existe, retorna-se
    if(!li) return;
    //se nÃ£o, declara-se um ponteiro auxiliar chamado dtc
    //que serÃ¡ usado para destruir a lista
    Lista dtc;
    //enquanto a lista nÃ£o for vazia
    while(*li)
    {
        //dtc aponta para o inÃ­cio da lista
        dtc = *li;
        //o inÃ­cio da lista aponta para seu prÃ³ximo elemento
        *li = (*li)->next;
        //libera-se a memÃ³ria alocada pelo elemento apontado por dtc
        free(dtc);
    }
    //libera-se a memÃ³ria alocada pelo elemento restante
    free(*li);
    //libera-se a memÃ³ria alocada pelo inÃ­cio da lista
    free(li);
    //fim da destruiÃ§Ã£o
}
int main()
{
        //variáveis de tempo para medir o tempo de execução
    clock_t tempoi,tempof;
    //inicio da contagem de tempo
    tempoi=clock();
    //cria-se uma estrutura para armazenar as informaÃ§Ãµes da matriz
    struct info* info = cria_info();
    //declara-se um ponteiro de ponteiro de inteiro mat que armazena o
    //endereÃ§o do primeiro elemento da matriz criada
    char** mat = matriz(info);
    //aplica o algoritmo busca
    busca_larg(mat, info);
    //libera a memÃ³ria alocada por info e mat
    free(info);
    free(mat);
    //fim do programa
        //fim da contagem de tempo
    tempof=clock();
    printf("\nTempo de execucao: %.5f\n", ((float)(tempof-tempoi))/CLOCKS_PER_SEC);
    printf("\nVezes: %d\n",vezes);
    return 0;
}
