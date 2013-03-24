#ifndef IMAGE_H
#define IMAGE_H


#include "Exception.h"
#include "io_png/io_png.h"

#include <assert.h>
#include <stdlib.h>
inline void c_free(void* ptr)
{
    free(ptr);
}

typedef unsigned char uchar;
typedef unsigned char uint8;

using namespace std;

/** A simple RGB image. **/
struct Image
{
    int width, height;
    uchar* data; // size: width*height*3

    /** Loads an image from a file. **/
    Image(const string& path)
    {
        size_t nx, ny, nc;
        uchar* img;
        uchar *img_r, *img_g, *img_b;

        // read the image info a float array
        img = io_png_read_uchar_opt(path.c_str(), &nx, &ny, &nc,IO_PNG_OPT_RGB);

        assert(nc == 3); // Number of color components should always be 3.

        width = nx;
        height = ny;
        data = new uchar[nx*ny*3];

        // copy data from RRRGGGBBB TO RGBRGBRGB format.
        img_r = img;                // red channel
        img_g = img + nx * ny;      // green channel
        img_b = img + 2 * nx * ny;  // blue channel

        for (ulong i=0; i<nx*ny; i++)
        {
            data[3*i] = img_r[i];
            data[3*i+1] = img_g[i];
            data[3*i+2] = img_b[i];
        }

        c_free(img);
    }

    /** Creates an empty 0 initialized image of given size.  **/
    Image(uint w, uint h)
    {
        width = w;
        height = h;
        data = new uchar[w*h*3];
        for (ulong i=0; i<w*h*3;i++)
        {
            data[i] = 0;
        }
    }

    void save(const string& path)
    {
        if (data == 0)
            throw Exception("No image data to write to file.");

        uchar* tmp = new uchar[width*height*3];
        uchar *tmp_r, *tmp_g, *tmp_b;

        tmp_r = tmp;                       // red channel
        tmp_g = tmp + width * height;      // green channel
        tmp_b = tmp + 2 * width * height;  // blue channel

        for (ulong i=0; i<width*height; i++)
        {
            tmp_r[i] = data[3*i];
            tmp_g[i] = data[3*i+1];
            tmp_b[i] = data[3*i+2];
        }

        io_png_write_uchar(path.c_str(), tmp, width, height, 3);
        cout << "file written: " << path << endl;

        delete tmp;
    }

    void free()
    {
        delete data;
        data = 0;
        width = height = 0;
    }
};

#endif // IMAGE_H
