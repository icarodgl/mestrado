# Variantes do merge sort

Para ver os gráficos use:

`python3 -m http.server 8000` 

## Análise / Relatório:

    Em meus testes, a pesar de rodar ao menos 3x para cada algoritmo, ouve variações sutis de tempos que me confundem as análises.
    Porém, sabendo disso, é facil perceber o quão bom o Merge sorte se torna ao aplicar `Cut-Off` e `Skip Merge`.
    O uso de Cut-off com tamanho 12, para as listas de 100k foi o que teve melhor desempenho, mesmo não avendo muita variação, e algumas vezes a variação era do momento do computador.

## Versão 1

    Algoritmo muito rápido mesmo sem melhorias.

 ## Versão 2

    Fiz teste com valoresde cutoff em 1 a 10, depois subi arbitrariamente para 12, 16, 24, 32, 48 e 64 para testar o desempenho.
    Os valores do cutoff pra 12 nas rodadas que fiz me pareciam promissoras, rodei ao menos 3x o teste para cada, pois as vezes surgiram valores discrepantes.
    Os valores a cima de 16 começa a ficar ruim para as listas invertidas por conta do insertion sort.
 
 ## Versão 3
    
    O Merge Skip melhora dasticamente o desempenho do algoritmo em listas já ordenadas ou semi ordenadas, As listas de 100k se aproximam do desempenho das listas de 10K.

## Versão 4

    A fusão das duas abordagens obviamente ajudou no desempenho, o skip merge ajuda muito a pulando as partes pré ordenadas, eliminando trabalho resultando em um desempenho muito bom.

## Versão 5

    Imaginei que o desempenho fosse terrivelmente pior, mas me surpreendi. pois ele mantem a proporção do metodo recursivo.


## Versão 6

    Comparado com a versão recursiva e usando o mesmo valor de cutoff de 12, a versão recursiva ainda é um pouco mais rápido, porém  pouquissima diferença.

## Versão 7

    O desempenho muito proximo da versão 4, recursiva implementando cutoff e skip merge, ambas as abordagens parecem boas, mas o recursivo ainda ganha um pouco no desempenho e na facilidade de implementação.


