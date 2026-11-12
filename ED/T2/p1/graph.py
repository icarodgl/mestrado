"""
Disciplina: EDPAA 2026/2 — T2 Parte 1
Aluno: Icaro Duarte Gavazza Lima — Matrícula: 2026230168
"""


class Graph:
 
    def __init__(self):
        """Cria um grafo vazio."""
        self._adj = {}          # dict[str, list[tuple[str, float]]]
        self._nodes = []        # list[str] — ordem de inserção (iteração estável)
        self._edge_count = 0

    def add_node(self, name):
        """Adiciona o nó `name` se ainda não existir. Idempotente."""
        if name not in self._adj:
            self._adj[name] = []
            self._nodes.append(name)

    def add_edge(self, src, dst, weight):
        """Adiciona aresta direcionada src -> dst com peso `weight`."""
        if weight <= 0:
            raise ValueError(f"peso deve ser > 0 (recebido: {weight!r})")

        self.add_node(src)
        self.add_node(dst)
        self._adj[src].append((dst, float(weight)))
        self._edge_count += 1


    def has_node(self, name):
        """True se `name` pertence a V."""
        return name in self._adj

    def nodes(self):
        """Itera sobre os nomes dos nós (ordem de inserção)."""
        return iter(self._nodes)

    def neighbors(self, node):
        """Itera sobre pares (destino, peso) adjacentes a `node`.

        Lança KeyError se `node` não existir (uso incorreto do consumidor).
        """
        if node not in self._adj:
            raise KeyError(f"nó inexistente: {node!r}")
        return iter(self._adj[node])

    def size(self):
        """Número de nós |V|."""
        return len(self._nodes)

    def edge_count(self):
        """Número de arestas |E|."""
        return self._edge_count


    def __len__(self):
        """Equivalente a size()."""
        return len(self._nodes)

    def __repr__(self):
        return (f"Graph(|V|={self.size()}, |E|={self._edge_count})")