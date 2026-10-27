def kendall_tau(A, B):
    n = len(A)
    if n <= 1:
        return 0
    
    pos = {v: i for i, v in enumerate(B)}
    discord = 0
    for j in range(1, n):
        if pos[A[0]] > pos[A[j]]:
            discord += 1
    return discord + kendall_tau(A[1:], B)


# EXEMPLO DE USO
A = [3, 1, 2, 4]
B = [1, 2, 3, 4]
print("Kendall τ:", kendall_tau(A, B))

'''
Análise do comportamento pela relação de recorrência:
'''
