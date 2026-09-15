class Empty(Exception):
    pass


class FilaListaEncadeada:
    
    # implementa uma fila usando uma LE simples

    # classe _Node - interna
    class _Node():

        __slots__ = '_info', '_prox'
        
        def __init__ (self, info, prox):
            # inicia os campos
            self._info = info
            self._prox = prox


    # métodos de fila
    def __init__ (self):
        # cria uma fila vazia
        self._inicio  = None # vazia
        self._final = None  # vazia
        self._tamanho = 0   # tamanho da pilha
        
    def __len__(self):
        # retorna o tamanho da fila
        return self._tamanho

    def is_empty(self):
        # retorna True se fila vazia
        return self._tamanho == 0
    

    def first(self):
        # retorna sem remover o inicio da fila.
        # sinaliza exceção se fila vazia
        if self.is_empty():
            Empty('fila vazia')
        return self._inicio._info

    def enqueue(self, info):
        novo = self._Node(info, None)
        if self.is_empty():
            self._inicio = novo
        else:
            self._final._prox = novo
        self._final = novo
        self._tamanho += 1
        
                    
    def dequeue(self):

        if self.is_empty():
            raise Empty('fila vazia')

        inicio = self._inicio          
        self._inicio = inicio._prox   

        if self._inicio is None:
            self._final = None

        self._tamanho -= 1
        return inicio._info

    def ImprimeFila(self):
        no = self._inicio
        while no is not None:
            print(no._info)
            no = no._prox


# # Cria uma fila em lista encadeada
# F = FilaListaEncadeada()
# # adiciona 10 elementos
# for k in range(10): F.enqueue(k)
# F.ImprimeFila()
# # remove 6 elementos
# print('________')
# for k in range(6): F.dequeue()
# F.ImprimeFila()
# # algumas informações da fila
# print("\nprimeiro elemento da fila = ", F.first())
# print("tamanho da fila = ", len(F))
# # remove 6 elementos - vai dar exceção
# try:
#     for k in range(6): F.dequeue()
# except :
#     print("Erro capturado")