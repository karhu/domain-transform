#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"
#include "Exception.h"
#include "Mat2.h"
#include "io_png/io_png.h"

#include <assert.h>
#include <stdlib.h>
#include <iostream>
inline void c_free(void* ptr)
{
    free(ptr);
}

typedef unsigned char uchar;
typedef unsigned char uint8;

using namespace std;

Mat2<float3> LoadPNG(const std::string& path)
{
    size_t nx, ny, nc;
    float* img;
    float *img_r, *img_g, *img_b;

    // read the image info a float array
    img = io_png_read_flt_opt(path.c_str(), &nx, &ny, &nc,IO_PNG_OPT_RGB);

    assert(nc == 3); // Number of color components should always be 3.

    uint width = nx;
    uint height = ny;
    Mat2<float3> mat(width,height);

    // copy data from RRRGGGBBB TO RGBRGBRGB format.
    img_r = img;                // red channel
    img_g = img + nx * ny;      // green channel
    img_b = img + 2 * nx * ny;  // blue channel

    float3* d = mat.data;
    for (ulong i=0; i<nx*ny; i++)
    {
        d[i].r = img_r[i]*255.0;
        d[i].g = img_g[i]*255.0;
        d[i].b = img_b[i]*255.0;
    }

    c_free(img);

    return mat;
}

void SavePNG(const std::string& path, Mat2<float3> mat)
{
    if (mat.width == 0 || mat.height == 0)
        throw Exception("No image data to write to file.");

    float* tmp = new float[mat.width*mat.height*3];
    float *tmp_r, *tmp_g, *tmp_b;

    tmp_r = tmp;                               // red channel
    tmp_g = tmp + mat.width * mat.height;      // green channel
    tmp_b = tmp + 2 * mat.width * mat.height;  // blue channel

    for (ulong i=0; i<mat.width*mat.height; i++)
    {
        tmp_r[i] = mat.data[i].r/255.0;
        tmp_g[i] = mat.data[i].g/255.0;
        tmp_b[i] = mat.data[i].b/255.0;
    }

    io_png_write_flt(path.c_str(), tmp, mat.width, mat.height, 3);
//    cout << "file written: " << path << endl;

    delete[] tmp;
}



#endif // IMAGE_H
