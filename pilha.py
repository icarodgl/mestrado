class Empty(Exception):
    pass
class UnknownOperator(Exception):
    pass

class PilhaLista():

    # Pilha como uma lista

    # Construtor da classe PilhaLista
    def __init__(self):
        self._pilha = [] # lista que contera a pilha

    # retorna o tamanho da pilha
    def __len__ (self):

        return len(self._pilha)

    def __str__(self):
        return ' '.join(self._pilha)

    def __iter__(self):
        return iter(self._pilha)

    def to_str(self):
        return self.__str__()
    # retorna True se pilha vazia
    def is_empty(self) -> bool:
        return len(self._pilha) == 0 

    # empilha novo elemento e 
    def push(self, e):
        self._pilha.append(e)


    # retorna o elemento do topo da pilha sem retira-lo
    # excecao se pilha vazia
    def top(self):
        if len(self._pilha) == 0:
            raise Empty('pilha vazia')
        return self._pilha[-1]

    # desempilha elemento
    # excessao se pilha vazia
    def pop(self):
        if len(self._pilha) == 0:
            raise Empty('pilha vazia')
        return self._pilha.pop()
