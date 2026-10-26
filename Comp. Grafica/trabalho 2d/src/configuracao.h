#pragma once

struct ConfigArena {
    int largura = 200;
    int altura  = 200;
};

struct ConfigJogador {
    float raioCabeca = 30.0f;
    float velocidade = 200.0f;
    int vida = 3;
};

struct ConfigInimigo {
    float raioCabeca      = 25.0f;
    float tirosPorSegundo = 2.0f;
    float velocidadeTiro  = 400.0f;
};

struct ConfigBarril {
    float altura       = 40.0f;
    float largura      = 30.0f;
    float   freqTiro  = 1.0;       
    int   nParaGanhar  = 10;
    float velocidade   = 60.0f;
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