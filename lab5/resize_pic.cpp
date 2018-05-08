#include "iostream"
#include "cstdio"
#include <stdio.h>
#include <cstdint>
 
using namespace std;
 
typedef struct {
    int8_t id[2];            // Завжди дві літери 'B' і 'M'
    int32_t filesize;        // Розмір файла в байтах                                           потрібна
    int16_t reserved[2];     // 0, 0
    int32_t headersize;      // 54L для 24-бітних зображень
    int32_t infoSize;        // 40L для 24-бітних зображень
    int32_t width;           // ширина зображення в пікселях                                    потрібна
    int32_t depth;           // висота зображення в пікселях                                    потрібна
    int16_t biPlanes;        // 1 (для 24-бітних зображень)
    int16_t bits;            // 24 (для 24-бітних зображень)
    int32_t biCompression;   // 0L
    int32_t biSizeImage;     // Можна поставити в 0L для зображень без компрессії (наш варіант)
    int32_t biXPelsPerMeter; // Рекомендована кількість пікселів на метр, можна 0L
    int32_t biYPelsPerMeter; // Те саме, по висоті
    int32_t biClrUsed;       // Для індексованих зображень, можна поставити 0L
    int32_t biClrImportant;  // Те саме
} BMPHEAD;
BMPHEAD bh, nh;               //Заголовки вхідного та вихідного файлів
typedef struct {
    int8_t redComponent;
    int8_t greenComponent;
    int8_t blueComponent;   //BGR
} PIXELDATA;
PIXELDATA px, helper;
 
int main() {
    FILE *f = fopen("in.bmp", "rb");
    FILE *newf = fopen("output.bmp", "wb");
    fread(&bh.id, sizeof(int8_t), 2, f);
    fread(&bh.filesize, sizeof(int32_t), 1, f);
    fread(&bh.reserved, sizeof(int16_t), 2, f);
    fread(&bh.headersize, sizeof(int32_t), 1, f);
    fread(&bh.infoSize, sizeof(int32_t), 1, f);
    fread(&bh.width, sizeof(int32_t), 1, f);
    fread(&bh.depth, sizeof(int32_t), 1, f);
    fread(&bh.biPlanes, sizeof(int16_t), 1, f);
    fread(&bh.bits, sizeof(int16_t), 1, f);
    fread(&bh.biCompression, sizeof(int32_t), 1, f);
    fread(&bh.biSizeImage, sizeof(int32_t), 1, f);
    fread(&bh.biXPelsPerMeter, sizeof(int32_t), 1, f);
    fread(&bh.biYPelsPerMeter, sizeof(int32_t), 1, f);
    fread(&bh.biClrUsed, sizeof(int32_t), 1, f);
    fread(&bh.biClrImportant, sizeof(int32_t), 1, f);
    nh = bh;
    int n;
    int Q = ((nh.width * 3) % 4);
    cout << "V skolko raz uvelichit:"; cin >> n;
    nh.width = nh.width*n;
    nh.depth = nh.depth*n;
    int bytesPerLine;
    bytesPerLine = nh.width * 3;
    if (bytesPerLine & 0x0003)
    {
        bytesPerLine |= 0x0003;
        ++bytesPerLine;
    }
    nh.filesize = nh.headersize + (long)bytesPerLine*nh.depth;
    fwrite(&nh.id, sizeof(int8_t), 2, newf);
    fwrite(&nh.filesize, sizeof(int32_t), 1, newf);
    fwrite(&nh.reserved, sizeof(int16_t), 2, newf);
    fwrite(&nh.headersize, sizeof(int32_t), 1, newf);
    fwrite(&nh.infoSize, sizeof(int32_t), 1, newf);
    fwrite(&nh.width, sizeof(int32_t), 1, newf);
    fwrite(&nh.depth, sizeof(int32_t), 1, newf);
    fwrite(&nh.biPlanes, sizeof(int16_t), 1, newf);
    fwrite(&nh.bits, sizeof(int16_t), 1, newf);
    fwrite(&nh.biCompression, sizeof(int32_t), 1, newf);
    fwrite(&nh.biSizeImage, sizeof(int32_t), 1, newf);
    fwrite(&nh.biXPelsPerMeter, sizeof(int32_t), 1, newf);
    fwrite(&nh.biYPelsPerMeter, sizeof(int32_t), 1, newf);
    fwrite(&nh.biClrUsed, sizeof(int32_t), 1, newf);
    fwrite(&nh.biClrImportant, sizeof(int32_t), 1, newf);
    int j = 0;
    while (j < nh.depth)
    {
        int help = 0;
        while (help<nh.width) {
            fread(&px, sizeof(PIXELDATA), 1, f);
            for (int i = 0; i < n; i++)
            {
                fwrite(&px, sizeof(PIXELDATA), 1, newf);
                help++;
            }
        }
       if (((nh.width * 3) % 4) != 0)
        
         
           for (int s = 0;s < (4 - Q);s++)
           {
           fputc(0x00, newf);
           }
        
        for (int m = 1;m < n;m++)
        {
            fseek(f, 54 + bh.width * j, SEEK_SET);
            int l = 0;
            while (l<nh.width)
            {
                fread(&px, sizeof(PIXELDATA), 1, f);
                for (int i = 0; i < n; i++)
                {
                    fwrite(&px, sizeof(PIXELDATA), 1, newf);
                    l++;
                }
            }
            if ((nh.width * 3) % 4)  Q = 4 - (nh.width * 3) % 4;
            for (size_t t = 0; t < Q; ++t) {
                fread(&px, sizeof(px), 1, f);
                fwrite(&px, sizeof(px), 1, newf);
        
            }
        }

        j++;
    }
    return 0;
}