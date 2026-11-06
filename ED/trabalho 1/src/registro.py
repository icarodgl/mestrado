import struct
from no import No

class Registro:

    def __init__(self, caminho_bin: str):
        self.__caminho = caminho_bin

    def limpar_registros(self,caminho):
        with open(caminho, "wb") as f:
            pass

    def _ler_no(self, indice: int) -> No:
        with open(self.__caminho, "rb") as f:
            f.seek(indice * No.TAMANHO)
            return No.from_bytes(f.read(No.TAMANHO))

    def _escrever_no(self, indice: int, no: No) -> None:
        with open(self.__caminho, "r+b") as f:
            f.seek(indice * No.TAMANHO)
            f.write(no.to_bytes())