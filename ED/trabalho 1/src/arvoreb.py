# 2026230168 - Icaro Duarte Gavazza Lima

import os
from no import No
from repositorio import Repositorio
VAZIO = -1

class ArvoreB():

    def __init__(self, ordem: int, repositorio: Repositorio):
        self.__d = ordem
        self.__repo = repositorio
        self.__raiz = VAZIO
        self.__num_nos = 0


    def inserir(self, chave: int, valor: int) -> None:
        if self.__raiz == VAZIO:
            raiz = No()
            raiz.inserir_em(0, chave, valor)
            self.__raiz = self.__repo.alocar(raiz)
            return

        raiz = self.__repo.ler(self.__raiz)
        if raiz.num_chaves == self.__d - 1:
            nova = No()
            nova.folha = False
            nova.set_filho(0, self.__raiz)
            self.__raiz = self.__repo.alocar(nova)
            self._split(self.__raiz, nova, 0)
            self._inserir_nao_cheio(self.__raiz, chave, valor)
        else:
            self._inserir_nao_cheio(self.__raiz, chave, valor)

    def _inserir_nao_cheio(self, indice: int, chave: int, valor: int) -> None:
        no = self.__repo.ler(indice)
        i = no.num_chaves - 1

        if no.folha:
            while i >= 0 and chave < no.chave(i):
                i -= 1
            if i >= 0 and chave == no.chave(i):
                no.set_valor(i, valor)
                self.__repo.escrever(indice, no)
                return
            no.inserir_em(i + 1, chave, valor)
            self.__repo.escrever(indice, no)
            return

        while i >= 0 and chave < no.chave(i):
            i -= 1

        if i >= 0 and chave == no.chave(i):
            no.set_valor(i, valor)
            self.__repo.escrever(indice, no)
            return

        # desce para o filho i+1
        filho_idx = no.filho(i + 1)
        filho = self.__repo.ler(filho_idx)

        if filho.num_chaves == self.__d - 1:
            self._split(indice, no, i + 1)
            if chave == no.chave(i + 1):
                no.set_valor(i + 1, valor)
                self.__repo.escrever(indice, no)
                return
            if chave > no.chave(i + 1):
                filho_idx = no.filho(i + 2)   # direita
            else:
                filho_idx = no.filho(i + 1)   # esquerda

        self._inserir_nao_cheio(filho_idx, chave, valor)

    def _menor_chave(self, indice: int):
        """
        Retorna (chave, valor) da MENOR chave da subárvore enraizada em `indice`.
        Desce sempre pelo primeiro filho até achar uma folha.
        """
        no = self.__repo.ler(indice)
        while not no.folha:
            no = self.__repo.ler(no.filho(0))
        return no.chave(0), no.valor(0)

    
    def _min_chaves(self):
        if self.__d % 2 == 1:
            return (self.__d - 1) // 2 - 1
        return (self.__d + 1) // 2 - 1

    def _maior_chave(self, indice: int):
        """
        Retorna (chave, valor) da MAIOR chave da subárvore enraizada em `indice`.
        Desce sempre pelo último filho até achar uma folha.
        """
        no = self.__repo.ler(indice)
        while not no.folha:
            no = self.__repo.ler(no.filho(no.num_chaves))
        j = no.num_chaves - 1
        return no.chave(j), no.valor(j)


    def _merge(self, pai_indice: int, pai: No, i: int) -> None:
        esq = self.__repo.ler(pai.filho(i))
        dir_ = self.__repo.ler(pai.filho(i + 1))
        folha = esq.folha

        # 1) a chave separadora do pai desce para o fim do filho esquerdo.
        #    Se o filho não é folha, o filho à direita dessa chave é o
        #    PRIMEIRO filho do irmão direito (dir_.filho(0)).
        esq.inserir_em(
            esq.num_chaves,
            pai.chave(i), pai.valor(i),
            dir_.filho(0) if not folha else -1,
        )

        # 2) copia todas as chaves/valores/filhos do irmão direito
        #    para o fim do filho esquerdo.
        for j in range(dir_.num_chaves):
            esq.inserir_em(
                esq.num_chaves,
                dir_.chave(j), dir_.valor(j),
                dir_.filho(j + 1) if not folha else -1,
            )

        # 3) remove a chave i e o filho i+1 do pai.
        pai.remover_em(i)

        # 4) persiste os nós modificados.
        self.__repo.escrever(pai.filho(i), esq)   # filho esquerdo (agora fundido)
        self.__repo.escrever(pai_indice, pai)     # pai modificado
        # o irmão direito (dir_) fica órfão no arquivo — não é mais referenciado.


    def imprimir_largura(self) -> list[str]:
        if self.__raiz == -1:
            return []
        linhas = []
        fila = [self.__raiz]
        while fila:
            proximo = []
            partes = []
            for idx in fila:
                no = self.__repo.ler(idx)
                corpo = "".join(f"key: {no.chave(i)}, " for i in range(no.num_chaves))
                partes.append(f"[{corpo}]")
                if not no.folha:
                    for i in range(no.num_chaves + 1):
                        if no.filho(i) != -1:
                            proximo.append(no.filho(i))
            linhas.append(" ".join(partes))
            fila = proximo
        return linhas

    def _split(self, pai_indice: int, pai: No, i: int) -> None:
        d = self.__d
        cheio = self.__repo.ler(pai.filho(i))
        t = (d - 1) // 2 - 1 if d % 2 == 1 else (d - 1) // 2  # recomendação da IA para não quebrar em D=3

        direita = No()
        direita.folha = cheio.folha
        n_dir = cheio.num_chaves - t - 1
        direita.set_num_chaves(n_dir)
        for j in range(n_dir):
            direita.set_chave(j, cheio.chave(t + 1 + j))
            direita.set_valor(j, cheio.valor(t + 1 + j))
        if not cheio.folha:
            for j in range(n_dir + 1):
                direita.set_filho(j, cheio.filho(t + 1 + j))

        chave_meio = cheio.chave(t)
        valor_meio = cheio.valor(t)

        cheio.set_num_chaves(t)
        for j in range(t, d - 1):
            cheio.set_chave(j, 0)
            cheio.set_valor(j, 0)
        for j in range(t + 1, d):
            cheio.set_filho(j, -1)

        indice_direita = self.__repo.alocar(direita)
        pai.inserir_em(i, chave_meio, valor_meio, indice_direita)

        self.__repo.escrever(pai.filho(i), cheio)  
        self.__repo.escrever(pai_indice, pai)     



    # # ============ busca ============
    def buscar(self, chave: int) -> bool:
        if self.__raiz == -1:
            return False
        return self._buscar_rec(self.__raiz, chave)

    def _buscar_rec(self, indice: int, chave: int) -> bool:
        no = self.__repo.ler(indice)
        i = 0
        while i < no.num_chaves and chave > no.chave(i):
            i += 1
        if i < no.num_chaves and chave == no.chave(i):
            return True
        if no.folha:
            return False
        return self._buscar_rec(no.filho(i), chave)


    def remover(self, chave):
        if self.__raiz == -1 or not self.buscar(chave):
            return
        self._remover(self.__raiz, chave)
        raiz = self.__repo.ler(self.__raiz)
        if raiz.num_chaves == 0:
            self.__raiz = -1 if raiz.folha else raiz.filho(0)

    def _remover(self, indice: int, chave: int) -> None:
        no = self.__repo.ler(indice)
        i = 0
        while i < no.num_chaves and chave > no.chave(i):
            i += 1

        # ---- Caso A: chave encontrada neste nó ----
        if i < no.num_chaves and chave == no.chave(i):
            if no.folha:
                # A1: folha, remove direto
                no.remover_em(i)
                self.__repo.escrever(indice, no)
            else:
                # Caso B: chave em nó interno
                esq = self.__repo.ler(no.filho(i))
                dir_ = self.__repo.ler(no.filho(i + 1))
                min_c = self._min_chaves()

                if esq.num_chaves > min_c:
                    # B1: predecessor
                    pred_chave, pred_valor = self._maior_chave(no.filho(i))
                    no.set_chave(i, pred_chave)
                    no.set_valor(i, pred_valor)
                    self.__repo.escrever(indice, no)
                    self._remover(no.filho(i), pred_chave)
                elif dir_.num_chaves > min_c:
                    # B2: sucessor
                    suc_chave, suc_valor = self._menor_chave(no.filho(i + 1))
                    no.set_chave(i, suc_chave)
                    no.set_valor(i, suc_valor)
                    self.__repo.escrever(indice, no)
                    self._remover(no.filho(i + 1), suc_chave)
                else:
                    # B3: merge dos dois filhos com a chave do meio
                    self._merge(indice, no, i)
                    self._remover(no.filho(i), chave)   # remove do nó fundido
            return

        # ---- Caso C: chave não está aqui, desce ----
        if no.folha:
            return   # não encontrada (não deveria acontecer se pré-condição vale)

        # garante mínimo ANTES de descer (top-down)  ← REATIVADO
        i = self._garantir_minimo(indice, no, i)
        no = self.__repo.ler(indice)   # reler: empréstimo/merge pode ter mudado o pai
        self._remover(no.filho(i), chave)


    def _garantir_minimo(self, pai_indice: int, pai: No, i: int) -> int:
        filho = self.__repo.ler(pai.filho(i))

        # ---- 1) já tem folga? ----
        if filho.num_chaves > self._min_chaves():
            return i

        min_c = self._min_chaves()

        # ---- 2) tenta empréstimo do irmão esquerdo ----
        if i > 0:
            esq = self.__repo.ler(pai.filho(i - 1))
            if esq.num_chaves > min_c:
                self._pegar_emprestado(pai_indice, pai, i)
                return i

        # ---- 3) tenta empréstimo do irmão direito ----
        if i < pai.num_chaves:
            dir_ = self.__repo.ler(pai.filho(i + 1))
            if dir_.num_chaves > min_c:
                self._pegar_emprestado(pai_indice, pai, i)
                return i

        # ---- 4) ninguém pode emprestar → merge ----
        if i < pai.num_chaves:
            # funde i com i+1 (o filho i absorve o i+1)
            self._merge(pai_indice, pai, i)
            return i
        else:
            # i é o último filho: funde i-1 com i (o i-1 absorve o i)
            self._merge(pai_indice, pai, i - 1)
            return i - 1

    def _pegar_emprestado(self, pai_indice: int, pai: No, i: int) -> None:

        filho = self.__repo.ler(pai.filho(i))
        min_c = self._min_chaves()

        # ---- tenta irmão esquerdo ----
        if i > 0:
            esq = self.__repo.ler(pai.filho(i - 1))
            if esq.num_chaves > min_c:
                ultima = esq.num_chaves - 1

                # 1) a chave separadora do pai desce para o INÍCIO do filho.
                #    O filho à direita dela é o antigo ÚLTIMO filho do irmão esquerdo
                #    (esq.filho(ultima + 1)), se não for folha.
                filho.inserir_em(
                    0,
                    pai.chave(i - 1), pai.valor(i - 1),
                    esq.filho(ultima + 1) if not esq.folha else VAZIO,
                )

                # 2) a ÚLTIMA chave do irmão esquerdo sobe para o pai.
                pai.set_chave(i - 1, esq.chave(ultima))
                pai.set_valor(i - 1, esq.valor(ultima))

                # 3) remove a última chave (e o último filho) do irmão esquerdo.
                esq.remover_em(ultima)

                # 4) persiste os três nós modificados.
                self.__repo.escrever(pai.filho(i - 1), esq)
                self.__repo.escrever(pai.filho(i), filho)
                self.__repo.escrever(pai_indice, pai)
                return

        # ---- tenta irmão direito ----
        if i < pai.num_chaves:
            dir_ = self.__repo.ler(pai.filho(i + 1))
            if dir_.num_chaves > min_c:
                # 1) a chave separadora do pai desce para o FIM do filho.
                #    O filho à direita dela é o PRIMEIRO filho do irmão direito
                #    (dir_.filho(0)), se não for folha.
                filho.inserir_em(
                    filho.num_chaves,
                    pai.chave(i), pai.valor(i),
                    dir_.filho(0) if not dir_.folha else VAZIO,
                )

                # 2) a PRIMEIRA chave do irmão direito sobe para o pai.
                pai.set_chave(i, dir_.chave(0))
                pai.set_valor(i, dir_.valor(0))

                # 3) remove a primeira chave (e o primeiro filho) do irmão direito.
                #    O remover_em CORRIGIDO desloca filhos a partir de i+1,
                #    então filhos[0] recebe o antigo filhos[1] — exatamente o que
                #    queremos (o antigo filhos[0] já foi para o filho).
                dir_.remover_em(0)

                # 4) persiste os três nós modificados.
                self.__repo.escrever(pai.filho(i + 1), dir_)
                self.__repo.escrever(pai.filho(i), filho)
                self.__repo.escrever(pai_indice, pai)
                return