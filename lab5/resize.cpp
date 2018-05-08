#include <iostream>
#include <fstream>

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
}BMPHEAD;
BMPHEAD bh, nh;               //Заголовки вхідного та вихідного файлів
typedef struct {
  int8_t redComponent;
  int8_t greenComponent;
  int8_t blueComponent;   //BGR
} PIXELDATA;
PIXELDATA px, helper;

int main(){
  BMPHEAD bmp;
  ifstream fin("in.bmp",std::ios::binary);
  ofstream fout("out.bmp");
  char ch = 'a';
  fin.read(reinterpret_cast<char*>(&bmp),sizeof(BMPHEAD));
  fout.write(reinterpret_cast<char*>(&bmp),sizeof(BMPHEAD));
  int padding = (4 - (bmp.width * sizeof(PIXELDATA)) % 4 );
  for(int i = 0; i < bmp.depth; i++){
     for(int j = 0; j < bmp.width; j++){
         fin.read(reinterpret_cast<char*>(&bmp),sizeof(PIXELDATA));
         fout.write(reinterpret_cast<char*>(&bmp),sizeof(PIXELDATA));
     } 
     fin.seekg(padding, SEEK_CUR);
     for(int k = 0; k < padding; k++){
         fout.put(0x00);
     }
    
  }
   fin.close();
   fout.close();
   return 0;
}