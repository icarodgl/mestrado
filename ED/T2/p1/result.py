"""
Disciplina: EDPAA 2026/2 — T2 Parte 1
Aluno: Icaro Duarte Gavazza Lima — Matrícula: 2026230168
"""



class ShortestPath:
    """Caminho mínimo da origem até `destination`."""

    def __init__(self, destination, path, cost):
        """`path` é a lista de nós na ordem origem->destino; `cost` é float."""
        self._destination = destination
        self._path = tuple(path)   # imutável
        self._cost = float(cost)

    def destination(self):
        """Nome do nó de destino."""
        ...

    def path(self):
        """Tupla de nós do caminho (origem primeiro)."""
        ...

    def cost(self):
        """Custo total (float)."""
        ...

    def format_line(self):
        """Retorna a linha no formato:
        'SHORTEST PATH TO <dest>: <dest> <- ... <- <orig> (Distance: X.XX)'
        """
        ...