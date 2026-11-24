#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "objloader.h"

using namespace std;

//função para carregar o OBJ
bool mesh::loadMesh(string path){
    vector<int> vertIndex, uvIndex, normIndex;//indexadores para vertices, mapeamento de textura e normais
    vertsPos.clear();
    vertsNorm.clear();
    vertsTex.clear();
    vertsS.clear();
    int i =0;
    FILE* file= fopen(path.data(), "r");
    if(file== NULL){
        cout<< "falha ao carregar o arquivo"<< endl;
        return false;
    }
    else{ 
        while(1){
            char lineHeader[128];
            int res= fscanf(file, "%s", lineHeader);
            if(res==EOF){
                    break;
            }
            if(strcmp(lineHeader, "v") == 0){
                pos vert;
                fscanf(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
                vertsPos.push_back(vert);
            }
            else if(strcmp(lineHeader,"vt")==0){
                tex vert;
                fscanf(file, "%f %f\n", &vert.u, &vert.v);
                vertsTex.push_back(vert);
            }
            else if(strcmp(lineHeader, "vn")==0){
                norm vert;
                fscanf(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
                vertsNorm.push_back(vert);
            }
            else if(strcmp(lineHeader, "f")==0){
                string v1, v2, v3;
                unsigned int vertInd[3], uvInd[3], normInd[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertInd[0], &uvInd[0], &normInd[0], &vertInd[1], &uvInd[1], &normInd[1], &vertInd[2], &uvInd[2], &normInd[2]);
                if(matches !=9){
                        printf("QUantidade de valores lidos diferente doesperado. Falha ao ler uma linha de face");
                        return false;			
                }
                //Cria uma lista com os índices na ordem apropriada para o desenho das faces
                //Esta é a lista de índices de vértices
                vertIndex.push_back(vertInd[0]);
                vertIndex.push_back(vertInd[1]);
                vertIndex.push_back(vertInd[2]);
                //Esta é a lista de índices de mapeamento de textura
                uvIndex.push_back(uvInd[0]);
                uvIndex.push_back(uvInd[1]);
                uvIndex.push_back(uvInd[2]);
                // Esta é a lista de índices de normais
                normIndex.push_back(normInd[0]);
                normIndex.push_back(normInd[1]);
                normIndex.push_back(normInd[2]);
            }
        }
        for(unsigned int i=0; i<vertIndex.size(); i++){
            verticeStrip vert;
            vert.vPos.x = vertsPos[vertIndex[i]-1].x;
            vert.vPos.y = vertsPos[vertIndex[i]-1].y;
            vert.vPos.z = vertsPos[vertIndex[i]-1].z;
            vert.vTex.u = vertsTex[uvIndex[i]-1].u;
            vert.vTex.v = vertsTex[uvIndex[i]-1].v;
            vert.vNorm.x = vertsNorm[normIndex[i]-1].x;
            vert.vNorm.y = vertsNorm[normIndex[i]-1].y;
            vert.vNorm.z = vertsNorm[normIndex[i]-1].z;
            vertsS.push_back(vert);
        }
    }
    return true;
}

void mesh::draw(bool comTextura) const
{
    glColor3f(1.0f, 1.0f, 1.0f);
    

    if (comTextura) {
        // Material neutro: textura aparece pura
        GLfloat branco[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat emissao[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissao);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  branco);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  branco);
    } else {
        GLfloat emissao[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat amb[]     = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat dif[]     = { 0.8f, 0.7f, 0.6f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissao);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  dif);
    }

    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i + 2 < vertsS.size(); i += 3) {
        for (int j = 0; j < 3; ++j) {

            // inverter textura
            float v = inverterV ? (1.0f - vertsS[i+j].vTex.v) : vertsS[i+j].vTex.v;
            glTexCoord2f(vertsS[i+j].vTex.u, v);
            // glTexCoord2f(vertsS[i+j].vTex.u, vertsS[i+j].vTex.v);

            glNormal3f(vertsS[i+j].vNorm.x, vertsS[i+j].vNorm.y, vertsS[i+j].vNorm.z);
            glVertex3f(vertsS[i+j].vPos.x,  vertsS[i+j].vPos.y,  vertsS[i+j].vPos.z);
        }
    }
    glEnd();
}