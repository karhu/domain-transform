#ifndef MAT2_H
#define MAT2_H

#include <xmmintrin.h>


//struct float3{
//    float r,g,b;
//} __attribute__((aligned (16)));

//struct double3{
//    double r,g,b;
//};


//struct float3{
//    float r,g,b;
//};

union float3{
  struct{ float r,g,b,a; };
  __m128 mmvalue;
};

struct uchar3{
    uchar r,g,b;
};

template<typename T>
struct Mat2
{
    uint width, height;
    T* data; // size: width*height

    /** Creates an uninitialized image of given size.  **/
    Mat2(uint w, uint h)
    {
        width = w;
        height = h;
        data = new T[w*h];

    }

    /** Sets the all entries to 0. **/
    void clear()
    {
        for (ulong i=0; i<width*height*3;i++)
        {
            data[i] = 0;
        }
    }

    void free()
    {
        delete[] data;
        data = 0;
        width = height = 0;
    }
};

#endif // MAT2_H
