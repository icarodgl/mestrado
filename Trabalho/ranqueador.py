
import re
import unicodedata
from pathlib import Path
from urllib.parse import urlsplit, urlunsplit
import pandas as pd


# ==================================================
# CONFIGURAÇÕES
# ==================================================

ARQUIVO_ENTRADA = Path("artigos.csv")
ARQUIVO_SAIDA = Path("artigos_ranqueados.csv")

COLUNA_TITULO = "titulo"
COLUNA_RESUMO = "resumo"
COLUNA_LINK = "link"  # Opcional


lista_desejados = [
    # TDAH e adultos
    "ADHD",
    "attention deficit hyperactivity disorder",
    "adult ADHD",
    "adults with ADHD",
    "TDAH",
    "transtorno do déficit de atenção com hiperatividade",
    "adultos com TDAH",
    "trastorno por déficit de atención e hiperactividad",
    "adultos con TDAH",

    # Realidade virtual
    "virtual reality",
    "immersive virtual reality",
    "virtual environment",
    "realidade virtual",
    "ambiente virtual",
    "realidad virtual",
    "entorno virtual",

    # Jogos sérios
    "serious game",
    "serious games",
    "game-based assessment",
    "jogo sério",
    "jogos sérios",
    "avaliação baseada em jogos",
    "juego serio",
    "juegos serios",
    "evaluación basada en juegos",

    # Identificação e avaliação
    "ADHD detection",
    "ADHD diagnosis",
    "screening",
    "cognitive assessment",
    "executive function",
    "identificação do TDAH",
    "diagnóstico de TDAH",
    "avaliação cognitiva",
    "detecção del TDAH",
    "diagnóstico del TDAH",
    "evaluación cognitiva",
]

lista_rejeitados = [
    # Crianças e adolescentes
    "children",
    "childhood",
    "pediatric",
    "preschool",
    "crianças",
    "infância",
    "pediátrico",
    "niños",
    "infancia",
    "preescolar",

    # Tecnologias diferentes
    "augmented reality",
    "realidade aumentada",
    "realidad aumentada",

    # Estudos focados em tratamento
    "treatment",
    "rehabilitation",
    "tratamento",
    "reabilitação",
    "tratamiento",
    "rehabilitación",
]


lista_prioritaria = [
    "virtual reality",
    "adult",
    "TDAH",
    "ADHD",
]

# Pesos do ranqueamento
PESO_TEMA = 10
PESO_TITULO = 5
PESO_REJEITADO = 1000
PESO_PRIORITARIO = 3

# Penalidade para resumos muito curtos
TAMANHO_MINIMO_RESUMO = 0
PENALIDADE_RESUMO_CURTO = 0





def normalizar_titulo(titulo):
    """Normaliza títulos para identificar duplicatas."""
    texto = str(titulo).strip().lower()
    texto = unicodedata.normalize("NFKD", texto)
    texto = "".join(
        caractere for caractere in texto
        if not unicodedata.combining(caractere)
    )
    texto = re.sub(r"[^\w\s]", " ", texto)
    return re.sub(r"\s+", " ", texto).strip()


def remover_duplicados(df):
    """Remove artigos repetidos por link ou título normalizado."""
    df = df.copy()

    df["_titulo_normalizado"] = (
        df["titulo"].fillna("").map(normalizar_titulo)
    )

    duplicados = (
        (df["_titulo_normalizado"] != "")
        & df["_titulo_normalizado"].duplicated(keep="first")
    )

    quantidade_removida = int(duplicados.sum())

    df = df.loc[~duplicados].copy()
    df = df.drop(
        columns=["_titulo_normalizado"],
        errors="ignore"
    )

    print(f"Artigos duplicados removidos: {quantidade_removida}")
    print(f"Artigos únicos restantes: {len(df)}")

    return df

# ==================================================
# NORMALIZAÇÃO E CONTAGEM
# ==================================================

def normalizar(texto):
    """Converte para minúsculas e remove acentos."""
    texto = str(texto).lower()
    texto = unicodedata.normalize("NFKD", texto)

    return "".join(
        c for c in texto
        if not unicodedata.combining(c)
    )


def contar_termo(texto, termo):
    """Conta ocorrências completas de um termo."""
    texto = normalizar(texto)
    termo = normalizar(termo)

    padrao = (
        r"(?<!\w)"
        + re.escape(termo).replace(r"\ ", r"\s+")
        + r"(?!\w)"
    )

    return len(re.findall(padrao, texto))


def analisar_termos(texto, termos):
    detalhes = {
        termo: contar_termo(texto, termo)
        for termo in termos
    }

    encontrados = [
        termo
        for termo, quantidade in detalhes.items()
        if quantidade > 0
    ]

    return sum(detalhes.values()), encontrados, detalhes


# ==================================================
# RANQUEAMENTO
# ==================================================

def avaliar_artigo(titulo, resumo, link=""):
    texto = f"{titulo} {resumo}"

    (
        total_desejados,
        encontrados_desejados,
        detalhes_desejados,
    ) = analisar_termos(texto, lista_desejados)

    (
        total_rejeitados,
        encontrados_rejeitados,
        detalhes_rejeitados,
    ) = analisar_termos(texto, lista_rejeitados)

    (
        total_prioritarios,
        encontrados_prioritarios,
        _,
    ) = analisar_termos(texto, lista_prioritaria)

    (
        desejados_titulo,
        termos_titulo,
        _,
    ) = analisar_termos(titulo, lista_desejados)

    # Percentual de termos desejados distintos encontrados
    cobertura = (
        100 * len(encontrados_desejados) / len(lista_desejados)
        if lista_desejados
        else 0
    )

    # Cálculo da pontuação
    pontuacao = (
        total_desejados * PESO_TEMA
        + desejados_titulo * PESO_TITULO
        + total_prioritarios * PESO_PRIORITARIO
        - total_rejeitados * PESO_REJEITADO
    )

    if len(resumo.split()) < TAMANHO_MINIMO_RESUMO:
        pontuacao -= PENALIDADE_RESUMO_CURTO

    # Justificativa da classificação
    motivos = []

    if encontrados_desejados:
        motivos.append(
            "Temas desejados: "
            + ", ".join(encontrados_desejados)
        )
    else:
        motivos.append("Nenhum tema desejado encontrado")

    if termos_titulo:
        motivos.append(
            "Tema no título: " + ", ".join(termos_titulo)
        )

    if encontrados_rejeitados:
        motivos.append(
            "Termos rejeitados: "
            + ", ".join(encontrados_rejeitados)
        )

    if encontrados_prioritarios:
        motivos.append(
            "Temas prioritários: "
            + ", ".join(encontrados_prioritarios)
        )

    if len(resumo.split()) < TAMANHO_MINIMO_RESUMO:
        motivos.append("Resumo curto; relevância incerta")

    return {
        "titulo": titulo,
        "link": link,
        "palavras_resumo": len(resumo.split()),
        "ocorrencias_desejadas": total_desejados,
        "termos_desejados_encontrados": ", ".join(
            encontrados_desejados
        ),
        "detalhes_desejados": "; ".join(
            f"{termo}: {qtd}"
            for termo, qtd in detalhes_desejados.items()
        ),
        "ocorrencias_rejeitadas": total_rejeitados,
        "termos_rejeitados_encontrados": ", ".join(
            encontrados_rejeitados
        ),
        "detalhes_rejeitados": "; ".join(
            f"{termo}: {qtd}"
            for termo, qtd in detalhes_rejeitados.items()
        ),
        "ocorrencias_prioritarias": total_prioritarios,
        "termos_prioritarios": ", ".join(
            encontrados_prioritarios
        ),
        "desejados_no_titulo": desejados_titulo,
        "percentual_cobertura": round(cobertura, 1),
        "pontuacao": pontuacao,
        "justificativa": " | ".join(motivos),
        "status": "OK",
    }


# ==================================================
# EXECUÇÃO
# ==================================================

def main():
    if not ARQUIVO_ENTRADA.exists():
        raise FileNotFoundError(
            f"Arquivo não encontrado: {ARQUIVO_ENTRADA}"
        )

    # O arquivo de entrada pode ser separado por vírgulas
    # ou ponto e vírgula.
    df = pd.read_csv(
        ARQUIVO_ENTRADA,
        sep=None,
        engine="python",
        encoding="utf-8-sig",
    )

    # Remove espaços extras dos nomes das colunas
    df.columns = df.columns.str.strip()

    colunas_obrigatorias = [
        COLUNA_TITULO,
        COLUNA_RESUMO,
    ]

    faltantes = [
        coluna
        for coluna in colunas_obrigatorias
        if coluna not in df.columns
    ]

    if faltantes:
        raise ValueError(
            f"Colunas obrigatórias ausentes: {faltantes}"
        )

    # Link é opcional
    if COLUNA_LINK not in df.columns:
        df[COLUNA_LINK] = ""

    df = remover_duplicados(df)
    df = df.fillna("")
    resultados = []

    for indice, linha in df.iterrows():
        titulo = str(linha[COLUNA_TITULO]).strip()
        resumo = str(linha[COLUNA_RESUMO]).strip()
        link = str(linha[COLUNA_LINK]).strip()

        if not titulo and not resumo:
            print(
                f"Linha {indice + 2}: registro vazio, ignorado."
            )
            continue

        print(
            f"Analisando artigo {indice + 1}: {titulo[:70]}"
        )

        try:
            resultado = avaliar_artigo(titulo, resumo, link)

        except Exception as erro:
            resultado = {
                "titulo": titulo,
                "link": link,
                "pontuacao": -999999,
                "ocorrencias_rejeitadas": 0,
                "ocorrencias_desejadas": 0,
                "status": f"ERRO: {erro}",
                "justificativa": "Falha na análise",
            }

        resultados.append(resultado)

    if not resultados:
        print("Nenhum artigo válido encontrado.")
        return

    saida = pd.DataFrame(resultados)

    saida["erro"] = ~saida["status"].eq("OK")

    # Primeiro, registros válidos; depois, maior pontuação.
    # Em caso de empate, menos termos rejeitados.
    saida = saida.sort_values(
        by=[
            "erro",
            "pontuacao",
            "ocorrencias_rejeitadas",
            "ocorrencias_desejadas",
        ],
        ascending=[True, False, True, False],
    )

    saida = saida.drop(columns=["erro"])
    saida.insert(0, "ranking", range(1, len(saida) + 1))

    saida.to_csv(
        ARQUIVO_SAIDA,
        index=False,
        sep=";",
        encoding="utf-8-sig",
    )

    print("\nProcessamento concluído!")
    print(f"Artigos analisados: {len(resultados)}")
    print(f"CSV gerado: {ARQUIVO_SAIDA.resolve()}")

if __name__ == "__main__":
    main()

