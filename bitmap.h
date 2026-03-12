#include <stdio.h>
#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}pixel24_t;

int bitmapTobmp(const pixel24_t image[], const uint32_t imageWidth, const uint32_t imageHeight, const char fileName[])
{
    #pragma pack(push, 1)

    struct
    {
        uint8_t fileSignature[2];
        uint32_t fileSize;
        uint32_t reserved;
        uint32_t bitmapOffset;
    } 
    fileHeader = {{'B', 'M'}, (imageWidth * imageHeight * 3) + 54, 0, 54};

    struct
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
    
    FILE *file = fopen(fileName, "w+");

    bitmapInfoHeader.sizeofInfoHeader = 40;
    bitmapInfoHeader.bitmapWidth = imageWidth;
    bitmapInfoHeader.bitmapHeight = imageHeight;
    bitmapInfoHeader.colorPlanes = 1;
    bitmapInfoHeader.bitsPerPixel = 24;
    bitmapInfoHeader.compression = 0;
    bitmapInfoHeader.sizeofBitmap = imageHeight * imageWidth * 3;
    bitmapInfoHeader.horizontalDPI = 2835;
    bitmapInfoHeader.verticalDPI = 2835;
    bitmapInfoHeader.colorPalett = 0;
    bitmapInfoHeader.importantColors = 0;

    fwrite(&fileHeader, sizeof(fileHeader), 1, file);
    fwrite(&bitmapInfoHeader, sizeof(bitmapInfoHeader), 1, file);
    fwrite(image, imageHeight * imageWidth * 3, 1, file);
    fclose(file);

    return sizeof(fileHeader);
}