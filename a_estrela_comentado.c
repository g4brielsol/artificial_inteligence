/*
Busca informada A*
Descrição do algoritmo:
Transforma-se um arquivo texto em matriz e trabalha-se com o conceito de agenda que é implementada em fila ordenada de lista
Observa-se os elementos vizinhos em sentido horário
Se o valor atual não é um caracter visitável, simplesmente é ignorado e observa-se o próximo
Se o próximo valor é um caracter visitável, armazena-se as coordenadas numa lista dentro da fila e troca-se o caracter por um não-visitável
Para se inserir na fila, calcula-se a f do elemento, sendo a soma do número de passos percorridos até o elemento
e a distância de Manhattan entre o elemento e o objetivo
Antes de avançar, verifica-se se está numa borda, pois não se pode exceder os limites da matriz
Quando nenhum outro valor ao redor do atual for visitÃ¡vel,
Ele é excluído da fila e acessa-se o próximo da fila, procurando se há valores visitáveis até chegar no destino
Então imprime-se as coordenadas dos valores listados, em ordem do inicial ao final
Caso a fila termine e não se encontrou o objetivo, imprime-se mensagem de erro e encerra-se a execução
*/

//inclusão de bibliotecas
//inclui funções de arquivos, exibição e leitura
#include <stdio.h>
//inclui funções de alocação dinâmica de memória
#include <stdlib.h>

//uso de fila para armazenar as coordenadas
//estrutura usada na fila é definida como Elemf
struct Elemf
{
    //cada elemento da fila será o início de uma lista
    struct lista** li;
    //para tornar a busca informada, são levados em consideração f,g e h dos elementos
    //sendo g a quantidade de passos dadas até o elemento
    //h a previsão baseada na distância de Manhattan entre o elemento e o objetivo
    //e f a soma de g e h
    int f,g,h;
    //ligação entre os elementos da pilha
    struct Elemf* next;
};

typedef struct Elemf* Fila;

//uso de lista duplamente encadeada para armazenar as coordenadas
//estrutura usada na fila é definida como Eleml
typedef struct lista
{
    //número da linha do elemento
    int el;
    //número da coluna do elemento
    int ec;
    //ligação entre os elementos da pilha
    struct lista* next;
    struct lista* ant;
}
Eleml;

//o ponteiro para a estrutura é definido por Lista
typedef Eleml* Lista;

//estrutura que será usada para armazenar as informações da matriz
struct info
{
    //número de linhas da matriz
    int l;
    //número de colunas da matriz
    int c;
    //número da linha do inicial
    int il;
    //número da coluna do inicial
    int ic;
    //número da linha do objetivo
    int ol;
    //número da coluna do objetivo
    int oc;
};

//cabeçalhos das funções
//cria-se uma estrutura info com atributos nulos
struct info* cria_info();
//transforma um arquivo texto em uma matriz e armazena as informações numa estrutura
char** matriz(struct info* info);
//inicializador da busca em largura, seus parâmetros são uma matriz e uma estrutura info
void busca_A(char** mat, struct info* info);
//cabeçalhos das funções da fila
//cria uma fila para armazenar as coordenadas
Fila* cria_fila();
//insere um elemento no fim da fila
//os parâmetros são uma fila e as coordenadas do elemento
void insere_fila(Fila* fi, int el, int ec, struct info* info);
//retorna a linha do elemento do início da fila
int linha_fila(Fila* fi,struct info* info);
//retorna a coluna do elemento do início da fila
int coluna_fila(Fila* fi,struct info* info);
//remove o elemento do início da fila
void remove_fila(Fila* fi);
//libera o espaço alocado para uma fila
void destruir_fila(Fila* fi);
//cria uma lista duplamente encadeada vazia para armazenar as coordenadas
Lista* cria_lista();
//insere um elemento no início da lista
//os parâmetros são o início da lista e as coordenadas do elemento
void insere_lista(Lista* li, int el, int ec);
//gera uma lista igual a outra
Lista* copia_lista(Lista* li);
//exibe uma lista do elemento inicial ao final
void exibe_lista(Lista* li);
//libera o espaço alocado para uma lista
void destruir_lista(Lista* li);

//implementações das funções
//cria uma estrutura info com valores iniciais 0 para armazenar as informações da matriz
struct info* cria_info()
{
    //declara-se e aloca-se memória para um ponteiro de estrutura info
    struct info* info = (struct info*)malloc(sizeof(struct info));
    //se não foi possível alocar memória, retorna-se
    if(!info) return 0;
    //se não, atribui-se 0 para todos os atributos
    info->l = 0;
    info->c = 0;
    info->il = 0;
    info->ic = 0;
    info->ol = 0;
    info->oc = 0;
    //retorna-se o ponteiro para estrutura inicial
    return info;
}

//transforma um arquivo texto em uma matriz
//o parâmetro é uma estrutura na qual serão armazenadas as informações do elemento inicial
char** matriz(struct info* info)
{
    //declara-se uma string para armazenar o nome do arquivo
    char nomarq[50];
    //pede-se para o usuário digitar o nome do arquivo
    printf("Nome do arquivo: ");
    //lê-se o nome do arquivo pelo teclado e armazena em nomarq
    scanf("%s", nomarq);
    //declara-se um ponteiro do tipo FILE que é usado para abrir o arquivo para leitura
    FILE* fp = fopen(nomarq, "r");
    //lê-se o número de linhas e armazena em l
    fscanf(fp, "%d", &(info->l));
    //lê-se o número de colunas e armazena em c
    fscanf(fp, "%d", &(info->c));
    //declara-se e aloca-se memória para um ponteiro de ponteiro de caracter do tamanho de uma coluna da matriz
    char** mat = (char**)malloc((info->l)*sizeof(char*));
    //para cada coluna aloca-se memória para uma linha da matriz
    for(register int i = 0; i < (info->l); i++) mat[i] = (char*)malloc((info->c)*sizeof(char));
    //lê-se e armazena-se os dados
    for(register int i=0; i<(info->l); i++)
    {
        fscanf(fp, "\n");
        for(register int j=0; j<(info->c); j++) fscanf(fp, "%c", &(mat[i][j]));
    }
    //usa-se função de busca em matriz para encontrar o elemento inicial
    //percorre-se o arquivo procurando o elemento inicializador
    int i,j;
    for(i=0,j=0;i<(info->l); i++) for(j=0; j<=(info->c); j++)if(mat[i][j]=='#')
    {
        //armazena-se o número da linha do inicial em il
        info->il=i;
        //armazena-se o número da coluna do inicial em ic
        info->ic=j;
    }
    for(i=0,j=0;i<(info->l); i++) for(j=0; j<=(info->c); j++)if(mat[i][j]=='$')
    {
        //armazena-se o número da linha do inicial em il
        info->ol=i;
        //armazena-se o número da coluna do inicial em ic
        info->oc=j;
    }
    //se o usuário iniciar fora da matriz, dá erro
    if((info->il) > (info->l) || (info->ic) > (info->c)) return 0;
    //fim da leitura, fecha-se o arquivo
    fclose(fp);
    //retorna-se o endereço do primeiro item da matriz
    return mat;
}

//busca em largura, seus parâmetros são uma matriz e uma estrutura info
void busca_A(char** mat, struct info* info)
{
    //declara-se um ponteiro de estrutura descritora de fila
    Fila* fila = cria_fila();
    //insere-se o elemento inicial na fila
    insere_fila(fila,info->il,info->ic,info);
    //troca-se o caracter para não ser visitado novamente
    (mat[info->il][info->ic]) = '-';
    //as coordenadas iniciais do laço serão do elemento inicial
    int al=info->il, ac=info->ic;
    while(*fila && mat[linha_fila(fila,info)][coluna_fila(fila,info)]!='$')
    {
        //atualiza-se os valores das coordenadas recebendo os valores do início da fila
        al=linha_fila(fila,info);
        ac=coluna_fila(fila,info);
        //verificação para norte
        //verificação de borda: se a linha atual for a primeira, não se pode ir para norte
        //verificação de caracter: se o elemento a norte não for visitável, não será aplicada a recursão
        if(al-1 >= 0) if((mat[al-1][ac]) != '-') 
        {
            //troca-se o caracter se ele não for o destino para não ser visitado mais de uma vez
            if(mat[al-1][ac] != '$')(mat[al-1][ac]) = '-';            
            insere_fila(fila, al-1, ac,info);
        }
        //verificação para leste
        //verificação de borda: se a coluna atual for a última, não se pode ir para leste
        //verificação de caracter: se o elemento a leste não for visitável, não será aplicada a recursão
        if((ac+1)<(info->c)) if((mat[al][ac+1])!='-') 
        {
            //troca-se o caracter se ele não for o destino para não ser visitado mais de uma vez
            if(mat[al][ac+1] != '$')(mat[al][ac+1]) = '-';        
            insere_fila(fila, al, ac + 1,info);
        }
        //verificação para sul
        //verificação de borda: se a linha atual for a última não se pode ir para sul
        //verificação de caracter: se o elemento a sul não for visitável, não será aplicada a recursão
        if(al+1 < info->l) if((mat[al+1][ac]) != '-') 
        {
            //troca-se o caracter se ele não for o destino para não ser visitado mais de uma vez
            if(mat[al+1][ac]!='$')(mat[al+1][ac]) = '-';
            insere_fila(fila, al+1, ac,info);
        }
        //verificação para oeste
        //verificação de borda: se a coluna atual for a primeira, não se pode ir para oeste
        //verificação de caracter: se o elemento a oeste não for visitável, não será aplicada a recursão
        if(ac-1 >= 0) if ((mat[al][ac-1]) != '-') 
        {
            //troca-se o caracter se ele não for o destino para não ser visitado mais de uma vez
            if(mat[al][ac-1] != '$')(mat[al][ac-1]) = '-';
            insere_fila(fila, al, ac - 1,info);
        }
        //terminadas as verificações, retira-se o elemento atual da fila
        remove_fila(fila);
    }
    
    //se não encontrou o objetivo, envia-se mensagem de erro e retorna
    if(!(*fila))
    {
        printf("O objetivo nao foi encontrado\n");
        return;
    }
    //imprime-se a lista de coordenadas
    exibe_lista((*fila)->li);
    //libera-se a memória alocada pela fila
    destruir_fila(fila);
    //fim da execução
}

//implementações das funções da fila
//cria uma fila vazia para armazenar as coordenadas
Fila* cria_fila()
{
    //declara-se e aloca memória para um ponteiro de estrutura descritora de fila
    Fila* fi = (Fila*)malloc(sizeof(Fila));
    //se não foi possível alocar memória, retorna-se 0 para sinalizar erro
    if (!fi) return 0;
    //se não, o início da fila aponta para o vazio
    *fi = NULL;
    //retorna-se o ponteiro de ponteiro de elemento de pilha que indica o início dela
    return fi;
}

//insere os elementos em ordem da fila
//os parâmetros são a estrutura descritora da fila e as coordenadas do elemento
void insere_fila(Fila* fi, int el, int ec, struct info* info)
{
    //se a fila não existe, retorna-se
    if(!fi) return;
    //cria-se uma nova lista para ser inserida na fila
    Lista* nova_lista=cria_lista();
    //cria-se um ponteiro auxiliar para percorrer a lista atual
    if(*fi)
    {
        //primeiro se chega ao final da lista
        Lista aux=*((*fi)->li),ant=NULL;
        while(aux)
        {
            ant=aux;
            aux=aux->next;
        }
        //então copia-se seu conteúdo à nova lista
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
    //a lista desse elemento de fila é a nova lista
    novo_fila->li=nova_lista;
    //para calcular h, serão somadas a distância horizontal e a distância vertical
    int hl, hc;
    //queremos valor absoluto portanto verificamos se a distância deu negativa
    if(((info->ol)-el)>=0)hl=(info->ol)-el;
    else hl=el-(info->ol);
    if(((info->oc)-ec)>=0) hc=(info->oc)-ec;
    else hc=ec-(info->oc);
    //os valores são atribuídos
    novo_fila->h=hl+hc;
    novo_fila->f=novo_fila->g + novo_fila->h;
    //se a fila estiver vazia
    if(!(*fi))
    {
        //como é o primeiro elemento, não foi dado um passo ainda
        (novo_fila->g)=0;
        //o próximo é o vazio
        novo_fila->next=NULL;
        //esse é o início da fila
        *fi=novo_fila;
        return;
        //fim da inserção
    }
    //se a fila não estiver vazia
    //o valor de passos atual é incrementado
    novo_fila->g=(*fi)->g + 1;
    //é preciso encontrar o lugar certo para a inserção
    //se a fila só tem um elemento
    if(!((*fi)->next))
    {
        //o próximo é o vazio, pois o primeiro elemento da fila será retirado
        novo_fila->next=NULL;
        //o elemento é adicionado à fila
        (*fi)->next=novo_fila;
        //fim da inserção
        return;
    }
    //se a fila tiver mais de 1 elemento, usam-se dois ponteiros auxiliares
    Fila ant = (*fi)->next, aux=NULL;
    //compara-se os valores de f para fazer a inserção
    while(ant && ant->f <= novo_fila->f)
    {
        aux=ant;
        ant=ant->next;
    }
    //se a inserção for logo após o início da fila
    if(ant==(*fi)->next)
    {
        //o próximo do novo elemento aponta para o próximo após o início
        novo_fila->next=(*fi)->next;
        //o elemento é adicionado à fila
        (*fi)->next=novo_fila;
        //fim da inserção
        return;
    }
    //se não, o próximo do elemento aponta para ant
    novo_fila->next=ant;
    //e o próximo de aux aponta para o elemento
    aux->next=novo_fila;
    //fim da inserção
}

//retorna a linha do primeiro elemento da fila
int linha_fila(Fila* fi, struct info* info)
{
    if(!fi||!(*fi))return info->il;
    return (*((*fi)->li))->el;
}

//retorna a coluna do primeiro elemento da fila
int coluna_fila(Fila* fi, struct info* info)
{
    if(!fi||!(*fi))return info->ic;
    return (*((*fi)->li))->ec;
}

//remove o primeiro elemento da fila
void remove_fila(Fila* fi)
{
    //se a fila não existe ou está vazia, retorna-se
    if(!fi||!(*fi))return;
    //se não, um ponteiro auxiliar aponta para o início da fila
    struct Elemf* aux=(*fi);
    //agora o início será o próximo
    *fi=(*fi)->next;
    //a lista do início é destruída
    destruir_lista(aux->li);
    //a memória alocada para o elemento é liberada
    free(aux);
    //fim da remoção
}

//destrói uma fila, o parâmetro é a estrutura descritora da fila
void destruir_fila(Fila* fi)
{
    //se a fila não existe ou está vazia, retorna-se
    if(!fi||!(*fi)) return;
    //se não, declara-se um ponteiro auxiliar chamado dtc
    //que será usado para destruir a fila
    struct Elemf* dtc;
    //enquanto a fila não for vazia
    while(*fi)
    {
        //dtc aponta para o início da fila
        dtc = *fi;
        //o início da pilha aponta para seu próximo elemento
        *fi = (*fi)->next;
        //destrói a lista 
        destruir_lista(dtc->li);
        free(dtc);
    }
    //libera-se a memória alocada pelo início da fila
    free(fi);
    //fim da destruição
}
//implementações das funções da lista duplamente encadeada
//cria uma lista encadeada vazia para armazenar as coordenadas
Lista* cria_lista()
{
    //declara-se e aloca memória para um ponteiro de ponteiro de elemento de lista
    Lista* li = (Lista*)malloc(sizeof(Lista));
    //se não foi possível alocar memória, retorna-se 0 para sinalizar erro
    if (!li) return 0;
    //se não, o início da lista aponta para o vazio
    *li = NULL;
    //retorna-se o ponteiro de ponteiro de elemento de lista que indica o início dela
    return li;
}

//insere um elemento numa lista
//os parâmetros são o início da lista e as coordenadas do elemento
void insere_lista(Lista* li, int el, int ec)
{
    //se a lista não existe, retorna-se
    if(!li) return;
    //se não, declara-se e aloca-se memória para um ponteiro elemento de lista
    Lista novo = (Lista)malloc(sizeof(Eleml));
    //se não foi possível alocar memória, retorna-se
    if (!novo) return;
    //se não, ao novo elemento se atribui as coordenadas passadas como parâmetros
    novo->el = el;
    novo->ec = ec;
    novo->ant=NULL;
    //se a lista estiver vazia
    if(!(*li))
    {
        //o ponteiro de ligação do novo elemento aponta para o vazio
        novo->next = NULL;
        //o início da lista aponta para o novo elemento
        *li = novo;
        //fim da inserção
        return;
    }
    //se a lista não estiver vazia
    //o ponteiro de ligação do novo elemento aponta para cmp
    novo->next = *li;
    (*li)->ant=novo;
    //o ponteiro de ligação de ant aponta para o novo elemento
    *li=novo;
    //fim da inserção
}

//exibe uma lista do elemento inicial ao final
void exibe_lista(Lista* li)
{
    //se a lista não existe ou está vazia, retorna-se
    if(!li || !(*li)) return;
    //se não, declara-se um ponteiro auxiliar para elemento de pilha chamado prt
    //que será usado para acessar a pilha e imprimi-la
    //o ponteiro prox auxilia prt
    Lista prox = *li, prt=NULL;
    //como queremos imprimir do elemento inicial ao final, primeiro chegamos ao fim da pilha
    while(prox)
    {
        prt=prox;
        prox=prox->next;
    }
    printf("[");
    int count=0;
    //enquanto não se chegar ao início da lista
    while(prt)
    {
        //imprime-se as coordenadas do elemento atual
        printf("(%d, %d)", prt->el, prt->ec);
        count+=1;
        if(prt->ant)printf(",");
        //prt aponta para o elemento anterior da pilha, que é o próximo elemento do trajeto
        prt = prt->ant;
    }
    //fim da exibição
    printf("]\n");
}
    
//destrói uma lista, o parâmetro é o início de uma lista
void destruir_lista(Lista* li)
{
    //se a lista não existe, retorna-se
    if(!li) return;
    //se não, declara-se um ponteiro auxiliar chamado dtc
    //que será usado para destruir a lista
    Lista dtc;
    //enquanto a lista não for vazia
    while(*li)
    {
        //dtc aponta para o início da lista
        dtc = *li;
        //o início da lista aponta para seu próximo elemento
        *li = (*li)->next;
        //libera-se a memória alocada pelo elemento apontado por dtc
        free(dtc);
    }
    //libera-se a memória alocada pelo elemento restante
    free(*li);
    //libera-se a memória alocada pelo início da lista
    free(li);
    //fim da destruição
}
int main()
{
    //cria-se uma estrutura para armazenar as informações da matriz
    struct info* info = cria_info();
    //declara-se um ponteiro de ponteiro de inteiro mat que armazena o
    //endereço do primeiro elemento da matriz criada
    char** mat = matriz(info);
    //aplica o algoritmo busca
    busca_A(mat, info);
    //libera a memória alocada por info e mat
    free(info);
    free(mat);
    //fim do programa
    return 0;
}
