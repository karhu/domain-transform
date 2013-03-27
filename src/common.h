#ifndef COMMON_H
#define COMMON_H

#include "Mat2.h"

Mat2<float3> diffX(Mat2<float3> input)
{
    const uint W = input.width;
    const uint H = input.height;

    Mat2<float3> output(W,H);

    for (uint i=0; i<H; i++)
    {
        for (uint j=0; j<W-1; j++)
        {
            uint idx = i*W + j;
            output.data[idx].r = input.data[idx+1].r - input.data[idx].r;
            output.data[idx].g = input.data[idx+1].g - input.data[idx].g;
            output.data[idx].b = input.data[idx+1].b - input.data[idx].b;
        }
    }
}

Mat2<float3> diffY(Mat2<float3> input)
{
    const uint W = input.width;
    const uint H = input.height;

    Mat2<float3> output(W,H);

    for (uint i=0; i<H-1; i++)
    {
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            output.data[idx].r = input.data[idx+W].r - input.data[idx].r;
            output.data[idx].g = input.data[idx+W].g - input.data[idx].g;
            output.data[idx].b = input.data[idx+W].b - input.data[idx].b;
        }
    }
}

/** In place cumulative sum along width/X/rows **/
void cumsumX(Mat2<float3> img)
{
    for (uint i=0; i<H; i++)
    {
        float sum = 0;
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            sum += img.data[idx];
            img.data[idx] = sum;
        }
    }
}

/** In place cumulative sum along height/Y/cols **/
void cumsumY(Mat2<float3> img)
{
    for (uint j=0; j<W; j++)
    {
        float sum = 0;
        for (uint i=0; i<H; i++)
        {
            uint idx = i*W + j;
            sum += img.data[idx];
            img.data[idx] = sum;
        }
    }
}

Mat2<float3> transposed(Mat2<float3> in)
{
    Mat2<float3> out(in.height,in.width);

    for (uint i=0; i<in.height; i++)
    {
        for (uint j=0; j<in.width; j++)
        {
            out.data[j*in.height + i] = in.data[i*in.width+j];
        }
    }
}


#endif // COMMON_H
