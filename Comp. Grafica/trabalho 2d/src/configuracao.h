#pragma once

struct ConfigArena {
    int largura = 200;
    int altura  = 150;
};

struct ConfigJogador {
    float raioCabeca = 30.0f;
    float velocidade = 100.0f;
    int vida = 3;
};

struct ConfigInimigo {
    float raioCabeca      = 25.0f;
    float tirosPorSegundo = 2.0f;
    float velocidadeTiro  = 400.0f;
};

struct ConfigBarril {
    float altura       = 50.0f;
    float largura      = 120.0f;
    float   freqTiro  = 1.0;       
    int   nParaGanhar  = 20;
    float velocidade   = 100.0f;
    int vida = 3;
};
struct ConfigTiro
{
    int tiroPorSegundo = 4;
    float velocidade = 200;
};

struct Configuracao {
    ConfigArena   arena;
    ConfigJogador jogador;
    ConfigInimigo inimigo;
    ConfigBarril  barril;
    ConfigTiro tiro;
    
};

bool CarregaConfiguracao(const char* caminho, Configuracao& out);