class ListaEncadeadaSimples:
    
    # Implementa uma LE simples

    # classe _Node - interna
    class _Node:

        __slots__ = '_info', '_prox'
        
        def __init__ (self, info, prox):
            # inicia os campos
            self._info = info
            self._prox = prox

    # métodos de LE

    def __init__ (self):
        # cria uma fila vazia
        self._inicio = None # vazia
        self._tamanho = 0   # tamanho da LE
        
    def __len__(self):
        # retorna o tamanho da LE
        return self._tamanho

    def is_empty(self):
        # retorna True se LE vazia
        return self._tamanho == 0
    
    def first(self):
        # retorna o campo info da LE sem remover
        # sinaliza exceção se LE vazia
        if self.is_empty():
            raise Empty("Lista encadeada Vazia")
        return self._inicio._info # elemento inicial da LE

    def adiciona(self, e):
        # adiciona elemento no inicio da LE
        # novo nó referencia o inicio da LE
        novo = self._Node(e, self._inicio)
        # novo nó será o inicio da LE
        self._inicio = novo
        self._tamanho += 1

    def adiciona_afrente(self, p, e):
        # adiciona elemento a frente de p
        # novo nó referencia o mesmo que p
        novo = self._Node(e, p._prox)
        # p referencia o novo nó
        p._prox = novo
        self._tamanho += 1
    def Adiciona_emOrdem(self, info):
        anterior = None
        atual = self._inicio

        #caso seja uma lista vazia
        if self.is_empty():
            self._inicio = self._Node(info, None)
            return
        if atual._info > info:
            self._inicio = self._Node(info, atual)
            return
        # busca o maior e adiciona antes do mesmo.
        while atual is not None:
            if atual._info > info:
                anterior._prox = self._Node(info, atual)
                return
            anterior = atual
            atual = atual._prox
        # caso não exista um nó maior, ele adiciona ao final.    
        anterior._prox = self._Node(info, atual)

    def conta(self, x):
        no = self._inicio
        count = 0
        while no is not None:
            if no._info == x:
                count+=1
        return count

    def comparaLE(self, lex: ListaEncadeadaSimples):
        no_self = self._inicio
        no_x = lex._inicio
        while no_self is not None and no_x is not None:
            if no_self._info != no_x._info:
                return False
            no_self = no_self._prox
            no_x = no_x._prox

        if no_self is None and no_x is None:
            return True
        else:
            return False
    
    def remove_afrente(self, p):
        # remove elemento a frente de p
        # caso particular - p não pode ser o último
        if p._prox is None:
            raise Empty("Nó inexistente") 
        # p referencia o mesmo que o próximo
        p._prox = p._prox._prox
        self._tamanho -= 1

    def busca(self, e):
        # procura elemento com info = e
        # devolve referência para esse elemento ou None
        # p percorre a lista
        if self.is_empty():
            return None
        p = self._inicio
        while p is not None:
            if p._info == e: return p # achou
            p = p._prox # vai para o próximo
        # se chegou aqui é porque não achou
        return None        

    def busca_ant(self, e):
        # procura elemento com info = e
        # devolve uma dupla de referências (p_ant, p)
        # p percorre a lista e pant referencia o anterior
        pant, p = None, self._inicio
        while p is not None:
            if p._info == e:
                return pant, p
            pant, p = p, p._prox
        return pant, p

    def CriaLE(self, vet):
        # cria uma LE com elementos do vetor vet
        for k in range(len(vet) - 1, -1, -1):
            self.adiciona(vet[k])
  
    def ImprimeLE(self):
        # só para teste
        p = self._inicio
        k = 1
        print("Imprimindo a lista encadeada:")
        while p is not None:
            print(k, " - ", p._info)
            k = k + 1
            p = p._prox

# Cria lista encadeada
cores = ["vermelho", "preto", "azul", "amarelo", "verde", "branco"]
LEC = ListaEncadeadaSimples()
LEC.CriaLE(cores)
LEC.ImprimeLE()

# Verifica se algumas cores estão na lista
while True:
    cor = input("\nEntre com o nome da cor:")
    if cor == 'fim': break
    if LEC.busca(cor) is None:
        print("A cor", cor, "não está na lista encadeada")
    else:
        print("Encontrada a cor", cor, "na lista encadeada")

print("\nAdicionando laranja e violeta depois e antes da azul")





# Adicionar cor laranja a frente do azul
anterior, atual = LEC.busca_ant("azul")
if atual is None:
    print("Cor azul não está na lista")
else:
    LEC.adiciona_afrente(atual, "laranja")

# Adicionar cor violeta antes da azul
if anterior is None:
    print("Cor azul não tem anterior")
else:
    LEC.adiciona_afrente(anterior, "violeta")

LEC.ImprimeLE()

print("\nRemovendo as cores amarelo e lilás da lista")

# Remove cor amarelo
anterior, atual = LEC.busca_ant("amarelo")
if atual is None or anterior is None:
    print("Cor amarelo não está na lista ou não tem anterior")
else:
    LEC.remove_afrente(anterior)

# Remove cor lilas
anterior, atual = LEC.busca_ant("lilas")
if atual is None or anterior is None:
    print("Cor lilas não está na lista ou não tem anterior")
else:
    LEC.remove_afrente(anterior)
    
LEC.ImprimeLE()

# teste de len e first
print("\nTamanho da LE = ", len(LEC))
print("\nPrimeiro elemento da LE = ", LEC.first())