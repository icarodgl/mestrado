class OutraListaEncadeadaSimples:
    
    ''' Nesta nova definição de LE, Uma LE é sempre um par:
        (info, prox), onde prox é uma lista encadeada.
        Quando info == None a lista encadeada é vazia. 
    '''

    def __init__ (self, info = None, prox= None):
        # cria nova LE
        self._info = info
        self._prox = prox

    def is_empty(self):
        # retorna True se LE vazia
        return self._info == None
    
    def info(self):
        # retorna o campo info da LE sem remover.
        # sinaliza exceção se LE vazia
        if self.is_empty():
            raise Empty("Lista encadeada Vazia")
        return self._info # informação desta LE

    def adicionaLE(self, e):
        # adiciona uma LE com info = e antes desta LE
        # novo nó referencia o inicio da LE
        novaLE = OutraListaEncadeadaSimples(e, self)
        return novaLE

    def busca(self, e):
        # procura LE com info = e. Devolve referência
        # para a LE encontrada ou None
        # A função é recursiva
        # p percorre a lista
        if self.is_empty(): return None
        if self._info == e: return self
        return self._prox.busca(e)

    def conta(self ,x , _count=0):
        if self.info == None:
            return _count
        if self.info == x:
            _count+=1
        return self._prox.conta(x, _count)

    
    def CriaLE(self, lista):
        # cria uma LE com elementos contidos em lista
        # for k in range(len(lista) - 1, -1, -1):
        #     self.adiciona(lista[k])

        self.adiciona(lista[len(lista)-1:])

        if len(lista) == 1:
            return lista[1]
        return self.CriaLE(lista[:len(lista)-1])

    def ImprimeLE(self):
        # imprime toda a LE
        p = self
        k = 1
        print("Imprimindo a lista encadeada:")
        while p is not None:
            print(k, " - ", p._info)
            k = k + 1
            p = p._prox

# Testes
# Cria lista encadeada
cores = ["vermelho", "preto", "azul", "amarelo", "verde", "branco"]
LEC = OutraListaEncadeadaSimples()
for cor in cores:
    LEC = LEC.adicionaLE(cor)
LEC.ImprimeLE()

# Procura alguns elementos
outrascores = ["laranja", "violeta", "lilas"]
for cor in cores + outrascores:
    if LEC.busca(cor) != None: print(cor, "está na LE")
    else: print(cor, "não está na LE")