import struct
VAZIO = -1

class No:
    _d = None
    _FORMATO = None
    TAMANHO = None

    @classmethod
    def configurar(cls, d: int) -> None:
        """Define o layout fixo do nó para a ordem d. Chamado UMA vez."""
        cls._d = d
        max_chaves = d - 1
        cls._FORMATO = (
            "ii" # num de chavez, é folha (0,1)
            + "i" * max_chaves          # chaves
            + "i" * max_chaves          # valor
            + "i" * d
        )
        cls.TAMANHO = struct.calcsize(cls._FORMATO)

    def __init__(self):
        d = No._d
        self.__num_chaves = 0
        self.__folha = True
        self.__chaves = [0] * (d - 1)
        self.__valores = [0] * (d - 1)
        self.__filhos = [VAZIO] * d

    # ---- acessores ----
    @property
    def num_chaves(self): return self.__num_chaves
    @property
    def folha(self): return self.__folha
    @folha.setter
    def folha(self, v): self.__folha = v

    def chave(self, i): return self.__chaves[i]
    def valor(self, i): return self.__valores[i]
    def filho(self, i): return self.__filhos[i]

    def set_chave(self, i, k): self.__chaves[i] = k
    def set_valor(self, i, v): self.__valores[i] = v
    def set_filho(self, i, f): self.__filhos[i] = f
    def set_num_chaves(self, n): self.__num_chaves = n

    def inserir_em(self, i, chave, valor, filho_dir=VAZIO):
        for j in range(self.__num_chaves, i, -1):
            self.__chaves[j] = self.__chaves[j - 1]
            self.__valores[j] = self.__valores[j - 1]
            self.__filhos[j + 1] = self.__filhos[j]
        self.__chaves[i] = chave
        self.__valores[i] = valor
        self.__filhos[i + 1] = filho_dir
        self.__num_chaves += 1

    def remover_em(self, i):
        """Remove a chave i e desloca à esquerda."""
        for j in range(i, self.__num_chaves - 1):
            self.__chaves[j] = self.__chaves[j + 1]
            self.__valores[j] = self.__valores[j + 1]
            self.__filhos[j + 1] = self.__filhos[j + 2]
        self.__num_chaves -= 1
        self.__filhos[self.__num_chaves + 1] = VAZIO

    # ---- serialização ----
    def to_bytes(self) -> bytes:
        d = No._d
        chaves = self.__chaves + [0] * ((d - 1) - len(self.__chaves))
        valores = self.__valores + [0] * ((d - 1) - len(self.__valores))
        filhos = self.__filhos + [-1] * (d - len(self.__filhos))
        folha = 1 if self.__folha else 0
        
        return struct.pack(
            No._FORMATO,
            self.__num_chaves, folha,
            *chaves[:d - 1], *valores[:d - 1], *filhos[:d],
        )

    @classmethod
    def from_bytes(cls, dados: bytes) -> "No":
        campos = struct.unpack(cls._FORMATO, dados)
        d = cls._d
        n = cls()
        n.__num_chaves = campos[0]
        n.__folha = bool(campos[1])
        base = 2
        n.__chaves = list(campos[base:base + (d - 1)])
        base += (d - 1)
        n.__valores = list(campos[base:base + (d - 1)])
        base += (d - 1)
        n.__filhos = list(campos[base:base + d])
        return n

    def __repr__(self):
        chaves = [self.__chaves[i] for i in range(self.__num_chaves)]
        return f"No(folha={self.__folha}, chaves={chaves})"