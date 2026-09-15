import sys

from ABB import ABB, busca, conta, insereElementoABB 
from ABB import ImprimeABBinLevel, ImprimeABBinOrder, ImprimeABBpreOrder, ImprimeABBpostOrder, altura
from filaListaEncadeada import FilaListaEncadeada
from pilhaListaEncadeada import PilhaListaEncadeada
from random import randint

n_args = len(sys.argv)

def main():
    print("Total arguments passed:", n_args)
    for i in range(n_args):
        # verificação de cada argumento de sys.argv
        print(sys.argv[i])

        # geração de n valores aleatórios no intervalo [0-999]
        # print(randint(0,1000))   
    abb = None
    for i in range(int(sys.argv[i])):
        if abb is None:
            abb = ABB(randint(0,1000))
        else:
            insereElementoABB(abb,randint(0,1000))


    print("resultado ImprimeABBinOrder: ")
    ImprimeABBinOrder(abb)
    print("resultado ImprimeABBinLevel: ")
    ImprimeABBinLevel(abb)
    print("resultado ImprimeABBpostOrder: ")
    ImprimeABBpostOrder(abb)
    # árvore deve ser gerada/populada

main()    