import struct

class Registro:
    # Layout fixo: chave (int) + valor (int) -> 8 bytes
    _FORMATO = "ii"
    TAMANHO = struct.calcsize(_FORMATO)

    def __init__(self, chave: int, valor: int):
        self.__chave = chave
        self.__valor = valor

    # ---- acessores (encapsulamento) ----
    @property
    def chave(self) -> int:
        return self.__chave

    @property
    def valor(self) -> int:
        return self.__valor

    @valor.setter
    def valor(self, novo: int) -> None:
        self.__valor = novo

    def to_bytes(self) -> bytes:
        return struct.pack(self._FORMATO, self.__chave, self.__valor)

    @classmethod
    def from_bytes(cls, dados: bytes) -> "Registro":
        chave, valor = struct.unpack(cls._FORMATO, dados)
        return cls(chave, valor)

    def __repr__(self):
        return f"Registro(chave={self.__chave}, valor={self.__valor})"