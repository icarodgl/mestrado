
import time
from pathlib import Path

def soma_divisao_e_conquista(A, inicio, fim):
    if (fim == inicio ):
        return A[inicio]
    meio = (fim+inicio) // 2
    return (
        soma_divisao_e_conquista(A,inicio, meio) +
        soma_divisao_e_conquista(A, meio+1, fim)
            )

def soma_vetor(A):
    return soma_divisao_e_conquista(A, 0, len(A) - 1)


def SomaVetorNormal(A):
    resultado = 0
    for i in A:
        resultado+=i
    return resultado




def ler_input(caminho: Path):
    with open(caminho, 'r') as file:
        return [int(line) for line in file if line.strip()]

# EXEMPLO DE USO
# A = [5, 2, 8, 3, 7] # 25
INPUT_DIR = Path("./")
arquivos = list(INPUT_DIR.rglob("*.txt"))
A = ler_input(arquivos[0])

start = time.time()
resultado = soma_vetor(A)
end = time.time()
# print("Vetor:", A)
print("Soma calculada por divisão e conquista:", resultado,f" em {end - start} segundos")

start = time.time()
resultado2 = SomaVetorNormal(A)
end = time.time()
print("Soma calculada por divisão e conquista:", resultado2,f" em {end - start} segundos")

'''
Análise do comportamento pela relação de recorrência:
'''