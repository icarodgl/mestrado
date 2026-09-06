from pilha import PilhaLista, Empty, UnknownOperator

def prioridade(x):
    dict_prioridade = {
        '+':1,
        '-':1,
        '*':2,
        '/':2,
        '^':3,
        '(':4,
        ')':5
        }
    return dict_prioridade.get(x,0)

def is_operador(a : str):
    return (prioridade(a) > 0)

def is_operando(a : str):
    return  (prioridade(a) == 0 )

# Transformação de in-fixa para pós-fixa

'''
Função que recebe uma expressão em notação in-fixa e retorna sua correspondente em notação 
pós-fixa. As expressões de entrada e saída são descritas como uma lista de strings.

@param inFixa: Lista de strings com a entrada -- expressão na notação in-fixa
@return posFixa: Lista de strings com a saída em notação pós-fixa
'''
def fromInToPos(inFixa: str):
    pilha = PilhaLista()
    posfixa = PilhaLista()
    lista = inFixa.split()
    
    for p in lista:
        if is_operando(p): #if (p é operando): coloque na pós-fixa
            posfixa.push(p)

        elif p == '(': # if (p é abre parêntesis): empilha p;
            pilha.push(p)

            """ if (p é fecha parêntesis): 
                desempilhe os operadores até o primeiro abre e coloque
                na pós-fixa na mesma ordem de retirada da pilha;
            """
        elif p == ')':
            while not pilha.is_empty() and pilha.top() != '(':
                posfixa.push(pilha.pop())
            if not pilha.is_empty() and pilha.top() == '(':
                pilha.pop()  # remove o '('

            """
            if (p é operador):
            tire da pilha e coloque na pós-fixa todos os operadores com prioridade maior ou igual a p, 
            na mesma ordem de retirada da pilha empilhe p"""
        else: 
            while (not pilha.is_empty() and pilha.top() != '(' and prioridade(p) <= prioridade(pilha.top())):
                posfixa.push(pilha.pop())
            pilha.push(p)
    
    """desempilhe todos os operadores que ainda estão na pilha e coloque na pós-fixa na mesma ordem de retirada da pilha"""
    while not pilha.is_empty():
        posfixa.push(pilha.pop())
    
    return ' '.join(posfixa)


'''
Função que recebe uma expressão em notação pós-fixa e retorna a sua resolução matemática.

@param posFixa: Lista de strings com a entrada -- expressão na notação pós-fixa
@return resultado: valor final da expressão, após sua resolução.
'''
def solvePosFixa(posFixa:str):
    lista = posFixa.split()
    pilha = PilhaLista()

    for p in lista:
    #if (p é operando): empilha p;
        if is_operando(p):
            pilha.push( int(p) )

    #if (p é operador binário):
    #    faz a operação com os 2 elementos do topo da pilha
    #   neste caso a pilha diminui de 1 elemento"""
        else:
            a = int(pilha.pop())
            b = int(pilha.pop())
            pilha.push(operar( b , a , p ))
            

    return pilha.pop()

def operar(a : int, b : int , operador: str):
    if operador == '+':
        return a + b
    if operador == '-':
        return a - b
    if operador == '*':
        return a * b
    if operador == '/':
        return a / b
    if operador == '^':
        return a ^ b
    raise Exception('Operador não inválido')
     
def main():
    # exemplos de entrada
    # inFixa = "12 + ( 13 - 4 * 2 ) * 15" # resultado = 87
    inFixa = "12 + 13 - 4 * 2 * 15" # resultado = -95

    # transforma a string de entrada em lista de strings

    # chamada da transformação de notações (in-fixa -> pós-fixa)
    posFixa = fromInToPos(inFixa)

    # impressão das notações em formato de string
    print("Notação in-fixa: ", "".join(inFixa))
    print("Notação pós-fixa: ", "".join(posFixa))

    # resolução da equação em notação pós-fixa
    print(solvePosFixa(posFixa))
    
main()