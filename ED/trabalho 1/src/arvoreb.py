import os
from no import No
from repositorio import Repositorio
RAIZ_VAZIA = -1

class ArvoreB():

    def __init__(self, ordem: int, repositorio: Repositorio):
        self.__d = ordem
        self.__repo = repositorio
        self.__raiz = RAIZ_VAZIA
        self.__num_nos = 0


    def inserir(self, chave: int, valor: int) -> None:
        if self.__raiz == RAIZ_VAZIA:
            raiz = No()
            raiz.inserir_em(0, chave, valor)
            self.__raiz = self.__repo.alocar(raiz)
            return

        raiz = self.__repo.ler(self.__raiz)
        # se a raiz está cheia, ela sobe (split) e ganha nova raiz
        if raiz.num_chaves == self.__d - 1:
            nova = No()
            nova.folha = False
            nova.set_filho(0, self.__raiz)
            self.__raiz = self.__repo.alocar(nova)
            self._split(nova, 0)
            self._inserir_nao_cheio(self.__raiz, chave, valor)
        else:
            self._inserir_nao_cheio(self.__raiz, chave, valor)

    def _min_chaves(self):
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

    def _split(self, pai: No, i: int) -> None:
        d = self.__d
        cheio = self.__repo.ler(pai.filho(i))
        t = (d - 1) // 2
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

    def _inserir_nao_cheio(self, indice: int, chave: int, valor: int) -> None:

        no = self.__repo.ler(indice)
        i = no.num_chaves - 1

        if no.folha:
            # procura posição de inserção (mantendo ordem crescente)
            while i >= 0 and chave < no.chave(i):
                i -= 1

            # chave repetida? atualiza o valor
            if i >= 0 and chave == no.chave(i):
                no.set_valor(i, valor)
                self.__repo.escrever(indice, no)
                return

            # insere na posição i+1 (desloca à direita)
            no.inserir_em(i + 1, chave, valor)
            self.__repo.escrever(indice, no)
            return

        # ---- caso 2: nó interno, desce para o filho apropriado ----
        while i >= 0 and chave < no.chave(i):
            i -= 1

        # chave repetida no nó interno? atualiza o valor
        if i >= 0 and chave == no.chave(i):
            no.set_valor(i, valor)
            self.__repo.escrever(indice, no)
            return

        # i+1 é o índice do filho onde a chave deve estar
        filho_idx = no.filho(i + 1)
        filho = self.__repo.ler(filho_idx)

        if filho.num_chaves == self.__d - 1:
            # filho cheio: split ANTES de descer
            self._split(no, i + 1)
            # após o split, o pai (no) ganhou uma chave em i+1
            # e o nó da direita foi criado. Precisamos decidir para qual lado descer.
            if chave > no.chave(i + 1):
                filho_idx = no.filho(i + 2)   # desce para a direita
            else:
                filho_idx = no.filho(i + 1)   # desce para a esquerda
            # se chave == no.chave(i+1), a chave já está no pai:
            if chave == no.chave(i + 1):
                no.set_valor(i + 1, valor)
                self.__repo.escrever(indice, no)
                return
            # reescreve o pai (o split já modificou em RAM, mas garantimos)
            self.__repo.escrever(indice, no)

        # desce recursivamente
        self._inserir_nao_cheio(filho_idx, chave, valor)

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
                # A1: folha, remove direto (garantido > mínimo pelo chamador)
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

        # garante mínimo ANTES de descer (top-down)
        #i = self._garantir_minimo(indice, no, i)
        no = self.__repo.ler(indice)   # reler: _garantir_minimo pode ter mudado o pai
        self._remover(no.filho(i), chave)


    