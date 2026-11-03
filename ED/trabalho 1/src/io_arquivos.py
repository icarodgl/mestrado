from pathlib import Path
def ler_entrada(caminho: str):
    """
    Retorna (ordem, lista_de_operacoes).
    Cada operação é uma tupla:
      ('I', chave, valor) | ('R', chave) | ('B', chave)
    """
    with open(caminho, "r", encoding="utf-8") as f:
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


def escrever_resultado_busca(caminho: str, achou: bool) -> None:
    with open(caminho, "a", encoding="utf-8") as f:
        if achou:
            f.write("O REGISTRO ESTA NA ARVORE!\n")
        else:
            f.write("O REGISTRO NAO ESTA NA ARVORE!\n")

def escrever_arquivo_saida(saida:str):
    caminho = Path(saida)
    caminho.parent.mkdir(parents=True, exist_ok=True)
    caminho.touch(exist_ok=True)
    open(saida, "w", encoding="utf-8").close()