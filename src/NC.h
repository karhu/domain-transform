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
void computeRowSAT1(const Mat2<float>& input, Mat2<float>& outSAT)
{
    FP_CALL_START(FunP::ID_computeRowSAT1);

    assert(input.width+1 == outSAT.width);
    assert(input.height == outSAT.height);

    const uint W = input.width;
    const uint H = input.height;

    for (uint i=0; i<H; i++)
    {
        float sum;
        sum = 0;
        for (uint j=0; j<W; j++)
        {
            uint idxIMG = i*W + j;
            uint idxSAT = i*(W+1)+j;

            // store current sum
            outSAT.data[idxSAT] = sum;

            // increase sum
            sum += input.data[idxIMG];
        }
        // store complete sum
        uint idxSAT = i*(W+1)+W;
        outSAT.data[idxSAT] = sum;
    }

    FP_CALL_END(FunP::ID_computeRowSAT1);
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
void TransformedDomainBoxFilter1(Mat2<float>& r, Mat2<float>& g, Mat2<float>& b,
                                const Mat2<int>& lowerIdx, const Mat2<int>& upperIdx,
                                Mat2<float>& satR, Mat2<float>& satG, Mat2<float>& satB)
{
//    assert(img.width+1 == sat.width);
//    assert(img.height == sat.height);
//    assert(lowerIdx.width == img.width && upperIdx.width == img.width);
//    assert(lowerIdx.height == img.height && upperIdx.height == img.height);

    computeRowSAT1(r,satR);
    computeRowSAT1(g,satG);
    computeRowSAT1(b,satB);

    FP_CALL_START(FunP::ID_boxFilter1);

    const uint W = r.width;
    const uint H = r.height;

    for (uint i=0; i<H; i++)
    {
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            uint lIdx = lowerIdx.data[idx] + i*(W+1);
            uint uIdx = upperIdx.data[idx] + i*(W+1);

            // TODO: mult vs. div?

            int delta = uIdx - lIdx;

            r.data[idx] = (satR.data[uIdx] - satR.data[lIdx]) / delta;
            g.data[idx] = (satG.data[uIdx] - satG.data[lIdx]) / delta;
            b.data[idx] = (satB.data[uIdx] - satB.data[lIdx]) / delta;
        }
    }

    FP_CALL_END(FunP::ID_boxFilter1);
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


void filter(Mat2<float>& r,Mat2<float>& g, Mat2<float>& b, float sigma_s, float sigma_r, uint nIterations)
{
    FP_CALL_START(FunP::ID_ALL);
    // Estimate horizontal and vertical partial derivatives using finite differences.
    Mat2<float> dIcdxR = diffX1(r);
    Mat2<float> dIcdxG = diffX1(g);
    Mat2<float> dIcdxB = diffX1(b);

    Mat2<float> dIcdyR = diffY1(r);
    Mat2<float> dIcdyG = diffY1(g);
    Mat2<float> dIcdyB = diffY1(b);

    const uint W = r.width;
    const uint H = r.height;

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
            dIdx.data[idx] = fabs(dIcdxR.data[idx-1]) +
                             fabs(dIcdxG.data[idx-1]) +
                             fabs(dIcdxB.data[idx-1]);
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
            dIdy.data[idx] = fabs(dIcdyR.data[idx-W]) +
                             fabs(dIcdyG.data[idx-W]) +
                             fabs(dIcdyB.data[idx-W]);
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
    transposeB(dIdy,dIdyT);
    cumsumX(dIdyT);

    Mat2<int> lowerX(dIdx.width,dIdx.height), upperX(dIdx.width,dIdx.height);
    Mat2<int> lowerY(dIdyT.width,dIdyT.height), upperY(dIdyT.width,dIdyT.height);

    Mat2<float> satRX(r.width+1,r.height);
    Mat2<float> satGX(g.width+1,g.height);
    Mat2<float> satBX(b.width+1,b.height);
    Mat2<float> satRY(r.height+1,r.width);
    Mat2<float> satGY(g.height+1,g.width);
    Mat2<float> satBY(b.height+1,b.width);

    Mat2<float> rT(r.height,r.width);
    Mat2<float> gT(g.height,g.width);
    Mat2<float> bT(b.height,b.width);

    for(uint i=0; i<nIterations; i++)
    {
        // Compute the sigma value for this iteration
        float sigmaHi = sigma_s * sqrt(3) * powf(2.0,(nIterations - (i+1))) / sqrt(powf(4,nIterations)-1);

        // Compute the radius of the box filter with the desired variance.
        float boxR = sqrt(3) * sigmaHi;

        BoxFilterBounds(dIdx,boxR,lowerX,upperX);
        TransformedDomainBoxFilter1(r,g,b, lowerX, upperX, satRX, satGX, satBX);
        transposeB(r,rT);
        transposeB(g,gT);
        transposeB(b,bT);

        BoxFilterBounds(dIdyT,boxR,lowerY,upperY);
        TransformedDomainBoxFilter1(rT, gT, bT, lowerY, upperY, satRY, satGY, satBY);
        transposeB(rT,r);
        transposeB(gT,g);
        transposeB(bT,b);
    }


    // cleanup
    dIcdxR.free();
    dIcdxG.free();
    dIcdxB.free();
    dIcdyR.free();
    dIcdyG.free();
    dIcdyB.free();
    dIdx.free();
    dIdy.free();
    dIdyT.free();

    lowerX.free();
    upperX.free();
    lowerY.free();
    upperY.free();

    satRX.free();
    satGX.free();
    satBX.free();
    satRY.free();
    satGY.free();
    satBY.free();

    rT.free();
    gT.free();
    bT.free();

    FP_CALL_END(FunP::ID_ALL);
}



}

#endif // NC_H
