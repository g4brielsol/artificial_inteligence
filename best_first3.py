import random
import time
import statistics


class Count:
    def __init__(self, contar, posicoes_proibidas, custo_posicao, distancia_real, custo_inicial):
        self.contar = 0
        self.posicoes_proibidas = []
        self.distancia_real = []


def get_posicao_simbolo(labirinto: list, simbolo: str) -> list:
    """Funcao que procura um símbolo e retorna sua posicao no mapa,
    representado por uma lista [x, y]
    """
    # para as posições verticais do mapa
    for linha in range(len(labirinto)):
        # para as posições horizontais do mapa
        for posicao in range(len(labirinto[linha])):
            # se a posição atual for igual ao símbolo desejado
            if labirinto[linha][posicao] == simbolo:
                # retorna sua posicao
                return ([linha, posicao])


def distancia_real(iteracao: Count, posicao_final: list, posicao_inicial: list):
    """Funcao que calcula o módulo das distancia horizontais e
    verticais de cada posicao, soma essas distancias e associa esse valor em
    sua respectiva posicao no mapa. Se for uma parede, o custo é -1.
    """
    # para cada linha do tabuleiro
    for linha in range(len(iteracao.distancia_real)):
        # para cada coluna do tabuleiro
        for coluna in range(len(iteracao.distancia_real[0])):
            # se a posicao atual for "-" (parede), custo = -1
            if [linha, coluna] in iteracao.posicoes_proibidas:
                iteracao.distancia_real[linha][coluna] = -1
            else:
                iteracao.distancia_real[linha][coluna] = -2


def get_custo_mapa_real(comprimento: int, largura: int, custo_posicao: list,
                        limites_do_labirinto: list, labirinto: list, iteracao: Count,
                        simbolo_final: str) -> list:
    """ Funcao que pega as posicoes adjacentes possíveis de se locomover (sul, leste, norte e oeste),
    se essa posição for o local final ($), termina a busca e retorna a posição.
    Se não for, retorna a posicao com menor custo f(n) até a posição final.
    Caso exista mais de uma posição adjacente com melhor custo, escolhe randomicamente entre elas
    (mistura dos algoritmos Steepest Ascent Hill Climbing com Stochastic Hill Climbing).
    """
    # iteracao.contar += 1
    posicoes_vizinhas = []
    # custo atual até a posicao final
    custo_atual = custo_posicao[0][0]
    # [x, y] da posicao atual
    posicao = custo_posicao[0][1:]
    # Sul: verifica se não é parede e se está nos limites do mapa
    # verifica se é a posicao final, caso não seja e esteja nos limites acima,
    #  adiciona na lista de possíveis vizinhos
    if (posicao[0] + 1 <= comprimento) and (iteracao.distancia_real[posicao[0] + 1][posicao[1]] == -2):
        adicao = posicao[0] + 1
        posicoes_vizinhas.append(
            [iteracao.distancia_real[posicao[0] + 1][posicao[1]], adicao, posicao[1]])
    # Leste: verifica se não é parede e se está nos limites do mapa
    # verifica se é a posicao final, caso não seja e esteja nos limites acima,
    #  adiciona na lista de possíveis vizinhos
    if (posicao[1] + 1 <= largura) and (iteracao.distancia_real[posicao[0]][posicao[1] + 1] == -2):
        adicao = posicao[1] + 1
        posicoes_vizinhas.append(
            [iteracao.distancia_real[posicao[0]][posicao[1] + 1], posicao[0], adicao])
    # Norte: verifica se não é parede e se está nos limites do mapa
    # verifica se é a posicao final, caso não seja e esteja nos limites acima,
    #  adiciona na lista de possíveis vizinhos
    if (posicao[0] - 1 >= 0) and (iteracao.distancia_real[posicao[0] - 1][posicao[1]] == -2):
        subtracao = posicao[0] - 1
        posicoes_vizinhas.append(
            [iteracao.distancia_real[posicao[0] - 1][posicao[1]], subtracao, posicao[1]])
    # Oeste: verifica se não é parede e se está nos limites do mapa
    # verifica se é a posicao final, caso não seja e esteja nos limites acima,
    #  adiciona na lista de possíveis vizinhos
    if (posicao[1] - 1 >= 0) and (iteracao.distancia_real[posicao[0]][posicao[1] - 1] == -2):
        subtracao = posicao[1] - 1
        posicoes_vizinhas.append(
            [iteracao.distancia_real[posicao[0]][posicao[1] - 1], posicao[0], subtracao])
    # custos_vizinhos = []
    # se tiver um vizinho ou mais pra se locomover
    if len(posicoes_vizinhas) >= 1:
        # para cada posicao vizinha à atual
        for i in posicoes_vizinhas:
            iteracao.distancia_real[i[1]][i[2]] = custo_atual + 1
            i[0] = iteracao.distancia_real[i[1]][i[2]]

    custo_posicao = custo_posicao + posicoes_vizinhas
    return custo_posicao[1:]


def get_vizinhos(comprimento: int, largura: int, custo_posicao: list,
                 limites_do_labirinto: list, labirinto: list, iteracao: Count,
                 simbolo_final: str) -> list:
    """ Funcao que pega as posicoes adjacentes possíveis de se locomover (sul, leste, norte e oeste),
    se essa posição for o local final ($), termina a busca e retorna a posição.
    Se não for, retorna a posicao com menor custo f(n) até a posição final.
    Caso exista mais de uma posição adjacente com melhor custo, escolhe randomicamente entre elas
    (mistura dos algoritmos Steepest Ascent Hill Climbing com Stochastic Hill Climbing).
    """
    iteracao.contar += 1
    posicoes_vizinhas = []
    # custo atual até a posicao final
    custo_atual = custo_posicao[0][0]
    # [x, y] da posicao atual
    posicao = custo_posicao[0][1:]
    # Sul: verifica se não é parede e se está nos limites do mapa
    # verifica se é a posicao final, caso não seja e esteja nos limites acima,
    #  adiciona na lista de possíveis vizinhos
    if (posicao[0] + 1 <= comprimento) and (iteracao.distancia_real[posicao[0] + 1][posicao[1]] > -1):
        if labirinto[posicao[0] + 1][posicao[1]] == simbolo_final:
            return[simbolo_final, posicao[0] + 1, posicao[1]]
        else:
            adicao = posicao[0] + 1
            posicoes_vizinhas.append([adicao, posicao[1]])
    # Leste: verifica se não é parede e se está nos limites do mapa
    # verifica se é a posicao final, caso não seja e esteja nos limites acima,
    #  adiciona na lista de possíveis vizinhos
    if (posicao[1] + 1 <= largura) and (iteracao.distancia_real[posicao[0]][posicao[1] + 1] > -1):
        if labirinto[posicao[0]][posicao[1] + 1] == simbolo_final:
            return[simbolo_final, posicao[0], posicao[1] + 1]
        else:
            adicao = posicao[1] + 1
            posicoes_vizinhas.append([posicao[0], adicao])
    # Norte: verifica se não é parede e se está nos limites do mapa
    # verifica se é a posicao final, caso não seja e esteja nos limites acima,
    #  adiciona na lista de possíveis vizinhos
    if (posicao[0] - 1 >= 0) and (iteracao.distancia_real[posicao[0] - 1][posicao[1]] > -1):
        if labirinto[posicao[0] - 1][posicao[1]] == simbolo_final:
            return[simbolo_final, posicao[0] - 1, posicao[1]]
        else:
            subtracao = posicao[0] - 1
            posicoes_vizinhas.append([subtracao, posicao[1]])
    # Oeste: verifica se não é parede e se está nos limites do mapa
    # verifica se é a posicao final, caso não seja e esteja nos limites acima,
    #  adiciona na lista de possíveis vizinhos
    if (posicao[1] - 1 >= 0) and (iteracao.distancia_real[posicao[0]][posicao[1] - 1] > -1):
        if labirinto[posicao[0]][posicao[1] - 1] == simbolo_final:
            return[simbolo_final, posicao[0], posicao[1] - 1]
        else:
            subtracao = posicao[1] - 1
            posicoes_vizinhas.append([posicao[0], subtracao])
    custos_vizinhos = []
    # se tiver um vizinho ou mais pra se locomover
    if len(posicoes_vizinhas) >= 1:
        # para cada posicao vizinha à atual
        for i in posicoes_vizinhas:
            # adiciona os custos melhores que o atual e suas respectivas posições na lista custos_vizinhos
            if iteracao.distancia_real[i[0]][i[1]] < custo_atual:
                custos_vizinhos.append(
                    [iteracao.distancia_real[i[0]][i[1]], i[0], i[1]])
        # ordena em ordem crescente a lista pelo primeiro elemento de cada sub lista (seus custos)
        custos_vizinhos = sorted(
            custos_vizinhos, key=lambda x: x[0])
    else:
        # printa custo atual
        print("f(n) = {}".format(custo_atual))
        print("Nao possui vizinhos com melhor f(n) = h(n)")
        # vizinho igual "" para sair do while no main()
        vizinho = ""
        return (vizinho)

    #print("posicao {}".format(custos_vizinhos))
    return custos_vizinhos


def main():
    contar_acerto = []
    caminhos_acerto = []
    acerto_tempo = []
    contar_erro = []
    caminhos_erro = []
    erro_tempo = []
    acertos = 0
    erros = 0
    for kas in range(1000):
        # grava o início da execução do programa
        comeco_timer = time.time()
        # abre o arquivo com o mapa em .txt
        # o nome precisa ser labirinto.txt
        entrada = open("labirinto10.txt", "r")
        # lê todas as linhas do arquivo
        arquivo_entrada = entrada.readlines()
        # transforma as quebras de linha \n em nada ""
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
    #    for linha in labirinto:
    #                print(linha)
    #            print()
        simbolo_inicial = "$"
        simbolo_final = "#"
        # pega a posicao inicial (#)
        posicao_inicial = get_posicao_simbolo(labirinto, simbolo_inicial)
        # pega a posicao final ($)
        posicao_final = get_posicao_simbolo(labirinto, simbolo_final)
        # associa o comprimento e a largura do mapa para a função de verificacao de borda
        comprimento = limites_do_labirinto[0] - 1
        largura = limites_do_labirinto[1] - 1
        # classe Count para armazenar informacoes importantes sobre o mapa
        iteracao = Count
        # cria a lista vazia que vai armazenar as paredes do mapa
        iteracao.posicoes_proibidas = []
        # cria a lista vazia que vai armazenar as distancias até o objetivo
        iteracao.distancia_real = []
        # para cada linha do mapa
        for linha in range(len(labirinto)):
            # adiciona listas de posicoes para calcular o custo de cada
            # posicao no mapa
            iteracao.distancia_real.append([0] * (largura + 1))
            # marca as posicoes "-" (parede) na lista iteracao.posicoes_proibidas
            for posicao in range(len(labirinto[linha])):
                if labirinto[linha][posicao] == "-":
                    iteracao.posicoes_proibidas.append([linha, posicao])
        # calcula as distancias de cada posicao
        distancia_real(iteracao, posicao_final, posicao_inicial)
        iteracao.distancia_real[posicao_inicial[0]][posicao_inicial[1]] = 0
        # associa o custo e posicao do ponto inicial
        vizinho = [[iteracao.distancia_real[posicao_inicial[0]][posicao_inicial[1]],
                    posicao_inicial[0], posicao_inicial[1]]]
        while (len(vizinho) > 0) and (vizinho[0] != simbolo_final):
            # procura posicoes possiveis adjacentes a posicao atual
            vizinho = get_custo_mapa_real(comprimento, largura, vizinho, limites_do_labirinto,
                                          labirinto, iteracao, simbolo_final)

        simbolo_inicial = "#"
        simbolo_final = "$"
        iteracao.contar = 0
        caminho_percorrido = []
        # pega a posicao inicial (#)
        posicao_inicial = get_posicao_simbolo(labirinto, simbolo_inicial)
        # pega a posicao final ($)
        posicao_final = get_posicao_simbolo(labirinto, simbolo_final)
        # printa a posicao inicial
        print("Posicao Inicial {}\n".format(tuple(posicao_inicial)))
        # custo e posicao da posicao inicial
        vizinho = [[iteracao.distancia_real[posicao_inicial[0]][posicao_inicial[1]],
                    posicao_inicial[0], posicao_inicial[1]]]
        # enquanto haver posicao para busca e ela nao for a posicao final
        # procura a posicao final
        while (len(vizinho) > 0) and (vizinho[0] != simbolo_final):
            # adiciona a posicao atual no caminho percorrido
            caminho_percorrido.append(tuple(vizinho[0][1:]))
            # procura posicoes possiveis adjacentes a posicao atual
            vizinho = get_vizinhos(comprimento, largura, vizinho, limites_do_labirinto,
                                   labirinto, iteracao, simbolo_final)
        # se tiver uma posicao adjacente
        if (len(vizinho) > 0):
            # se for a posicao final
            if vizinho[0] == simbolo_final:
                # adiciona a posicao final ao caminho
                caminho_percorrido.append(tuple(vizinho[1:]))
        # calcula o tempo final de execução
        tempo_total = time.time() - comeco_timer
        # se tiver solucao
        if vizinho != "":
            acertos += 1
            contar_acerto.append(iteracao.contar)
            caminhos_acerto.append(len(caminho_percorrido))
            acerto_tempo.append(tempo_total)
            print("Solução Encontrada")
        else:
            erros += 1
            contar_erro.append(iteracao.contar)
            caminhos_erro.append(len(caminho_percorrido))
            erro_tempo.append(tempo_total)
            print("Solução não foi encontrada")
            print("Tempo Total de Execução: {:.5f} segundos".format(
                tempo_total))
        print("Iterações de Busca: {}".format(iteracao.contar))
        print("Tamanho do Caminho Percorrido {}".format(len(caminho_percorrido)))
        print("Passos andados {}".format(caminho_percorrido))
        print("Tempo Total de Execução: {:.5f} segundos".format(tempo_total))

    print("Acertos {}".format(acertos))
    print("Erros {}".format(erros))
    if acertos > 0:
        print("caminhos media acerto {:.6f}".format(
            statistics.mean(caminhos_acerto)))
        print("contar media acerto {:.6f}".format(
            statistics.mean(contar_acerto)))
        print("caminhos desvio padrao acerto {:.6f}".format(
            statistics.stdev(caminhos_acerto)))
        print("contar desvio padrao acerto {:.6f}".format(
            statistics.stdev(contar_acerto)))
        print("porcentagem acertos {}%".format(
            (acertos * 100) / (acertos + erros)))
        print("tempo medio acerto {:.6f}".format(
            statistics.mean(acerto_tempo)))
        print("tempo desvio padro acerto {:.6f}".format(
            statistics.stdev(acerto_tempo)))
    print()
    if erros > 0:
        print("caminhos media erro {:.6f}".format(
            statistics.mean(caminhos_erro)))
        print("contar media erro {:.6f}".format(statistics.mean(contar_erro)))
        print("caminhos desvio padrao erro {:.6f}".format(
            statistics.stdev(caminhos_erro)))
        print("contar desvio padrao erro {:.6f}".format(
            statistics.stdev(contar_erro)))
        print("porcentagem erro {:.6f}%".format(
            (erros * 100) / (acertos + erros)))
        print("tempo medio erro {:.6f}".format(statistics.mean(erro_tempo)))
        print("tempo desvio padrao erro {:.6f}".format(
            statistics.stdev(erro_tempo)))
    print()

    # for j in iteracao.distancia_real:
    #    print(j)


# inicia o programa
if __name__ == "__main__":
    main()
