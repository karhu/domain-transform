#ifndef NC_H
#define NC_H

#include "Image.h"

namespace NC
{

Image<float3> diffX(Image<float3> input)
{
    const uint W = input.width;
    const uint H = input.height;

    Image<float3> output(W,H);

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

Image<float3> diffY(Image<float3> input)
{
    const uint W = input.width;
    const uint H = input.height;

    Image<float3> output(W,H);

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

void filter(Image<float3> img, double sigma_s, double sigma_r, uint nIterations)
{
    //Estimate horizontal and vertical partial derivatives using finite differences.
    Image<float3> dIcdx = diffX(img);
    Image<float3> dIcdy = diffY(img);

    const uint W = img.width;
    const uint H = img.height;

    // Compute the l1-norm distance of neighbor pixels.
    Image<float> dIdx(W,H);
    Image<float> dIdy(W,H);
    for (uint i=0; i<H; i++)
    {
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            dIdx[idx] = abs<float>(dIcdx[idx].r) +
                        abs<float>(dIcdx[idx].g) +
                        abs<float>(dIcdx[idx].b);

            dIdy[idx] = abs<float>(dIcdy[idx].r) +
                        abs<float>(dIcdy[idx].g) +
                        abs<float>(dIcdy[idx].b);
        }
    }



    // ...

    dIcdx.free();
    dIcdy.free();
    dIdx.free();
    dIdy.free();
}

};

#endif // NC_H
