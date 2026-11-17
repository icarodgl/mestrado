#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

// Carrega um BMP 24 bits (sem canal alfa) e retorna o ID da textura OpenGL.
// Retorna 0 em caso de falha.
GLuint loadBMP(const char* filename);

// Habilita textura 2D com filtros padrão (linear, repetição).
void texture_enable();

// Desabilita textura 2D e volta ao estado "cor sólida".
void texture_disable();

#endif