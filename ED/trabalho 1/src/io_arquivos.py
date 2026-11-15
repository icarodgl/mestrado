
# 2026230168 - Icaro Duarte Gavazza Lima
from pathlib import Path


class IO:

    def __init__(self, entrada, saida):
        self.__saida = saida
        self.__entrada = entrada

    def ler_entrada(self):
        with open(self.__entrada, "r", encoding="utf-8") as f:
            linhas = [l.strip() for l in f if l.strip()]

        ordem = int(linhas[0])
        n = int(linhas[1])
        operacoes = []


        for linha in linhas[2:2 + n]:
            partes = linha.replace(",","").split()
            op = partes[0].upper()
            if op == "I":
                chave_str = partes[1]
                valor_str = partes[2]
                operacoes.append(("I", int(chave_str), int(valor_str)))
            elif op == "R":
                operacoes.append(("R", int(partes[1])))
            elif op == "B":
                operacoes.append(("B", int(partes[1])))
        return ordem, operacoes


    def escrever_resultado_busca(self,achou: bool) -> None:
        with open(self.__saida, "a", encoding="utf-8") as f:
            if achou:
                f.write("O REGISTRO ESTA NA ARVORE!\n")
            else:
                f.write("O REGISTRO NAO ESTA NA ARVORE!\n")

    def escrever_arvore(self, arvore: list[str])-> None:
            with open(self.__saida, "a", encoding="utf-8") as out:
                out.write("-- ARVORE B\n")
                for linha in arvore:
                    out.write(linha + "\n")

    def debuger(self,saida):
        with open(self.__saida, "a", encoding="utf-8") as f:
            f.write(f"debugger: {saida}\n")

    def escrever_arquivo_saida(self):
        caminho = Path(self.__saida)
        caminho.parent.mkdir(parents=True, exist_ok=True)
        caminho.touch(exist_ok=True)
        open(self.__saida, "w", encoding="utf-8").close()