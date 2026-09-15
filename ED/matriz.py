class Matriz:
    
    def __init__(self, m, n):
        self.linhas = m
        self.colunas = n
        self.matriz = [[0] * n for _ in range(m)]
    
    def __getitem__(self, index):
        return self.matriz[index]
    
    def __setitem__(self, index, valor):
        if len(valor) != self.colunas:
            raise ValueError(f"A linha deve ter {self.colunas} elementos")
        self.matriz[index] = valor[:]
        for elemento in valor:
            if not isinstance(elemento, (int, float)):
                raise TypeError("Todos os elementos devem ser números")
    
    def __str__(self):
        resultado = ""
        for linha in self.matriz:
            resultado += str(linha) + "\n"
        return resultado.strip()
    
    def __add__(self, other):
        if self.linhas != other.linhas or self.colunas != other.colunas:
            raise ValueError("Matrizes devem ter o mesmo tamanho para soma")
        
        resultado = Matriz(self.linhas, self.colunas)
        for i in range(self.linhas):
            for j in range(self.colunas):
                resultado.matriz[i][j] = self.matriz[i][j] + other.matriz[i][j]
        return resultado
    
    def __sub__(self, other):
        if self.linhas != other.linhas or self.colunas != other.colunas:
            raise ValueError("Matrizes devem ter o mesmo tamanho para subtração")
        
        resultado = Matriz(self.linhas, self.colunas)
        for i in range(self.linhas):
            for j in range(self.colunas):
                resultado.matriz[i][j] = self.matriz[i][j] - other.matriz[i][j]
        return resultado
    
    def __mul__(self, other):
        if self.colunas != other.linhas:
            raise ValueError(f"Número de colunas da primeira matriz ({self.colunas}) deve ser igual ao número de linhas da segunda ({other.linhas})")
        
        resultado = Matriz(self.linhas, other.colunas)
        for i in range(self.linhas):
            for j in range(other.colunas):
                soma = 0
                for k in range(self.colunas):
                    soma += self.matriz[i][k] * other.matriz[k][j]
                resultado.matriz[i][j] = soma
        return resultado
    
    def __eq__(self, other):
        if self.linhas != other.linhas or self.colunas != other.colunas:
            return False
        for i in range(self.linhas):
            for j in range(self.colunas):
                if self.matriz[i][j] != other.matriz[i][j]:
                    return False
        return True
    
    def __len__(self):
        return self.linhas
    

ma = Matriz(2, 3)
mb = Matriz(2, 3)

ma[0] = [1, 2, 3]
ma[1] = [4, 5, 6]
mb[0] = [5, 5, 5]
mb[1] = [9, 9, 9]

print("Matriz A:")
print(ma)
print("Matriz B:")
print(mb)


mc = ma + mb
print(mc)

# Subtração
md = ma - mb
print(md)

me = Matriz(2, 2)
me[0] = [1, 2]
me[1] = [3, 4]
mf = Matriz(2, 2)
mf[0] = [5, 6]
mf[1] = [7, 8]

print(me)
print(mf)
print(me * mf)

print("A == A?", ma == ma)
print("A == B?", ma == mb)