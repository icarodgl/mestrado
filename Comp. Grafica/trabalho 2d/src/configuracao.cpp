// configuracao.cpp
#include "configuracao.h"
#include "tinyxml2.h"
#include <cstdio>
#include <cstring>

using namespace tinyxml2;


static void LerFloat(const XMLElement* el, const char* nome, float& out)
{
    if (!el) return;
    el->QueryFloatAttribute(nome, &out);   // ignora o retorno: default persiste
}

static void LerInt(const XMLElement* el, const char* nome, int& out)
{
    if (!el) return;
    el->QueryIntAttribute(nome, &out);
}


bool CarregaConfiguracao(const char* caminho, Configuracao& out)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(caminho);
    if (err != XML_SUCCESS)
    {
        fprintf(stderr,
                "Erro ao abrir '%s': %s (linha %d)\n",
                caminho,
                doc.ErrorStr() ? doc.ErrorStr() : "desconhecido",
                doc.ErrorLineNum());
        return false;
    }

    XMLElement* raiz = doc.RootElement();
    if (!raiz || strcmp(raiz->Name(), "jogo") != 0)
    {
        fprintf(stderr, "Raiz do XML deve ser <jogo>\n");
        return false;
    }

    if (const XMLElement* e = raiz->FirstChildElement("arena")) {
        LerInt(e, "largura", out.arena.largura);
        LerInt(e, "altura",  out.arena.altura);
    }

    if (const XMLElement* e = raiz->FirstChildElement("jogador")) {
        LerFloat(e, "raioCabeca", out.jogador.raioCabeca);
        LerFloat(e, "velocidade", out.jogador.velocidade);
        LerInt(e, "vida", out.jogador.vida);
    }
    if (const XMLElement* e = raiz->FirstChildElement("tiro")) {
        LerInt(e, "tiroPorSegundo", out.tiro.tiroPorSegundo);
        LerFloat(e, "velocidade", out.tiro.velocidade);
    }
    if (const XMLElement* e = raiz->FirstChildElement("inimigo")) {
        LerFloat(e, "raioCabeca",      out.inimigo.raioCabeca);
        LerFloat(e, "tirosPorSegundo", out.inimigo.tirosPorSegundo);
        LerFloat(e, "velocidadeTiro",  out.inimigo.velocidadeTiro);
    }

    if (const XMLElement* e = raiz->FirstChildElement("barril")) {
        LerFloat(e, "altura",      out.barril.altura);
        LerFloat(e, "largura",     out.barril.largura);
        LerFloat  (e, "freqTiro", out.barril.freqTiro);
        LerInt  (e, "nParaGanhar", out.barril.nParaGanhar);
        LerFloat(e, "velocidade",  out.barril.velocidade);
    }
    if (out.jogador.vida <= 0 ) {
        fprintf(stderr, "Vida do jogador inválida\n"); return false;
    }
    if (out.arena.largura <= 0 || out.arena.altura <= 0) {
        fprintf(stderr, "Arena inválida\n"); return false;
    }
    if (out.barril.freqTiro <= 0) {
        fprintf(stderr, "freqTiro deve ser >= 1.0\n"); return false;
    }
    if (out.barril.nParaGanhar <= 0) {
        fprintf(stderr, "nParaGanhar deve ser >= 1\n"); return false;
    }

    return true;
}