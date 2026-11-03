import sys
from arvoreb import ArvoreB
from io_arquivos import ler_entrada, escrever_resultado_busca, escrever_arquivo_saida

ARQUIVO_BIN = "arvore.bin"

def main():
    if len(sys.argv) != 3:
        return
    entrada, saida = sys.argv[1], sys.argv[2]

    escrever_arquivo_saida(saida)

    ordem, operacoes = ler_entrada(entrada)
    arvore = ArvoreB(ordem, ARQUIVO_BIN)

    for op in operacoes:
        if op[0] == "I":
            _, chave, valor = op
            arvore.inserir(chave, valor)
        elif op[0] == "R":
            _, chave = op
            arvore.remover(chave)
        elif op[0] == "B":
            _, chave = op
            escrever_resultado_busca(saida, arvore.buscar(chave))

    arvore.imprimir_largura(saida)


if __name__ == "__main__":
    main()