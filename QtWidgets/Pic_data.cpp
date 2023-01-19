#include "Pic_data.h"
#include <png.h>
#include <QImage>
#include <iostream>
#define PNG_BYTES_TO_CHECK 4 
Pic_data::Pic_data(string path)
{
	LoadPNGImage(path.c_str());

}
int Pic_data::LoadPNGImage(const char* file_name)
{
	FILE* fp;
	png_structp png_ptr;
	png_infop info_ptr;
	char buf[PNG_BYTES_TO_CHECK];
	int  x, y, temp, pos;

	fp = fopen(file_name, "rb");
	if (fp == NULL) {
		cout << "[load_png_file] File " << file_name << " could not be opened for reading" << endl;
		return -1;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);

	setjmp(png_jmpbuf(png_ptr));
	temp = fread(buf, 1, PNG_BYTES_TO_CHECK, fp);
	if (temp < PNG_BYTES_TO_CHECK) {
		cout << file_name << ": [load_png_file] The data read does not have PNG_BYTES_TO_CHECK bytes" << endl;
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return -1;
	}
	temp = png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
	if (temp != 0) {
		cout << file_name << ": [load_png_file] The data is not a PNG signature" << endl;
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return -1;
	}

	rewind(fp);
	png_init_io(png_ptr, fp);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	m_colortype = png_get_color_type(png_ptr, info_ptr);
	m_width = png_get_image_width(png_ptr, info_ptr);
	m_height = png_get_image_height(png_ptr, info_ptr);
	m_row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	m_pixeldata = new unsigned char[m_row_bytes * m_height];
	//graph->SetBitDepth(png_get_bit_depth(png_ptr, info_ptr));
	m_row_pointers = png_get_rows(png_ptr, info_ptr);

	//Pic_data::m_width = png_get_image_width(png, infop);
	//Pic_data::m_height = png_get_image_height(png, infop);
	//Pic_data::m_colortype = png_get_color_type(png, infop);
	//Pic_data::m_row_bytes = png_get_rowbytes(png, infop);
	//Pic_data::m_pixeldata = new unsigned char[Pic_data::m_row_bytes * Pic_data::m_height];
	//Pic_data::m_rgba = png_get_rows(png, infop);


	switch (m_colortype) {
	case PNG_COLOR_TYPE_RGB_ALPHA:
		pos = 0;
		m_rgba = new unsigned char* [4];
		m_alpha_flag = true;
		for (int i = 0; i < 4; i++)
		{
			m_rgba[i] = new unsigned char[m_width * m_height];
		}
		for (y = 0; y < m_height; ++y) {
			for (x = 0; x < m_width * 4; ) {
				m_rgba[0][pos] = m_row_pointers[y][x++]; // red   
				m_rgba[1][pos] = m_row_pointers[y][x++]; // green   
				m_rgba[2][pos] = m_row_pointers[y][x++]; // blue   
				m_rgba[3][pos] = m_row_pointers[y][x++]; // alpha   
				++pos;
			}
		}
		break;

	case PNG_COLOR_TYPE_RGB:
		pos = 0;
		m_rgba = new unsigned char* [3];
		m_alpha_flag = false;
		for (int i = 0; i < 3; i++)
		{
			m_rgba[i] = new unsigned char[m_height * m_width];
		}
		for (y = 0; y < m_height; ++y) {
			for (x = 0; x < m_width * 3; ) {
				m_rgba[0][pos] = m_row_pointers[y][x++]; // red   
				m_rgba[1][pos] = m_row_pointers[y][x++]; // green   
				m_rgba[2][pos] = m_row_pointers[y][x++]; // blue   
				++pos;
			}
		}
		break;
	default:
		cout << file_name << ": [load_png_file] Illegal color type" << endl;
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return -1;
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	return 0;
}

int Pic_data::Getwidth(png_structp png, png_infop infop)
{
	Pic_data::m_width = png_get_image_width(png, infop);
	return Pic_data::m_width;
}

int Pic_data::Getheight(png_structp png, png_infop infop)
{
	Pic_data::m_height = png_get_image_height(png, infop);
	return Pic_data::m_height;
}

int Pic_data::Getdepth(png_structp png, png_infop infop)
{
	Pic_data::m_bit_depth = png_get_bit_depth(png, infop);
	return Pic_data::m_bit_depth;
}

int Pic_data::Getcolortype(png_structp png, png_infop infop)
{
	Pic_data::m_colortype = png_get_color_type(png, infop);
	return Pic_data::m_colortype;
}

png_bytep* Pic_data::Getrgba(png_structp png, png_infop infop)
{
	Pic_data::m_rgba = png_get_rows(png, infop);
	return Pic_data::m_rgba;
}
