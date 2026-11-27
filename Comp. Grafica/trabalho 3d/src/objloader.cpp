#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "objloader.h"
#include "texture.h" 

using namespace std;

// função para carregar o OBJ
bool mesh::loadMesh(string path)
{
    vector<int> vertIndex, uvIndex, normIndex; // indexadores para vertices, mapeamento de textura e normais
    vertsPos.clear();
    vertsNorm.clear();
    vertsTex.clear();
    vertsS.clear();
    int i = 0;
    FILE *file = fopen(path.data(), "r");
    if (file == NULL)
    {
        cout << "falha ao carregar o arquivo" << endl;
        return false;
    }
    else
    {
        while (1)
        {
            char lineHeader[128];
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
            {
                break;
            }
            if (strcmp(lineHeader, "v") == 0)
            {
                pos vert;
                fscanf(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
                vertsPos.push_back(vert);
            }
            else if (strcmp(lineHeader, "vt") == 0)
            {
                tex vert;
                fscanf(file, "%f %f\n", &vert.u, &vert.v);
                vertsTex.push_back(vert);
            }
            else if (strcmp(lineHeader, "vn") == 0)
            {
                norm vert;
                fscanf(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
                vertsNorm.push_back(vert);
            }
else if (strcmp(lineHeader, "f") == 0)
{
    // Lê a linha inteira (até \n) e parseia manualmente
    char linhaFace[1024];
    fgets(linhaFace, sizeof(linhaFace), file);

    // Extrai até 8 tokens (vértices) da face
    // Suporta triângulos E quads E ngons (triangula em leque)
    std::vector<int> vIdx, vtIdx, vnIdx;
    char* token = strtok(linhaFace, " \t\r\n");
    while (token) {
        int v = 0, vt = 0, vn = 0;
        int n = sscanf(token, "%d/%d/%d", &v, &vt, &vn);
        if (n == 0) break;            // linha inválida
        vIdx.push_back(v);
        vtIdx.push_back(vt);           // 0 se não havia
        vnIdx.push_back(vn);           // 0 se não havia
        token = strtok(NULL, " \t\r\n");
    }

    if (vIdx.size() < 3) {
        fprintf(stderr, "Face com menos de 3 vertices em %s\n", path.c_str());
        return false;
    }

    // Triangula em leque: (0, i, i+1)
    for (size_t i = 1; i + 1 < vIdx.size(); ++i)
    {
        vertIndex.push_back(vIdx[0]);  vertIndex.push_back(vIdx[i]);  vertIndex.push_back(vIdx[i+1]);
        uvIndex.push_back(vtIdx[0]);   uvIndex.push_back(vtIdx[i]);   uvIndex.push_back(vtIdx[i+1]);
        normIndex.push_back(vnIdx[0]); normIndex.push_back(vnIdx[i]); normIndex.push_back(vnIdx[i+1]);
    }
}
        }
for (unsigned int i = 0; i < vertIndex.size(); i++)
{
    verticeStrip vert;
    vert.vPos.x = vertsPos[vertIndex[i]-1].x;
    vert.vPos.y = vertsPos[vertIndex[i]-1].y;
    vert.vPos.z = vertsPos[vertIndex[i]-1].z;

    // UV: se não existir, usa (0,0)
    if (uvIndex[i] > 0 && (size_t)(uvIndex[i]-1) < vertsTex.size()) {
        vert.vTex.u = vertsTex[uvIndex[i]-1].u;
        vert.vTex.v = vertsTex[uvIndex[i]-1].v;
    } else {
        vert.vTex.u = 0.0f;
        vert.vTex.v = 0.0f;
    }

    // Normal: se não existir, usa (0,0,1) como fallback
    if (normIndex[i] > 0 && (size_t)(normIndex[i]-1) < vertsNorm.size()) {
        vert.vNorm.x = vertsNorm[normIndex[i]-1].x;
        vert.vNorm.y = vertsNorm[normIndex[i]-1].y;
        vert.vNorm.z = vertsNorm[normIndex[i]-1].z;
    } else {
        vert.vNorm.x = 0.0f;
        vert.vNorm.y = 0.0f;
        vert.vNorm.z = 1.0f;
    }

    vertsS.push_back(vert);
}
    }
    return true;
}

void mesh::draw(GLfloat textura) const
{
    if (textura == 0)
    {
        GLfloat emissao[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat dif[] = {0.5f, 0.5f, 0.5f, 5.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissao);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);

        glBegin(GL_TRIANGLES);
        for (unsigned int i = 0; i + 2 < vertsS.size(); i += 3)
        {
            for (int j = 0; j < 3; ++j)
            {
                glNormal3f(vertsS[i + j].vNorm.x, vertsS[i + j].vNorm.y, vertsS[i + j].vNorm.z);
                glVertex3f(vertsS[i + j].vPos.x, vertsS[i + j].vPos.y, vertsS[i + j].vPos.z);
            }
        }
        glEnd();
    }
    else
    {
        GLfloat emissao[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat dif[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissao);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
        texture_enable();

        glBindTexture(GL_TEXTURE_2D, textura);
        glBegin(GL_TRIANGLES);
        for (unsigned int i = 0; i + 2 < vertsS.size(); i += 3)
        {
            for (int j = 0; j < 3; ++j)
            {
                // inverter textura
                float v = inverterV ? (1.0f - vertsS[i + j].vTex.v) : vertsS[i + j].vTex.v;
                glTexCoord2f(vertsS[i + j].vTex.u, v);
                // glTexCoord2f(vertsS[i+j].vTex.u, vertsS[i+j].vTex.v);

                glNormal3f(vertsS[i + j].vNorm.x, vertsS[i + j].vNorm.y, vertsS[i + j].vNorm.z);
                glVertex3f(vertsS[i + j].vPos.x, vertsS[i + j].vPos.y, vertsS[i + j].vPos.z);
            }
        }
        glEnd();

        texture_disable();
    }
}