"""
Disciplina: EDPAA 2026/2 — T2 Parte 1
Aluno: Icaro Duarte Gavazza Lima — Matrícula: 2026230168
"""


class FileIO:

    def __init__(self, input, output):
        self.__input_path = input
        self.__output_path = output

    def read_graph(self):
        pass
        """Lê o arquivo de entrada e devolve (src, Graph).

        Formato: primeira linha = Vsrc; depois |V| linhas no formato
        'node_i, d0, d1, ..., d_{|V|-1}' (com d_i ignorado / zero).
        Aresta i->j existe quando d_j > 0.
        """
        ...


    def write_results(self, results):
        """Grava os ShortestPath em `path`, ordenados por custo crescente,
        uma linha por destino no padrão de `ShortestPath.format_line()`."""
        ...