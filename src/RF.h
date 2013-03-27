#ifndef RF_H
#define RF_H

#include "common.h"

namespace RF {
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
            dIdx.data[idx] = fabs(dIcdx.data[idx].r) +
                             fabs(dIcdx.data[idx].g) +
                             fabs(dIcdx.data[idx].b);

            dIdy.data[idx] = fabs(dIcdy.data[idx].r) +
                             fabs(dIcdy.data[idx].g) +
                             fabs(dIcdy.data[idx].b);
        }
    }


    Mat2<float> dHdx(W,H);
    Mat2<float> dVdy(W,H);
    for (uint i=0; i<H; i++)
    {
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            dHdx.data[idx] = 1.0+sigma_s/sigma_r*dIdx.data[idx];
            dVdy.data[idx] = 1.0+sigma_s/sigma_r*dIdy.data[idx];
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

    uint n = nIterations;

    Mat2<float3> F = img;

    double sigma_H = sigma_s;
    for (uint i=0; i<nIterations; i++)
    {
//        double sigma_H_i = sigma_H*sqrt(3)*pow
    }
    // ...

    dIcdx.free();
    dIcdy.free();
    dIdx.free();
    dIdy.free();
}

};


#endif // RF_H
