
class Conjunto:
    ''' Define a classe Conjunto'''
    # Construtor da classe
    def __init__(self, lista = []):
        ''' Cria uma instância de Conjunto '''
        self.lset = self.elimina_repetidos(lista)
        self.lset.sort()
    def elimina_repetidos(self,lista):
        nova_lista = []
        for k in range(0, len(lista)):
            item = lista[k]
            parte = lista[k+2:]
            if item not in parte :
                nova_lista.append(item)
        return nova_lista
    # união (+)
    def __add__(s1, s2):
        # cria Conjunto com a concatenação de duas listas
        return Conjunto(s1.lset + s2.lset)
    def __mul__(s , o):
        c1 = s.lset
        c2 = o.lset
        t1 = len(c1)
        t2 = len(c2)
        i = 0
        nova_lista = []
        while (i <= t1 or i <= t2):
            if i < t1 and c1[i] in c2:
                nova_lista.append(c1[i])
            if i < t2 and c2[i] in c1 and c2[i] not in nova_lista:
                nova_lista.append(c2[i])
            i+=1
        return Conjunto(nova_lista)

    def __sub__(self, other):
        s = self.lset
        o = other.lset
        i = 0
        nova_lista = []
        while (i <= len(o) or i <= len(s)):
            if i < len(s) and s[i] not in o:
                            nova_lista.append(s[i])
            if i < len(o) and o[i] not in s and o[i] not in nova_lista:
                nova_lista.append(o[i])
            i+=1
        return Conjunto(nova_lista)
    def __lt__(self, other):
        s = self.lset
        o = other.lset
        for i in s:
             if i not in o:
                  return False
        return True
    def __gt__(self, other):
        return other < self

    def __eq__(self, value):
        if len(self) != len(value): return False
        s = self.lset
        o = value.lset
        for i, v in enumerate(s):
             if v != o[i]:
                  return False
        return True
    def __iadd__(self, other):
        if type(other).__name__ == 'int':
             return Conjunto(self.lset + [other])
        if type(other).__name__ == 'Conjunto':
              return self + other
        else:
            raise ValueError("Tipo não permitido", type(other))
    # Tamanho do Conjunto
    def __len__(self):
        return len(self.lset)
    # Transforma elemento da classe em str para o print
    # Mostrar um '*' na frente para diferenciar do print(lista)
    def __str__(self):
        return '*' + str(self.lset)


def main():
    a = Conjunto([1,2,3,3])
    b = Conjunto([1,2,3,5,6,7,8,9])
    c = Conjunto([2])

    print('+',a+b)
    print('-',a-b)
    print('*',a*b)
    print('>',a>b)
    print('<',a<b)
    print('==',a==b)
    c += 2
    print('+=',c)
    

if __name__ == '__main__':
    main()