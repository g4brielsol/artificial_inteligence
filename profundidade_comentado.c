/*
Busca em profundidade
Descrição do algoritmo:
Transforma-se um arquivo texto em matriz
Visita-se os elementos vizinhos em sentido horário
Se o valor atual não é um caracter visitável, simplesmente é ignorado e visita-se o próximo
Se o próximo valor é um caracter visitável, armazena-se as coordenadas numa pilha duplamente encadeada, 
troca-se o caracter por um não-visitável e aplica-se a recursão
Antes de se aplicar recursão, verifica-se se está numa borda, pois não se pode exceder os limites da matriz
Quando nenhum outro valor ao redor do atual for visitável,
Volta-se ao anterior pela pilha, desempilhando-o e procurando se há valores visitáveis até chegar no destino
Então imprime-se as coordenadas dos valores que foram empilhados, usando o sentido inverso da pilha, 
para que a ordem seja do inicial ao final
Caso a recursão em todos os elementos termine e não se encontrou o objetivo, imprime-se mensagem de erro e encerra-se a execução
*/

//inclusão de bibliotecas
//inclui funções de arquivos, exibição e leitura
#include <stdio.h>
//inclui funções de alocação dinâmica de memória
#include <stdlib.h>

//é preciso uma flag para indicar se já se encontrou o objetivo
int flag=0;

//uso de pilha duplamente encadeada para armazenar as coordenadas
//estrutura usada na pilha é definida como Elem
typedef struct pilha
{
    //número da linha do elemento
    int el;
    //número da coluna do elemento
    int ec;
    //ligação entre os elementos da pilha
    struct pilha* next;
    struct pilha* ant;
}
Elem;

//o ponteiro para a estrutura é definido por Pilha
typedef Elem* Pilha;

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
};


//cabeçalhos das funções
//cria-se uma estrutura info com atributos nulos
struct info* cria_info();
//transforma um arquivo texto em uma matriz e armazena as informações numa estrutura
char** matriz(struct info* info);
//inicializador da busca em profundidade, seus parâmetros são uma matriz e uma estrutura info
void busca_prof(char** mat, struct info* info);
//algoritmo recursivo, seus parâmetros são uma matriz, as coordenadas do elemento atual,
//uma pilha para armazenar as coordenadas e uma estrutura com as informações da matriz
void bp(char** mat, int al, int ac, Pilha* pilha, struct info* info);
//cabeçalhos das funções da pilha duplamente encadeada
//cria uma pilha duplamente encadeada vazia para armazenar as coordenadas
Pilha* cria_pilha();
//insere um elemento no inicio da pilha
//os parâmetros são o início da pilha e as coordenadas do elemento
void insere_pilha(Pilha* pi, int el, int ec);
//retorna a linha do ultimo elemento inserido na pilha
int linha_pilha(Pilha* pi,struct info* info);
//retorna a coluna do ultimo elemento inserido na pilha
int coluna_pilha(Pilha* pi,struct info* info);
//remove o último elemento inserido na pilha
void remove_pilha(Pilha* pi);
//exibe uma pilha do elemento inicial ao final
void exibe_pilha(Pilha* pi);
//libera o espaço alocado para uma pilha
void destruir_pilha(Pilha* pi);

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
    //lê o nome do arquivo pelo teclado e armazena em nomarq
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
    //se o usuário iniciar fora da matriz, dá erro
    if((info->il) > (info->l) || (info->ic) > (info->c)) return 0;
    //fim da leitura, fecha-se o arquivo
    fclose(fp);
    //retorna-se o endereço do primeiro item da matriz
    return mat;
}



//inicializador da busca em profundidade, seus parâmetros são uma matriz e uma estrutura info
void busca_prof(char** mat, struct info* info)
{
    //declara-se um ponteiro de elemento de pilha que armazena o início da nova pilha criada
    Pilha* pilha = cria_pilha();
    //uso do algoritmo recursivo
    bp(mat, (info->il), (info->ic), pilha, info);
    //se não encontrou o objetivo, envia-se mensagem de erro e retorna
    if(!flag)
    {
        printf("O objetivo nao foi encontrado\n");
        return;
    }
    //imprime-se a pilha de coordenadas
    exibe_pilha(pilha);
    //libera-se a memória alocada pela pilha
    destruir_pilha(pilha);
    //fim da execução
}

//algoritmo recursivo, seus parâmetros são uma matriz, as coordenadas do elemento atual,
//uma pilha para armazenar as coordenadas e uma estrutura com as informações da matriz
//o ponto de parada da recursão é quando não há elementos vizinhos visitáveis
//ou quando se encontra o objetivo
void bp(char** mat, int al, int ac, Pilha* pilha, struct info* info)
{
    //verifica-se se chegou ao destino
    if((mat[al][ac])=='$')
    {
        insere_pilha(pilha, al,ac);
        flag=1;
        return;
    }
    //troca-se o caracter
    (mat[al][ac]) = '-';
    //insere-se o atual na pilha de coordenadas
    insere_pilha(pilha, al, ac);
    //verificação de flag: se o objetivo foi encontrado, retornar
    if(flag)return;
    //verificação para norte
    //verificação de flag: se o objetivo já foi encontrado, não será aplicada a recursão
    //verificação de borda: se a linha atual for a primeira, não se pode ir para norte
    //verificação de caracter: se o elemento a norte não for visitável, não será aplicada a recursão
    if(!flag)if(al-1 >= 0) if((mat[al-1][ac]) != '-') 
    {
        bp(mat, al-1, ac, pilha, info);
        //como a tentativa foi mal sucedida, remove-se o último elemento da pilha e o torna acessível
        if(!flag) remove_pilha(pilha);
    }
    //verificação para leste
    //verificação de flag: se o objetivo já foi encontrado, não será aplicada a recursão
    //verificação de borda: se a coluna atual for a última, não se pode ir para leste
    //verificação de caracter: se o elemento a leste não for visitável, não será aplicada a recursão
    if(!flag)if((ac+1)<(info->c)) if((mat[al][ac+1])!='-') 
    {
        bp(mat, al, ac + 1, pilha, info);
        //como a tentativa foi mal sucedida, remove-se o último elemento da pilha e o torna acessível
        if(!flag)remove_pilha(pilha);
    }
    //verificação para sul
    //verificação de flag: se o objetivo já foi encontrado, não será aplicada a recursão
    //verificação de borda: se a linha atual for a última não se pode ir para sul
    //verificação de caracter: se o elemento a sul não for visitável, não será aplicada a recursão
    if(!flag)if(al+1 < info->l) if((mat[al+1][ac]) != '-') 
    {
        bp(mat, al+1, ac, pilha, info);
        //como a tentativa foi mal sucedida, remove-se o último elemento da pilha e o torna acessível
        if(!flag)remove_pilha(pilha);
    }
    //verificação para oeste
    //verificação de flag: se o objetivo já foi encontrado, não será aplicada a recursão
    //verificação de borda: se a coluna atual for a primeira, não se pode ir para oeste
    //verificação de caracter: se o elemento a oeste não for visitável, não será aplicada a recursão
    if(!flag)if(ac-1 >= 0) if ((mat[al][ac-1]) != '-') 
    {
        bp(mat, al, ac - 1, pilha, info);
        //como a tentativa foi mal sucedida, remove-se o último elemento da pilha e o torna acessível
        if(!flag)remove_pilha(pilha);
    }
    //backtracking
}

//implementações das funções da pilha duplamente encadeada
//cria uma pilha duplamente encadeada vazia para armazenar as coordenadas
Pilha* cria_pilha()
{
    //declara-se e aloca memória para um ponteiro de ponteiro de elemento de pilha
    Pilha* pi = (Pilha*)malloc(sizeof(Pilha));
    //se não foi possível alocar memória, retorna-se 0 para sinalizar erro
    if (!pi) return 0;
    //se não, o início da pilha aponta para o vazio
    *pi = NULL;
    //retorna-se o ponteiro de ponteiro de elemento de pilha que indica o início dela
    return pi;
}

//insere os elementos no início da pilha
//os parâmetros são o início da pilha e as coordenadas do elemento
void insere_pilha(Pilha* pi, int el, int ec)
{
    //se a pilha não existe, retorna-se
    if(!pi) return;
    //se não, declara-se e aloca-se memória para um ponteiro elemento de pilha
    Pilha novo = (Pilha)malloc(sizeof(Elem));
    //se não foi possível alocar memória, retorna-se
    if(!novo) return;
    //se não, ao novo elemento se atribui as coordenadas passadas como parâmetros
    novo->el = el;
    novo->ec = ec;
    //como a inserção é sempre no início, o anterior aponta para o vazio e o próximo para o próximo do início
    novo->ant=NULL;
    if(!(*pi)) novo->next=NULL;
    else 
    {
        novo->next=(*pi);
        (*pi)->ant=novo;
    }
    *pi=novo;
    //fim da inserção
}

//retorna a linha do ultimo elemento inserido na pilha
int linha_pilha(Pilha* pi, struct info* info)
{
    if(!*(pi))return info->il;
    return (*pi)->el;
}

//retorna a coluna do ultimo elemento inserido na pilha
int coluna_pilha(Pilha* pi, struct info* info)
{
    if(!(*pi))return info->ic;
    return (*pi)->ec;
}

//remove o último elemento inserido na pilha
void remove_pilha(Pilha* pi)
{
    if(!pi||!(*pi))return;
    Pilha aux=*pi;
    *pi=(*pi)->next;
    free(aux);
}

//exibe uma pilha, o parâmetro é o início de uma pilha
void exibe_pilha(Pilha* pi)
{
    //se a pilha não existe ou está vazia, retorna-se
    if(!pi || !(*pi)) return;
    printf("[");
    //se não, declara-se um ponteiro auxiliar para elemento de pilha chamado prt
    //que será usado para acessar a pilha e imprimi-la
    //o ponteiro prox auxilia prt
    Pilha prox = *pi, prt=NULL;
    //como queremos imprimir do elemento inicial ao final, primeiro chegamos ao fim da pilha
    while(prox)
    {
        prt=prox;
        prox=prox->next;
    }
    //enquanto não se chegar ao início da lista
    while(prt)
    {
        //imprime-se as coordenadas do elemento atual
        printf("(%d, %d)", prt->el, prt->ec);
        if(prt->ant)printf(",");
        //prt aponta para o elemento anterior da pilha, que é o próximo elemento do trajeto
        prt = prt->ant;
    }
    //fim da exibição
    printf("]");
}

//destrói uma pilha, o parâmetro é o início de uma pilha
void destruir_pilha(Pilha* pi)
{
    //se a pilha não existe, retorna-se
    if(!pi) return;
    //se não, declara-se um ponteiro auxiliar chamado dtc
    //que será usado para destruir a pilha
    Pilha dtc;
    //enquanto a pilha não for vazia
    while(*pi)
    {
        //dtc aponta para o início da pilha
        dtc = *pi;
        //o início da pilha aponta para seu próximo elemento
        *pi = (*pi)->next;
        //libera-se a memória alocada pelo elemento apontado por dtc
        free(dtc);
    }
    //libera-se a memória alocada pelo elemento restante
    free(*pi);
    //libera-se a memória alocada pelo início da pilha
    free(pi);
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
    busca_prof(mat, info);
    //libera a memória alocada por info e mat
    free(info);
    free(mat);
    //fim do programa
    return 0;
}
