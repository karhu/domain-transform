#ifndef NC_H
#define NC_H

#include "common.h"
#include <cstring>

namespace NC
{

void TransformedDomainBoxFilter(Mat2<float3>& img,
                                Mat2<float3>& imgOut,
                                Mat2<float> dIdx, float boxR)
{
    FP_CALL_START(FunP::ID_boxFilter);

    const uint W = img.width;
    const uint H = img.height;

    for (uint i=0; i<H; i++)
    {
        uint posL = 0;
        uint posR = 0;

        // row sat
        float3 sum; sum.r = sum.g = sum.b = 0;

        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            uint idxT = j*H + i;

            // compute box filter bounds
            float dtL = dIdx.data[idx] - boxR;
            float dtR = dIdx.data[idx] + boxR;

            // update box filter window
            while (dIdx.data[i*W+posL] < dtL && posL < W-1)
            {
                sum.r -= img.data[i*W+posL].r;
                sum.g -= img.data[i*W+posL].g;
                sum.b -= img.data[i*W+posL].b;
                posL++;
            }
            while (posR < W && dIdx.data[i*W+posR] < dtR )  // attention, allows for index = W
            {
                sum.r += img.data[i*W+posR].r;
                sum.g += img.data[i*W+posR].g;
                sum.b += img.data[i*W+posR].b;
                posR++;
            }

            int delta = posR - posL;
            float invD = 1.0f / delta;

            imgOut.data[idxT].r = sum.r * invD;
            imgOut.data[idxT].g = sum.g * invD;
            imgOut.data[idxT].b = sum.b * invD;
        }
    }

    FP_CALL_END(FunP::ID_boxFilter);
}


void filter(Mat2<float3>& img, float sigma_s, float sigma_r, uint nIterations)
{
    FP_CALL_START(FunP::ID_ALL);
    // Estimate horizontal and vertical partial derivatives using finite differences.

    // Compute the l1-norm distance of neighbor pixels.
    FP_CALL_START(FunP::ID_domainTransform);

    const uint W = img.width;
    const uint H = img.height;

    float s = sigma_s/sigma_r;

    Mat2<float> dIdx(W,H);
    Mat2<float> dIdy(W,H);

    for (uint j=0; j<W; j++)
    {
        dIdy.data[j] = 1.0f;
    }

    for (uint i=0; i<H-1; i++)
    {
        dIdx.data[i*W] = 1.0f;
        for (uint j=0; j<W-1; j++)
        {
            uint idx = i*W + j;
            dIdx.data[idx+1] = 1.0f +
                               s*(fabs(img.data[idx+1].r - img.data[idx].r) +
                                  fabs(img.data[idx+1].g - img.data[idx].g) +
                                  fabs(img.data[idx+1].b - img.data[idx].b));
            dIdy.data[idx+W] = 1.0f +
                               s*(fabs(img.data[idx+W].r - img.data[idx].r) +
                                  fabs(img.data[idx+W].g - img.data[idx].g) +
                                  fabs(img.data[idx+W].b - img.data[idx].b));
        }
    }

    dIdx.data[(H-1)*W] = 1.0f;
    for (uint j=0; j<W-1; j++)
    {
        uint idx = (H-1)*W + j;
        dIdx.data[idx+1] = 1.0f +
                           s*(fabs(img.data[idx+1].r - img.data[idx].r) +
                              fabs(img.data[idx+1].g - img.data[idx].g) +
                              fabs(img.data[idx+1].b - img.data[idx].b));
    }

    for (uint i=0; i<H-1; i++)
    {
        uint idx = i*W + (W-1);
        dIdy.data[idx+W] = 1.0f +
                           s*(fabs(img.data[idx+W].r - img.data[idx].r) +
                              fabs(img.data[idx+W].g - img.data[idx].g) +
                              fabs(img.data[idx+W].b - img.data[idx].b));
    }


    FP_CALL_END(FunP::ID_domainTransform);

    Mat2<float> dIdyT(H,W);

    cumsumX(dIdx);
    transposeB(dIdy,dIdyT);
    cumsumX(dIdyT);

    Mat2<float3> imgT(img.height,img.width);

    for(uint i=0; i<nIterations; i++)
    {
        // Compute the sigma value for this iteration
        float sigmaHi = sigma_s * sqrt(3) * powf(2.0,(nIterations - (i+1))) / sqrt(powf(4,nIterations)-1);

        // Compute the radius of the box filter with the desired variance.
        float boxR = sqrt(3) * sigmaHi;

        TransformedDomainBoxFilter(img,imgT, dIdx, boxR);
//        transposeB(img,imgT);

        TransformedDomainBoxFilter(imgT,img,dIdyT,boxR);
//        transposeB(imgT,img);
    }


    // cleanup
    dIdx.free();
    dIdy.free();
    dIdyT.free();

    imgT.free();

    FP_CALL_END(FunP::ID_ALL);
}



}

#endif // NC_H
