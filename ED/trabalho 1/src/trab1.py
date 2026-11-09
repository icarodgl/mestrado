import sys
from arvoreb import ArvoreB
from no import No
from repositorio import Repositorio
from io_arquivos import IO

ARQUIVO_BIN = "arvore.bin"

def main():
    
    if len(sys.argv) != 3:
        return
    entrada, saida = sys.argv[1], sys.argv[2]

    io = IO(entrada,saida)

    io.escrever_arquivo_saida()

    ordem, operacoes = io.ler_entrada()

    No.configurar(ordem)
    repo = Repositorio(ARQUIVO_BIN)
    arvore = ArvoreB(ordem, repo)

    for op in operacoes:
        if op[0] == "I":
            _, chave, valor = op
            arvore.inserir(chave, valor)
        elif op[0] == "R":
            _, chave = op
            arvore.remover(chave)
        elif op[0] == "B":
            _, chave = op
            io.escrever_resultado_busca(arvore.buscar(chave))

    io.escrever_arvore(arvore.imprimir_largura())

if __name__ == "__main__":
    main()