"""
Disciplina: EDPAA 2026/2 — T2 Parte 1
Aluno: Icaro Duarte Gavazza Lima — Matrícula: 2026230168
"""

import sys

from file_io import FileIO
from dijkstra import shortest_paths


# Códigos de saída (convenção POSIX: 0 = sucesso)
_EXIT_OK = 0
_EXIT_USAGE = 1          # argumentos inválidos
_EXIT_INPUT_ERROR = 2    # arquivo de entrada inexistente/ilegível/malformado
_EXIT_OUTPUT_ERROR = 3   # não foi possível gravar o arquivo de saída


def _usage_error(prog_name):
    return f"Uso: python {prog_name} <arquivo_entrada> <arquivo_saida>"


def _validate_args(argv):
    prog = argv[0] if argv else "trab2.py"
    args = argv[1:]

    if len(args) != 2:
        print(_usage_error(prog), file=sys.stderr)
        return None

    input_path = args[0]
    output_path = args[1]

    return input_path, output_path


def main(argv):
    # Validação dos argumentos
    validated = _validate_args(argv)
    if validated is None:
        return _EXIT_USAGE
    input_path, output_path = validated

    file_io = FileIO(input_path,output_path)

    # 2) Leitura do grafo (I/O isolado em file_io)
    try:
        src, graph = file_io.read_graph()
    except FileNotFoundError:
        print(f"Erro: arquivo de entrada não encontrado: {input_path}",
              file=sys.stderr)
        return _EXIT_INPUT_ERROR
    except (OSError, ValueError) as exc:
        print(f"Erro ao ler entrada '{input_path}': {exc}", file=sys.stderr)
        return _EXIT_INPUT_ERROR

    # 3) Execução do algoritmo (função pura, sem estado global)
    results = shortest_paths(graph, src)

    # 4) Escrita da saída (ordenada por custo; feito em file_io)
    try:
        FileIO.write_results(results)
    except OSError as exc:
        print(f"Erro ao gravar saída '{output_path}': {exc}", file=sys.stderr)
        return _EXIT_OUTPUT_ERROR

    return _EXIT_OK


if __name__ == "__main__":
    sys.exit(main(sys.argv))