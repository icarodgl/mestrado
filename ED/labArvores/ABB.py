from filaListaEncadeada import FilaListaEncadeada
class ABB:

    def __init__ (self, raiz):
        # cria uma nova ABB com o info raiz e sem filhos
        self._info = raiz
        self._eprox = None
        self._dprox = None

def push(raiz:ABB, e:int):
    #A insercao de uma chave que ja existe na ABB termina silenciosamente sem modificar a estrutura
    if raiz._info == e:
        return

    if raiz._info > e:
        if raiz._dprox is None:
            raiz._dprox = ABB(e)
            return
        else:
            push(raiz._dprox,e)
    else:
        if raiz._eprox is None:
            raiz._eprox = ABB(e)
            return
        else:
            push(raiz._eprox, e)


def push_interativo(raiz,e):
    no = raiz
    continua = True
    while continua:
        #A insercao de uma chave que ja existe na ABB termina silenciosamente sem modificar a estrutura
        if no._info == e:
            return

        if no._info > e:
            if no._dprox is None:
                no._dprox = ABB(e)
                continua = False
            else:
                no = no._dprox
        else:
            if no._eprox is None:
                no._eprox = ABB(e)
                continua = False
            else:
                no = no._eprox
    return


# Procura elemento com info igual a v na ABB h
# Versão recursiva
def busca(h:ABB, v:int):
    if h is None:
        return None

    if h._info == v:
        return h

    if h._info > v:
        return busca(h._dprox,v)
    else:
        return busca(h._eprox,v)

# conta elementos com info igual a v na ABB h
def conta(h:ABB, v:int):
    if h is None:
        return 0

    if h._info == v:
        count = 1
    else:
        count = 0

    return count + conta(h._eprox, v) + conta(h._dprox,v)

def contaNN(h):
   if h is None: return 0
   return 1 + contaNN(h._eprox) + contaNN(h._dprox)

# altura de uma ABB - raíz é altura 0
def altura(h:ABB):
    if h is None:
        return 0
    esquerda = altura(h._eprox)
    direita = altura(h._dprox)

    return max(esquerda,direita) + 1


def ImprimeABBinOrder(h:ABB):
    if h is None:
        return
    ImprimeABBinOrder(h._eprox)
    print(h._info)
    ImprimeABBinOrder(h._dprox)



def ImprimeABBpreOrder(h:ABB):
    if h is None:
        return
    print(h._info)
    ImprimeABBpreOrder(h._eprox)
    ImprimeABBpreOrder(h._dprox)

def ImprimeABBpostOrder(h:ABB):
    if h is None:
            return
    ImprimeABBpostOrder(h._eprox)
    ImprimeABBpostOrder(h._dprox)
    print(h._info)


def ImprimeABBinLevel(h):
    if h is None:
        return

    fila = FilaListaEncadeada()
    fila.enqueue(h)
    nivel = 0
    while not fila.is_empty():
        print('Nivel',nivel)
        nivel +=1
        tamanho_nivel = len(fila)

        for _ in range(tamanho_nivel):
            no_atual = fila.dequeue()
            print(no_atual._info, end=' ')

            if no_atual._eprox is not None:
                fila.enqueue(no_atual._eprox)
            if no_atual._dprox is not None:
                fila.enqueue(no_atual._dprox)
        print()  
        



def insereElementoABB(h:ABB, elemento:int):# aceita repeticao
    if h._info >= elemento:
        if h._dprox is None:
            h._dprox = ABB(elemento)
            return
        else:
            push(h._dprox,elemento)
    else:
        if h._eprox is None:
            h._eprox = ABB(elemento)
            return
        else:
            push(h._eprox, elemento)

# Monta uma ABB a partir de uma lista
# Elementos repetidos devem ficar a direita
def montaABB(a):
    raiz = None
    for e in a:
        if raiz is None:
            raiz = ABB(e)
        else:
            insereElementoABB(raiz, e)
    return raiz



lista = [1,2]
mabb = montaABB(lista)

a = altura(mabb)
print(a)

'''
Testes

outralista = [10, 4, 2, 30, 7, 15, 40, 27, 60, 6]
mabb = montaABB(lista, len(lista))
moabb = montaABB(outralista, len(outralista))

ImprimeABBinOrder(mabb)
print()
ImprimeABBinOrder(moabb)
print()

ImprimeABBpreOrder(mabb)
print()
ImprimeABBpreOrder(moabb)
print()

ImprimeABBpostOrder(mabb)
print()
ImprimeABBpostOrder(moabb)
print()

ImprimeABBinLevel(mabb)
print()
ImprimeABBinLevel(moabb)
print()
'''

