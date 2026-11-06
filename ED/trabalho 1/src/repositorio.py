from no import No


class Repositorio:

    def __init__(self, caminho: str):
        self.__caminho = caminho
        self.__num_nos = 0
        with open(self.__caminho, "wb"):
            pass   # cria/trunca

    @property
    def num_nos(self) -> int:
        return self.__num_nos

    def ler(self, indice: int) -> No:
        with open(self.__caminho, "rb") as f:
            f.seek(indice * No.TAMANHO)
            return No.from_bytes(f.read(No.TAMANHO))

    def escrever(self, indice: int, no: No) -> None:
        with open(self.__caminho, "r+b") as f:
            f.seek(indice * No.TAMANHO)
            f.write(no.to_bytes())

    def alocar(self, no: No) -> int:
        """Grava um novo nó e devolve o índice."""
        indice = self.__num_nos
        self.__num_nos += 1
        self.escrever(indice, no)
        return indice