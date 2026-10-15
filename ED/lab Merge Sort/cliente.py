import sys
import time
import json
from sorts import *
from pathlib import Path

sys.setrecursionlimit(2000)

INPUT_DIR = Path("input/in")

# Ordem dos tamanhos (prefixos dos nomes dos arquivos)
ORDEM_TAMANHOS = ["1K", "10K", "100K"]


def ler_input(caminho: Path):
    with open(caminho, 'r') as file:
        return [int(line) for line in file if line.strip()]


def sort(a):
    aux = [None] * len(a)
    hi = len(a) - 1
    # merge_sort_classico(a, aux, 0, hi)
    # merge_sort_cutoff(a, aux, 0, hi)
    # merge_sort_skip_merge(a, aux, 0, hi)
    # merge_sort_cutof_skip_merge(a, aux, 0, hi)
    merge_sort_bottomup(a, 0, hi)


def tamanho_de(nome: str) -> int:
    """Retorna o índice do tamanho no ORDEM_TAMANHOS (para ordenação)."""
    for i, tam in enumerate(ORDEM_TAMANHOS):
        if nome.startswith(tam):
            return i
    return len(ORDEM_TAMANHOS)  # desconhecidos vão pro fim


def processar(caminho: Path):
    """Executa um arquivo e devolve dict {label: tempo}."""
    label = caminho.stem  # ex: "1Kunif_rand"
    start = time.time()
    dados = ler_input(caminho)
    sort(dados)
    end = time.time()
    return label, end - start


def main():
    # Coleta todos os .txt
    arquivos = list(INPUT_DIR.rglob("*.txt"))

    # Ordena por tamanho (1K < 10K < 100K) e depois alfabeticamente
    arquivos.sort(key=lambda p: (tamanho_de(p.stem), p.stem))

    print("[")
    for caminho in arquivos:
        label, tempo = processar(caminho)
        # Cada linha é um objeto JSON independente (JSON Lines)

        print(json.dumps({label: round(tempo, 6)}),end=",\n")
    print("]")


if __name__ == "__main__":
    main()