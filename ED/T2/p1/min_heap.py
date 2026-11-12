"""
Disciplina: EDPAA 2026/2 — T2 Parte 1
Aluno: Icaro Duarte Gavazza Lima — Matrícula: 2026230168
"""

class MinHeap:

    def __init__(self):
        """Cria um heap vazio."""
        self._heap = []      # list[tuple[float, item]] — (prioridade, item)
        self._pos = {}       # dict[item, int]          — item -> índice
        self._size = 0

    def push(self, priority, item):
        """Insere (priority, item) ou atualiza se `item` já existe.

        Se `item` já está no heap:
          - nova prioridade < atual  -> decrease_key (sobe no heap)
          - nova prioridade >= atual -> no-op (mantém a menor já registrada)
        """
        if item in self._pos:
            idx = self._pos[item]
            if priority < self._heap[idx][0]:
                self._heap[idx] = (priority, item)
                self._sift_up(idx)
            return

        self._heap.append((priority, item))
        self._pos[item] = self._size
        self._size += 1
        self._sift_up(self._size - 1)

    def pop_min(self):
        """Remove e devolve (priority, item) de menor prioridade.

        Lança IndexError se o heap estiver vazio.
        """
        if self.is_empty():
            raise IndexError("pop_min em heap vazio")

        top = self._heap[0]
        last_idx = self._size - 1

        # Move o último elemento para a raiz e remove o último slot.
        self._swap(0, last_idx)
        self._heap.pop()
        del self._pos[top[1]]
        self._size -= 1

        if self._size > 0:
            self._sift_down(0)

        return top

    def decrease_key(self, item, new_priority):

        if item not in self._pos:
            raise KeyError(f"decrease_key: item ausente: {item!r}")

        idx = self._pos[item]
        if new_priority >= self._heap[idx][0]:
            raise ValueError(
                f"decrease_key requer prioridade menor "
                f"(atual={self._heap[idx][0]}, nova={new_priority})"
            )

        self._heap[idx] = (new_priority, item)
        self._sift_up(idx)

    def is_empty(self):
        return self._size == 0

    def __len__(self):
        return self._size


    def _swap(self, i, j):
        if i == j:
            return
        self._heap[i], self._heap[j] = self._heap[j], self._heap[i]
        self._pos[self._heap[i][1]] = i
        self._pos[self._heap[j][1]] = j

    def _sift_up(self, i):
        while i > 0:
            parent = (i - 1) // 2
            if self._heap[i][0] >= self._heap[parent][0]:
                break
            self._swap(i, parent)
            i = parent

    def _sift_down(self, i):
        n = self._size
        while True:
            left = 2 * i + 1
            right = 2 * i + 2
            smallest = i

            if left < n and self._heap[left][0] < self._heap[smallest][0]:
                smallest = left
            if right < n and self._heap[right][0] < self._heap[smallest][0]:
                smallest = right

            if smallest == i:
                break
            self._swap(i, smallest)
            i = smallest