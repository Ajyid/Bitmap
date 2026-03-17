#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DPI72 2835
#define DPI96 3780
#define DPI150 5906
#define DPI200 7874
#define DPI300 11811
#define DPI600 23622

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}
pixel24_t;

#pragma pack(push, 1)

typedef struct
{
    uint8_t fileSignature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t bitmapOffset;
} 
bitmapFileheader;

typedef struct
{
    uint32_t sizeofInfoHeader;
    uint32_t bitmapWidth;
    uint32_t bitmapHeight;
    uint16_t colorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t sizeofBitmap;
    uint32_t horizontalDPI;
    uint32_t verticalDPI;
    uint32_t colorPalett;
    uint32_t importantColors;
}
bitmapInfoHeader;

#pragma pack(pop)

int writeBitmap(const char filename[], const void* image, const uint32_t imageWidth, const uint32_t imageHeight, const uint16_t bitsPerPixel, uint32_t horizontalDPI, uint32_t verticalDPI);

int writeBitmap24(const char fileName[], const pixel24_t image[], const uint32_t imageWidth, const uint32_t imageHeight)
{
    return writeBitmap(fileName, image, imageWidth, imageHeight, 24, DPI96, DPI96);
}

int writeBitmap(const char filename[], const void* image, const uint32_t imageWidth, const uint32_t imageHeight, const uint16_t bitsPerPixel, uint32_t horizontalDPI, uint32_t verticalDPI)
{
    bitmapFileheader fileHeader;
    bitmapInfoHeader infoHeader;
    FILE *file = fopen(filename, "wb");
    uint8_t *blackmagic = (uint8_t*) image;

    if(file == NULL)
    {
        return EXIT_FAILURE;
    }

    memcpy(fileHeader.fileSignature, "BM", 2);
    fileHeader.fileSize = (((imageWidth * (bitsPerPixel / 8) + 3) & ~3) * imageHeight) + 54;
    fileHeader.reserved = 0;
    fileHeader.bitmapOffset = 54;

    infoHeader.sizeofInfoHeader = 40;
    infoHeader.bitmapWidth = imageWidth;
    infoHeader.bitmapHeight = imageHeight;
    infoHeader.colorPlanes = 0;
    infoHeader.bitsPerPixel = bitsPerPixel;
    infoHeader.compression = 0;
    infoHeader.sizeofBitmap = fileHeader.fileSize - fileHeader.bitmapOffset;
    infoHeader.horizontalDPI = horizontalDPI;
    infoHeader.verticalDPI = verticalDPI;
    infoHeader.colorPalett = 0;
    infoHeader.importantColors = 0;

    fwrite(&fileHeader, sizeof(fileHeader), 1, file);
    fwrite(&infoHeader, sizeof(infoHeader), 1, file);

    for(int i = 0; i < infoHeader.bitmapHeight; i++)
    {
        fwrite(blackmagic + imageWidth * i * (bitsPerPixel / 8), sizeof(pixel24_t), imageWidth, file);
        fwrite("\00\00\00", 1, (4 - ((imageWidth * 3) % 4)) % 4, file);
    }

    fclose(file);

    return EXIT_SUCCESS;
}