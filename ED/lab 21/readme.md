### 1- Aplique a estratégia da divisão e conquista ao problema de calcular a soma dos elementos de um vetor A[1 .. n] de números inteiros. Estime o consumo de tempo do algoritmo. Compare o resultado com o consumo de tempo do algoritmo trivial de soma.

Teorema mestre:
    T(n) = aT(n/b) + f (n)

a = 2 dois problemas
b = 2 divididos em dois
n = 1 soma das partes

2T(1/2) + f(1)

log²(2) = 1
então: T(n).
Linear assim como o algoritmo trivial.
Mas na máquina tem um ovehead gigante por conta das chamadas recursivas e calculo de meio que deixa bem pior.👍

---


### 2- Escreva e implemente um algoritmo de divisão e conquista para encontrar o valor de um elemento máximo de um vetor A[p .. r] de números inteiros. Seja n o número r − p + 1 e C(n) o número de comparações que seu algoritmo faz entre elementos do vetor. Escreva a recorrência que C(n) satisfaz. Resolva a recorrência utilizando o teorema mestre.

a = 2
b = 2
n = 1

2T(1/2) + f(1)

log²​(2) = 1
então: T(n).

---

### 3- Distância τ de Kendall. Suponha dadas duas permutações, digamos A[1 .. n] e B[1 .. n], de um mesmo conjunto de números. A distância τ de Kendall entre A e B é o número de pares de elementos do conjunto que estão em ordem diferente em A e B, ou seja, o número ⎮X − Y⎮ onde X é o conjunto de todos os pares (A[i], A[j]) tais que i < j e Y é o conjunto de todos os pares (B[i], B[j]) tais que i < j. (A definição não é assimétrica pois ⎮X − Y⎮ = ⎮Y − X⎮.) Escreva uma função eficiente que calcule a distância τ de Kendall entre A e B.


---
### 4- Dado um conjunto de pontos no espaço bidimensional, você deve encontrar a distância entre os dois pontos mais próximos. Entrada: O arquivo de entrada consiste de vários casos de teste. Cada começa com um inteiro N (0 ≤ N ≤ 10000), que denota o número de pontos no teste. As próximas N linhas contém as coordenadas dos N pontos bidimensionais. O primeiro inteiro denota a coordenada X e o segundo a coordenada Y de cada ponto. A entrada é terminada por um caso de teste com N = 0. O valor de cada coordenada deverá ser sempre maior ou igual a zero e menor que 40000. Saída: Para cada caso de teste deve ser impressa uma única linha contendo um ponto flutuante (com quatro casas decimais) representando a menor distância entre dois pontos. Se não houver nenhum par de pontos com distância menor que 10000, devem ser impressos os caracteres “INFINITY”.
