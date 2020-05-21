import random
import time


class Count:
    def __init__(self, contar, posicoes_proibidas):
        self.contar = 0
        self.posicoes_proibidas = []


def get_posicao_inicial(labirinto: list) -> list:
    for linha in range(len(labirinto)):
        for posicao in range(len(labirinto[linha])):
            if labirinto[linha][posicao] == "#":
                return([linha, posicao])


def get_vizinhos(comprimento: int, largura: int, posicao: list,
                 limites_do_labirinto: list, labirinto: list,
                 vizinhos: list, iteracao: Count) -> list:
    iteracao.contar += 1
    # labirinto[posicao[0]] = (
    #    labirinto[posicao[0]][:posicao[1]] + "-" + labirinto[posicao[0]][posicao[1]+1:])
    posicoes_vizinhas = []
    # sul
    if (posicao[0] + 1 <= comprimento) and ([posicao[0] + 1, posicao[1]] not in iteracao.posicoes_proibidas):
        if labirinto[posicao[0] + 1][posicao[1]] == "$":
            return["$", posicao[0] + 1, posicao[1]]
        else:
            adicao = posicao[0] + 1
            posicoes_vizinhas.append([adicao, posicao[1]])
    # leste
    if (posicao[1] + 1 <= largura) and ([posicao[0], posicao[1] + 1] not in iteracao.posicoes_proibidas):
        if labirinto[posicao[0]][posicao[1] + 1] == "$":
            return["$", posicao[0], posicao[1] + 1]
        else:
            adicao = posicao[1] + 1
            posicoes_vizinhas.append([posicao[0], adicao])
    # norte
    if (posicao[0] - 1 >= 0) and ([posicao[0] - 1, posicao[1]] not in iteracao.posicoes_proibidas):
        if labirinto[posicao[0] - 1][posicao[1]] == "$":
            return["$", posicao[0] - 1, posicao[1]]
        else:
            subtracao = posicao[0] - 1
            posicoes_vizinhas.append([subtracao, posicao[1]])
    # oeste
    if (posicao[1] - 1 >= 0) and ([posicao[0], posicao[1] - 1] not in iteracao.posicoes_proibidas):
        if labirinto[posicao[0]][posicao[1] - 1] == "$":
            return["$", posicao[0], posicao[1] - 1]
        else:
            subtracao = posicao[1] - 1
            posicoes_vizinhas.append([posicao[0], subtracao])

    for i in range(len(posicoes_vizinhas)):
        posicao_aleatoria = random.choice(posicoes_vizinhas)
        iteracao.posicoes_proibidas.append(posicao_aleatoria)
        vizinhos.insert(1, posicao_aleatoria)
        posicoes_vizinhas.remove(posicao_aleatoria)
    return vizinhos[1:]


def main():
    comeco_timer = time.time()
    entrada = open("labirinto.txt", "r")
    arquivo_entrada = entrada.readlines()
    arquivo_entrada = [i.replace("\n", "") for i in arquivo_entrada]
    limites_do_labirinto = arquivo_entrada[0].split(" ")
    limites_do_labirinto[0] = int(limites_do_labirinto[0])
    limites_do_labirinto[1] = int(limites_do_labirinto[1])
    labirinto = arquivo_entrada[1:]
    for linha in labirinto:
        print(linha)
    print()

    posicao_inicial = get_posicao_inicial(labirinto)
    print("Posicao Inicial {}\n".format(tuple(posicao_inicial)))
    iteracao = Count
    iteracao.posicoes_proibidas = [posicao_inicial]
    for linha in range(len(labirinto)):
        for posicao in range(len(labirinto[linha])):
            if labirinto[linha][posicao] == "-":
                iteracao.posicoes_proibidas.append([linha, posicao])
    comprimento = limites_do_labirinto[0] - 1
    largura = limites_do_labirinto[1] - 1
    menor_valor = 10 ** 10
    tamanho_posicoes_proibidas = len(iteracao.posicoes_proibidas)
    menor_caminho = 0
    for i in range(30):
        iteracao.contar = 0
        # pega só as posições "-" e exclui as posições adicionadas na iteração anterior
        iteracao.posicoes_proibidas = iteracao.posicoes_proibidas[:tamanho_posicoes_proibidas]
        vizinhos = [posicao_inicial]
        caminho_percorrido = []
        while (len(vizinhos) > 0) and (vizinhos[0] != "$"):
            caminho_percorrido.append(tuple(vizinhos[0]))
            vizinhos = get_vizinhos(comprimento, largura, vizinhos[0], limites_do_labirinto,
                                    labirinto, vizinhos, iteracao)
        print(len(caminho_percorrido))
        if (len(vizinhos) > 0):
            if vizinhos[0] == "$":
                caminho_percorrido.append(tuple(vizinhos[1:]))
                if iteracao.contar < menor_valor:
                    menor_valor = iteracao.contar
                    menor_caminho = caminho_percorrido

    tempo_total = time.time() - comeco_timer
    if menor_caminho != 0:
        print(menor_caminho)
        print("Tamanho menor caminho {}".format(len(menor_caminho)))
        print("Tempo Total de Execução: {:.5f} segundos".format(tempo_total))
        print("Conclusao: {:.0f} passos".format(menor_valor))
    else:
        print("Solução não foi encontrada")
        print("Tempo Total de Execução: {:.5f} segundos".format(tempo_total))


# inicia o programa
if __name__ == "__main__":
    main()
