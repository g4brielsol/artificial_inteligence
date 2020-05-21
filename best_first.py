# funcao de escolha aleatoria
import random
# funcao para calcular o tempo do programa
import time


def get_posicao_inicial(labirinto: list) -> list:
    """Funcao que procura e retorna a posicao inicial no mapa (#), 
    representado por uma lista [x, y] 
    """
    # para as posições no comprimento do mapa
    for linha in range(len(labirinto)):
        # para as posições na largura do mapa
        for posicao in range(len(labirinto[linha])):
            # se a posição atual for #
            if labirinto[linha][posicao] == "#":
                # retorna a posição incial
                return([linha, posicao])


def get_vizinhos(comprimento: int, largura: int, posicao: list,
                 limites_do_labirinto: list, labirinto: list, vizinhos: list) -> list:
    """ Funcao que transforma a posicao atual * em -, pega as posicoes adjacentes
    possíveis de se locomover (sul, leste, norte e oeste), se essa posição for o 
    local final ($), termina a busca e retorna a posição. Se não for, insere uma 
    posicao adjacente à atual de forma randômica na lista de proximas buscas, porque
    se não for randômico, a ordem das buscas sempre será sul, leste, norte e oeste de
    acordo com a ordem de implementação. -> retorna uma lista com as próximas posições
    de busca ou a posição final encontrada.
    """
    # transforma a poisção atual * em -
    labirinto[posicao[0]] = (
        labirinto[posicao[0]][:posicao[1]] + "-" + labirinto[posicao[0]][posicao[1]+1:])
    # cria a lista de posições possíveis de se locomover (* ou $)
    posicoes_vizinhas = []
    # verifica se pode ser locomover para a posicao sul (limite de borda e de "-")
    # se sim adiciona a uma lista de posicoes possiveis
    if (posicao[0] + 1 <= comprimento) and (labirinto[posicao[0] + 1][posicao[1]] != "-"):
        if labirinto[posicao[0] + 1][posicao[1]] == "$":
            return["$", posicao[0] + 1, posicao[1]]
        else:
            adicao = posicao[0] + 1
            posicoes_vizinhas.append([adicao, posicao[1]])
    # verifica se pode ser locomover para a posicao leste (limite de borda e de "-")
    # se sim adiciona a uma lista de posicoes possiveis
    if (posicao[1] + 1 <= largura) and (labirinto[posicao[0]][posicao[1] + 1] != "-"):
        if labirinto[posicao[0]][posicao[1] + 1] == "$":
            return["$", posicao[0], posicao[1] + 1]
        else:
            adicao = posicao[1] + 1
            posicoes_vizinhas.append([posicao[0], adicao])
    # verifica se pode ser locomover para a posicao norte (limite de borda e de "-")
    # se sim adiciona a uma lista de posicoes possiveis
    if (posicao[0] - 1 >= 0) and (labirinto[posicao[0] - 1][posicao[1]] != "-"):
        if labirinto[posicao[0] - 1][posicao[1]] == "$":
            return["$", posicao[0] - 1, posicao[1]]
        else:
            subtracao = posicao[0] - 1
            posicoes_vizinhas.append([subtracao, posicao[1]])
    # verifica se pode ser locomover para a posicao oeste (limite de borda e de "-")
    # se sim adiciona a uma lista de posicoes possiveis
    if (posicao[1] - 1 >= 0) and (labirinto[posicao[0]][posicao[1] - 1] != "-"):
        if labirinto[posicao[0]][posicao[1] - 1] == "$":
            return["$", posicao[0], posicao[1] - 1]
        else:
            subtracao = posicao[1] - 1
            posicoes_vizinhas.append([posicao[0], subtracao])
    # escolha randômica em relação a última posicao para ser adicionada
    # a lista de próximos caminhos para encontrar a posicao final
    for i in range(len(posicoes_vizinhas)):
        posicao_aleatoria = random.choice(posicoes_vizinhas)
        # insere o próximo caminho na segunda posicao da lista de caminhos
        # (a primeira posicao da lista é a posicao do mapa atual)
        vizinhos.insert(1, posicao_aleatoria)
        posicoes_vizinhas.remove(posicao_aleatoria)
    # exclui a posicao atual da lista de busca, o novo primeiro item sera a nova
    # posicao de busca
    return vizinhos[1:]


def main():
    # grava o início da execução do programa
    comeco_timer = time.time()
    # abre o arquivo com o mapa em .txt
    # o nome precisa ser labirinto.txt
    entrada = open("labirinto.txt", "r")
    # lê todas as linhas do arquivo
    arquivo_entrada = entrada.readlines()
    # transforma as quebras de linha em nada ""
    arquivo_entrada = [i.replace("\n", "") for i in arquivo_entrada]
    # pega a primeira linha do arquivo para saber o tamanho do labrinto
    # o associa a uma lista [x, y]
    limites_do_labirinto = arquivo_entrada[0].split(" ")
    # tranforma a string x (primeiro elemento da lista) em int
    limites_do_labirinto[0] = int(limites_do_labirinto[0])
    # tranforma a string y (segundo elemento da lista) em int
    limites_do_labirinto[1] = int(limites_do_labirinto[1])
    # exclui a primeira linha do arquivo .txt e mantém só o mapa
    labirinto = arquivo_entrada[1:]
    # printa o mapa lido no arquivo .txt
    for linha in labirinto:
        print(linha)
    # pula uma linha
    print()
    # pega a posicao inicial (#)
    posicao_inicial = get_posicao_inicial(labirinto)
    # printa a posicao inicial
    print("Posicao Inicial {}\n".format(tuple(posicao_inicial)))
    # associa o comprimento e a largura do mapa para a função de verificacao de borda
    comprimento = limites_do_labirinto[0] - 1
    largura = limites_do_labirinto[1] - 1
    # cria uma lista com a posicao inicial
    vizinhos = [posicao_inicial]
    # pega as posições adjacentes possíveis em relação a posição atual [0] na lista de vizinhos
    vizinhos = get_vizinhos(comprimento, largura, vizinhos[0], limites_do_labirinto,
                            labirinto, vizinhos)
    # cria uma lista vazia com as posições percorridas no mapa
    caminho_percorrido = []
    # enquanto não encontra a posicao final e enquanto existirem mais posições de busca na lista
    while (vizinhos[0] != "$") and (len(vizinhos) > 0):
        # adiciona a posicao atual na lista de caminhos percorridos
        caminho_percorrido.append(tuple(vizinhos[0]))
        # procura posições adjacentes em relação a posição atual no mapa
        vizinhos = get_vizinhos(comprimento, largura, vizinhos[0], limites_do_labirinto,
                                labirinto, vizinhos)
    # se o primeiro item atual da lista de busca for a posicao final
    if vizinhos[0] == "$":
        # adiciona a posição final no caminho percorrido
        caminho_percorrido.append(tuple(vizinhos[1:]))
        # printa o caminho percorrido
        print(caminho_percorrido)
        # faz o cálculo do tempo final de execução do programa
        tempo_total = time.time() - comeco_timer
        # printa o tempo total de execução
        print("Tempo Total de Execução: {:.5f} segundos".format(tempo_total))
    # se a solução não for encontrada
    else:
        print("A solução não foi encontrada")


# inicia o programa
if __name__ == "__main__":
    main()
