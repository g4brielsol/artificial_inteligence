def get_posicao_inicial(labirinto: list) -> list:
    for linha in range(len(labirinto)):
        for posicao in range(len(labirinto[linha])):
            if labirinto[linha][posicao] == "#":
                return([linha, posicao]) 

def get_vizinhos(posicao: list, limites_do_labirinto: list, labirinto:list, vizinhos: list) -> list:
    comprimento = limites_do_labirinto[0] - 1
    largura = limites_do_labirinto[1] - 1
    labirinto[posicao[0]] = labirinto[posicao[0]][:posicao[1]] +  "-" + labirinto[posicao[0]][posicao[1]+1:]

    #sul
    if (posicao[0] + 1 > comprimento) or (labirinto[posicao[0] + 1][posicao[1]] == "-"):
        pass
    else:
        if labirinto[posicao[0] + 1][posicao[1]] == "$":
            return["$", posicao[0] + 1, posicao[1]]
        else:
            adicao = posicao[0] + 1
            vizinhos.insert(0, [adicao, posicao[1]])
    #leste
    if (posicao[1] + 1 > largura) or (labirinto[posicao[0]][posicao[1] + 1] == "-"):
        pass
    else:
        if labirinto[posicao[0]][posicao[1] + 1] == "$":
            return["$", posicao[0], posicao[1] + 1]
        else:
            adicao = posicao[1] + 1
            vizinhos.insert(0, [posicao[0], adicao])
    
    #norte
    if (posicao[0] - 1 < 0) or (labirinto[posicao[0] - 1][posicao[1]] == "-"):
        pass
    else:
        if labirinto[posicao[0] - 1][posicao[1]] == "$":
            return["$", posicao[0] - 1, posicao[1]]
        else:
            subtracao = posicao[0] - 1
            vizinhos.insert(0, [subtracao, posicao[1]])
    #oeste
    if (posicao[1] - 1 < 0) or (labirinto[posicao[0]][posicao[1] - 1] == "-"):
        pass
    else:
        if labirinto[posicao[0]][posicao[1] - 1] == "$":
            return["$", posicao[0], posicao[1] - 1]
        else:
            subtracao = posicao[1] - 1
            vizinhos.insert(0, [posicao[0], subtracao])
    
    return(vizinhos[1:])


        
def main():
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
    vizinhos = [posicao_inicial]
    pegar_vizinhos = get_vizinhos(vizinhos[0], limites_do_labirinto, 
                                  labirinto, vizinhos)
    print(pegar_vizinhos)
    caminho_percorrido = []
    contador = 0
    while(pegar_vizinhos[contador][0] != "$" or pegar_vizinhos[contador] != "?"):
        caminho_percorrido.append(pegar_vizinhos[0])
    #print(pegar_vizinhos)
        pegar_vizinhos = get_vizinhos(pegar_vizinhos[contador], limites_do_labirinto, 
                                labirinto, vizinhos)
        print(pegar_vizinhos)
        contador += 1
        #pegar_vizinhos = get_vizinhos(pegar_vizinhos[0], limites_do_labirinto, 
        #                        labirinto, vizinhos)
        #print(pegar_vizinhos)



# inicia o programa
if __name__ == "__main__":
    main()

    