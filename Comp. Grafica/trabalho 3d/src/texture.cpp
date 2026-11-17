#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct
{
    unsigned short type; // "BM"
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offBits;
} BMPHeader;

typedef struct
{
    unsigned int size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitCount;
    unsigned int compression;
    unsigned int sizeImage;
    int xPelsPerMeter;
    int yPelsPerMeter;
    unsigned int clrUsed;
    unsigned int clrImportant;
} BMPInfoHeader;
#pragma pack(pop)

GLuint loadBMP(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    //printf("loadBMP: abrindo %s\n", filename);
    if (!file)
    {
        fprintf(stderr, "loadBMP: falha ao abrir %s\n", filename);
        return 0;
    }

    BMPHeader header;
    fread(&header, sizeof(BMPHeader), 1, file);
    if (header.type != 0x4D42)
    { // "BM"
        fprintf(stderr, "loadBMP: %s nao e BMP\n", filename);
        fclose(file);
        return 0;
    }

    BMPInfoHeader info;
    fread(&info, sizeof(BMPInfoHeader), 1, file);

    if (info.bitCount != 24)
    {
        fprintf(stderr, "loadBMP: %s nao e 24 bits (bitCount=%d)\n",
                filename, info.bitCount);
        fclose(file);
        return 0;
    }

    // BMP alinha cada linha em múltiplos de 4 bytes
    int width = info.width;
    int height = info.height;
    int rowSize = (width * 3 + 3) & ~3;
    int dataSize = rowSize * height;

    unsigned char *data = (unsigned char *)malloc(dataSize);
    fseek(file, header.offBits, SEEK_SET);
    fread(data, 1, dataSize, file);
    fclose(file);

    // Converte BGR -> RGB e inverte vertical (BMP é bottom-up)
    unsigned char *rgb = (unsigned char *)malloc(width * height * 3);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int srcIdx = y * rowSize + x * 3;
            int dstIdx = ((height - 1 - y) * width + x) * 3;
            rgb[dstIdx + 0] = data[srcIdx + 2]; // R
            rgb[dstIdx + 1] = data[srcIdx + 1]; // G
            rgb[dstIdx + 2] = data[srcIdx + 0]; // B
        }
    }
    free(data);

    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, rgb);

    free(rgb);
    printf("loadBMP: OK %s -> texId=%u (%dx%d)\n", filename, texId, width, height);
    return texId;
}

void texture_enable()
{
    glEnable(GL_TEXTURE_2D);
    // Modula: cor do material * cor da textura (útil para iluminar a textura)
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void texture_disable()
{
    glDisable(GL_TEXTURE_2D);
}