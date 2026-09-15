def Elimina_Repetidos(lista):
    nova_lista = []
    for i, e in enumerate(lista):
        if e not in lista[i+1:]:
            nova_lista.append(e)
    return nova_lista

class Conjunto:
    ''' Define a classe Conjunto'''
    # Construtor da classe
    def __init__(self, lista = []):
        ''' Cria uma instância de Conjunto '''
        self.lset = Elimina_Repetidos(lista)
        self.lset.sort()
    # união (+)
    def __add__(s1, s2):
        # cria Conjunto com a concatenação de duas listas
        return Conjunto(s1.lset + s2.lset)
    # Tamanho do Conjunto
    def __len__(self):
        return len(self.lset)
        # Transforma elemento da classe em str para o print
        # Mostrar um '*' na frente para diferenciar do print(lista)
    def __str__(self):
        return '*' + str(self.lset)

    def __mul__(self, other):
        resultado = [e for e in self.lset if e in other.lset]
        return Conjunto(resultado)
    
    def __sub__(self, other):
        resultado = [e for e in self.lset if e not in other.lset]
        return Conjunto(resultado)
    
    def __lt__(self, other):
        contem = len([e for e in self.lset if e in other.lset]) > 0 
        diferente = self.lset != other.lset
        return contem and diferente
    
    def __gt__(self, other):
        contem = len([e in self.lset for e in other.lset]) > 0
        diferente = self.lset != other.lset
        return contem and diferente
    
    def __eq__(self, value):
        if not isinstance(value, Conjunto):
            return False
        return self.lset == value.lset
    
    def __contains__(self, item):
        return item in self.lset
    
    def __iadd__(self, other):
        self.lset = Elimina_Repetidos(self.lset + other.lset)
        self.lset.sort()
        return self
    
    def __isub__(self, other):
        self.lset = [elemento for elemento in self.lset if elemento not in other.lset]
        return self
        
x = Conjunto([1, 2, 25, 5, 72, 5, 1])
y = Conjunto([])   
z = Conjunto([1, 9, 113, 5, 9, 2, 22, 12, 2, 11])



print("x =", x)
print("z =", z)

print("x ^ z =", x * z)
print("x - z =", x - z)
print("z - x =", z - x)

print("x == z?", x == z)


a = Conjunto([1,2,3])
b = Conjunto([1,2,3])
print('a == b', a == b)

print("1 in x?", 1 in x)
print("10 in x?", 10 in x)


print("x < z?", x < z)
print("z > x?", z > x)

b = Conjunto([1, 2, 3, 4])

print("a < b?", a < b)
print("b > a?", b > a)
print("a == a?", a == a)