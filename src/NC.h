#ifndef NC_H
#define NC_H

#include "common.h"
namespace NC
{

/**
 * Computes a row-wise exclusive Summed Area Table of the Input.
 * This means entry i contains the sum of entries 0 to i-1.
 * The outSAT matrix is assumed to be of size (W+1,H).
 **/
void computeRowSAT(const Mat2<float3>& input, Mat2<float3>& outSAT)
{
    FP_CALL_START(FunP::ID_computeRowSAT);

    assert(input.width+1 == outSAT.width);
    assert(input.height == outSAT.height);

    const uint W = input.width;
    const uint H = input.height;

    for (uint i=0; i<H; i++)
    {
        float3 sum;
        sum.r = sum.g = sum.b = 0;
        for (uint j=0; j<W; j++)
        {
            uint idxIMG = i*W + j;
            uint idxSAT = i*(W+1)+j;

            // store current sum
            outSAT.data[idxSAT].r = sum.r;
            outSAT.data[idxSAT].g = sum.g;
            outSAT.data[idxSAT].b = sum.b;

            // increase sum
            sum.r += input.data[idxIMG].r;
            sum.g += input.data[idxIMG].g;
            sum.b += input.data[idxIMG].b;
        }
        // store complete sum
        uint idxSAT = i*(W+1)+W;
        outSAT.data[idxSAT].r = sum.r;
        outSAT.data[idxSAT].g = sum.g;
        outSAT.data[idxSAT].b = sum.b;
    }

    FP_CALL_END(FunP::ID_computeRowSAT);
}

void TransformedDomainBoxFilter(Mat2<float3>& img,
                                const Mat2<int>& lowerIdx, const Mat2<int>& upperIdx,
                                Mat2<float3>& sat)
{
    assert(img.width+1 == sat.width);
    assert(img.height == sat.height);
    assert(lowerIdx.width == img.width && upperIdx.width == img.width);
    assert(lowerIdx.height == img.height && upperIdx.height == img.height);

    computeRowSAT(img,sat);

    FP_CALL_START(FunP::ID_boxFilter);

    const uint W = img.width;
    const uint H = img.height;

    for (uint i=0; i<H; i++)
    {
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            uint lIdx = lowerIdx.data[idx] + i*(W+1);
            uint uIdx = upperIdx.data[idx] + i*(W+1);

            // TODO: mult vs. div?

            int delta = uIdx - lIdx;

            img.data[idx].r = (sat.data[uIdx].r - sat.data[lIdx].r) / delta;
            img.data[idx].g = (sat.data[uIdx].g - sat.data[lIdx].g) / delta;
            img.data[idx].b = (sat.data[uIdx].b - sat.data[lIdx].b) / delta;
        }
    }

    FP_CALL_END(FunP::ID_boxFilter);
}

void BoxFilterBounds(const Mat2<float>& ct, float boxR, Mat2<int>& outLowerIdx, Mat2<int>& outUpperIdx)
{
    FP_CALL_START(FunP::ID_BoxFilterBounds);

    assert(ct.width == outLowerIdx.width && ct.width == outUpperIdx.width);
    assert(ct.height == outLowerIdx.height && ct.height == outUpperIdx.height);

    const uint W = ct.width;
    const uint H = ct.height;

    for (uint i=0; i<H; i++)
    {
        uint posL = 0;
        uint posR = 0;
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;

            float dtL = ct.data[idx] - boxR;
            float dtR = ct.data[idx] + boxR;

            while (ct.data[i*W+posL] < dtL && posL < W-1)
            {
                posL++;
            }
            while (posR < W && ct.data[i*W+posR] < dtR )  // attention, allows for index = W
            {
                posR++;
            }

            outLowerIdx.data[idx] = posL;
            outUpperIdx.data[idx] = posR;
        }
    }

    FP_CALL_END(FunP::ID_BoxFilterBounds);
}


void filter(Mat2<float3>& img, float sigma_s, float sigma_r, uint nIterations)
{
    FP_CALL_START(FunP::ID_ALL);
    // Estimate horizontal and vertical partial derivatives using finite differences.
    Mat2<float3> dIcdx = diffX(img);
    Mat2<float3> dIcdy = diffY(img);

    const uint W = img.width;
    const uint H = img.height;

    // Compute the l1-norm distance of neighbor pixels.
    FP_CALL_START(FunP::ID_domainTransform);

    Mat2<float> dIdx(W,H);
    Mat2<float> dIdy(W,H);

    for (uint i=0; i<H; i++)
    {
        dIdx.data[i*W] = 0.0f;
        for (uint j=1; j<W; j++)
        {
            uint idx = i*W + j;
            dIdx.data[idx] = fabs(dIcdx.data[idx-1].r) +
                             fabs(dIcdx.data[idx-1].g) +
                             fabs(dIcdx.data[idx-1].b);
        }
    }

    for (uint j=0; j<W; j++)
    {
        dIdy.data[j] = 0.0f;
    }
    for (uint i=1; i<H; i++)
    {
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            dIdy.data[idx] = fabs(dIcdy.data[idx-W].r) +
                             fabs(dIcdy.data[idx-W].g) +
                             fabs(dIcdy.data[idx-W].b);
        }
    }

    // Compute the derivatives of the horizontal and vertical domain transforms.
    float s = sigma_s/sigma_r;
    for (uint i=0; i<H*W; i++)
    {
        dIdx.data[i] = 1.0f + s*dIdx.data[i];
        dIdy.data[i] = 1.0f + s*dIdy.data[i];
    }
    FP_CALL_END(FunP::ID_domainTransform);

    Mat2<float> dIdyT(H,W);

    cumsumX(dIdx);
    transpose(dIdy,dIdyT);
    cumsumX(dIdyT);

    Mat2<int> lowerX(dIdx.width,dIdx.height), upperX(dIdx.width,dIdx.height);
    Mat2<int> lowerY(dIdyT.width,dIdyT.height), upperY(dIdyT.width,dIdyT.height);

    Mat2<float3> satX(img.width+1,img.height);
    Mat2<float3> satY(img.height+1,img.width);

    Mat2<float3> imgT(img.height,img.width);

    for(uint i=0; i<nIterations; i++)
    {
        // Compute the sigma value for this iteration
        float sigmaHi = sigma_s * sqrt(3) * powf(2.0,(nIterations - (i+1))) / sqrt(powf(4,nIterations)-1);

        // Compute the radius of the box filter with the desired variance.
        float boxR = sqrt(3) * sigmaHi;

        BoxFilterBounds(dIdx,boxR,lowerX,upperX);
        TransformedDomainBoxFilter(img, lowerX, upperX, satX);
        transpose(img,imgT);

        BoxFilterBounds(dIdyT,boxR,lowerY,upperY);
        TransformedDomainBoxFilter(imgT, lowerY, upperY,satY);
        transpose(imgT,img);
    }


    // cleanup
    dIcdx.free();
    dIcdy.free();
    dIdx.free();
    dIdy.free();
    dIdyT.free();

    lowerX.free();
    upperX.free();
    lowerY.free();
    upperY.free();

    satX.free();
    satY.free();

    imgT.free();

    FP_CALL_END(FunP::ID_ALL);
}



}

#endif // NC_H
