class Empty(Exception):
    pass

class PilhaListaEncadeada:
    
    # implementa uma pilha usando uma LE simples

    # classe _Node - interna
    class _Node:

        __slots__ = '_info', '_prox'
        
        def __init__ (self, info, prox):
            # inicia os campos
            self._info = info
            self._prox = prox
    
    # métodos de pilha
    def __init__ (self):
        # cria uma pilha vazia
        self._topo = None # vazia
        self._tamanho = 0 # tamanho da pilha
        
    def __len__(self):
        # retorna o tamanho da pilha
        return self._tamanho

    def is_empty(self):
        # retorna True se pilha vazia
        return self._tamanho == 0

    def push(self, e):
        # adiciona elemento ao topo da pilha
        topo = self._topo
        novo = self._Node(e,topo)
        self._topo = novo
        self._tamanho +=1
        
        
    def top(self):
        # retorna sem remover o topo da pilha
        # sinaliza exceção se pilha vazia
        if self.is_empty():
           raise Empty('Pilha vazia')
        return self._topo._info
                    
    def pop(self):
        # remove e retorna o topo da pilha
        # sinaliza exceção se pilha vazia
        if self.is_empty():
            raise Empty('Pilha vazia')

        topo = self._topo
        self._topo = topo._prox
        self._tamanho -= 1
        return topo._info
    
    def ImprimePilha(self):
        no = self._topo
        while no is not None:
            print(no._info)
            no = no._prox



# # Cria uma pilha
# P = PilhaListaEncadeada()
# # adiciona 10 elementos
# for k in range(10): P.push(k)
# P.ImprimePilha()
# # remove 5 elementos
# for k in range(5): P.pop()
# P.ImprimePilha()
# # algumas informações da pilha
# print("\ntopo da pilha = ", P.top())
# print("tamanho da pilha = ", len(P))
# # remove 6 elementos - vai dar excessão
# for k in range(6): P.pop()