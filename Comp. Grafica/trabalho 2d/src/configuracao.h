#pragma once

struct ConfigArena {
    int largura = 800;
    int altura  = 600;
};

struct ConfigJogador {
    float raioCabeca = 30.0f;
    float velocidade = 200.0f;
};

struct ConfigInimigo {
    float raioCabeca      = 25.0f;
    float tirosPorSegundo = 2.0f;
    float velocidadeTiro  = 400.0f;
};

struct ConfigBarril {
    float altura       = 40.0f;
    float largura      = 30.0f;
    int   numeroTiros  = 3;       
    int   nParaGanhar  = 10;
    float velocidade   = 60.0f;
};

struct Configuracao {
    ConfigArena   arena;
    ConfigJogador jogador;
    ConfigInimigo inimigo;
    ConfigBarril  barril;
};

bool CarregaConfiguracao(const char* caminho, Configuracao& out);