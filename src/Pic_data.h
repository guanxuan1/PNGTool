#pragma once
#include <string>
#include <png.h>
using namespace std;
class Pic_data {
public:
    int m_width, m_height;
    int m_bit_depth;
    bool m_alpha_flag;
    int m_colortype;
    unsigned char* m_pixeldata;
    unsigned int m_row_bytes;
    png_bytep* m_row_pointers;
    png_bytep* m_rgba;

public:
    Pic_data(string path);
    int Getwidth(png_structp, png_infop);
    int Getheight(png_structp, png_infop);
    int Getdepth(png_structp, png_infop);
    int Getcolortype(png_structp, png_infop);
    png_bytep* Getrgba(png_structp, png_infop);
    int LoadPNGImage(const char* file_name);

};

