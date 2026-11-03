import os
from no import No

class ArvoreB:
    """
    TAD opaco: Árvore B persistida em arquivo binário.
    Os nós NÃO ficam em RAM; cada operação lê/escreve no disco.
    """
    def __init__(self, ordem: int, caminho_bin: str):
        self.__d = ordem
        self.__caminho = caminho_bin
        No.configurar(ordem)

        # cria/limpa o arquivo binário
        with open(self.__caminho, "wb") as f:
            pass

        self.__raiz = -1          # índice da raiz no arquivo (-1 = árvore vazia)
        self.__num_nos = 0

    # ============ persistência (memória secundária) ============
    def _ler_no(self, indice: int) -> No:
        with open(self.__caminho, "rb") as f:
            f.seek(indice * No.TAMANHO)
            return No.from_bytes(f.read(No.TAMANHO))

    def _escrever_no(self, indice: int, no: No) -> None:
        with open(self.__caminho, "r+b") as f:
            f.seek(indice * No.TAMANHO)
            f.write(no.to_bytes())

    def _alocar_no(self, no: No) -> int:
        indice = self.__num_nos
        self.__num_nos += 1
        self._escrever_no(indice, no)
        return indice

    # ============ busca ============
    def buscar(self, chave: int) -> bool:
        if self.__raiz == -1:
            return False
        return self._buscar_rec(self.__raiz, chave)

    def _buscar_rec(self, indice: int, chave: int) -> bool:
        no = self._ler_no(indice)
        i = 0
        while i < no.num_chaves and chave > no.chave(i):
            i += 1
        if i < no.num_chaves and chave == no.chave(i):
            return True
        if no.folha:
            return False
        return self._buscar_rec(no.filho(i), chave)

    # ============ inserção ============
    def inserir(self, chave: int, valor: int) -> None:
        if self.__raiz == -1:
            raiz = No()
            raiz.inserir_em(0, chave, valor)
            self.__raiz = self._alocar_no(raiz)
            return

        raiz = self._ler_no(self.__raiz)
        # se a raiz está cheia, ela sobe (split) e ganha nova raiz
        if raiz.num_chaves == self.__d - 1:
            nova = No()
            nova.folha = False
            nova.set_filho(0, self.__raiz)
            self.__raiz = self._alocar_no(nova)
            self._split(nova, 0)
            self._inserir_nao_cheio(self.__raiz, chave, valor)
        else:
            self._inserir_nao_cheio(self.__raiz, chave, valor)

    # (implementar _split, _inserir_nao_cheio, e toda a remoção com
    #  empréstimo/merge — ver seção 3.2 do enunciado)

    # ============ impressão em largura ============
    def imprimir_largura(self, caminho_saida: str) -> None:
        """Imprime a árvore nível a nível no arquivo de saída."""
        with open(caminho_saida, "a", encoding="utf-8") as out:
            out.write("-- ARVORE B\n")
            if self.__raiz == -1:
                return
            fila = [self.__raiz]
            while fila:
                proximo = []
                linha = ""
                for idx in fila:
                    no = self._ler_no(idx)
                    for i in range(no.num_chaves):
                        linha += f"[key: {no.chave(i)}, "
                    if no.num_chaves > 0:
                        # fecha o colchete do nó
                        linha = linha.rstrip(", ") + ", ]" if False else linha
                    # formatação correta do nó: [key: x, key: y, ]
                    linha_no = "[" + "".join(
                        f"key: {no.chave(i)}, " for i in range(no.num_chaves)
                    ) + "]"
                    linha += linha_no + " " if not linha else ""
                    # (reescrever com cuidado — veja observação abaixo)
                    if not no.folha:
                        for i in range(no.num_chaves + 1):
                            if no.filho(i) != -1:
                                proximo.append(no.filho(i))
                out.write(linha.rstrip() + "\n")
                fila = proximo