#ifndef NC_H
#define NC_H

#include "Mat2.h"

namespace NC
{


void filter(Mat2<float3> img, float sigma_s, float sigma_r, uint nIterations)
{
    //Estimate horizontal and vertical partial derivatives using finite differences.
    Mat2<float3> dIcdx = diffX(img);
    Mat2<float3> dIcdy = diffY(img);

    const uint W = img.width;
    const uint H = img.height;

    // Compute the l1-norm distance of neighbor pixels.
    Mat2<float> dIdx(W,H);
    Mat2<float> dIdy(W,H);
    for (uint i=0; i<H; i++)
    {
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            dIdx.data[idx] = abs<float>(dIcdx.data[idx].r) +
                             abs<float>(dIcdx.data[idx].g) +
                             abs<float>(dIcdx.data[idx].b);

            dIdy.data[idx] = abs<float>(dIcdy.data[idx].r) +
                             abs<float>(dIcdy.data[idx].g) +
                             abs<float>(dIcdy.data[idx].b);
        }
    }



    // Compute the derivatives of the horizontal and vertical domain transforms.
    float s = sigma_s/sigma_r;
    for (uint i=0; i<H*W; i++)
    {
        dIdx.data[i] = 1.0f + s*dIdx.data[i];
        dIdy.data[i] = 1.0f + s*dIdy.data[i];
    }

    Mat2<float> dIdyT = transposed(dIdy);

    // ...

    dIcdx.free();
    dIcdy.free();
    dIdx.free();
    dIdy.free();
}

};

#endif // NC_H
