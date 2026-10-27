def max_divisao_e_conquista(A, inicio, fim):
    if (fim == inicio ):
        return A[inicio]
    meio = (fim+inicio) // 2
    a = max_divisao_e_conquista(A,inicio, meio) 
    b = max_divisao_e_conquista(A, meio+1, fim)
    if a>b: return a
    return b            

# EXEMPLO DE USO
A = [7, 3, 12, 1, 9, 15, 4]

resultado = max_divisao_e_conquista(A, 0, len(A) - 1)

print("Vetor:", A)
print("Máximo encontrado por divisão e conquista:", resultado)
    
'''
Análise do comportamento pela relação de recorrência:
'''