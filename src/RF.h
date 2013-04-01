#ifndef RF_H
#define RF_H

#include "common.h"

namespace RF {

void recursiveFilter(Mat2<float3> F, Mat2<float> dHdx, float sigma)
{
    float a = exp(-sqrt(2.0)/sigma);
    uint H = F.height;
    uint W = F.width;

    Mat2<float> V(W, H);

    for (uint i=0; i<H; i++)
    {
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;
            float tmp = dHdx.data[idx];
            V.data[idx] = pow(a, tmp);
        }
    }
//    % Left -> Right filter.
//    for i = 2:w
//        for c = 1:num_channels
//            F(:,i,c) = F(:,i,c) + V(:,i) .* ( F(:,i - 1,c) - F(:,i,c) );
//        end
//    end
    for (uint i=0; i<H; i++)

    {
        for (uint j=1; j<W; j++) // index starts from 1:w

        {

            uint idx = i*W + j;
            F.data[idx].r += V.data[idx]*(F.data[idx-1].r-F.data[idx].r);
            F.data[idx].g += V.data[idx]*(F.data[idx-1].g-F.data[idx].g);
            F.data[idx].b += V.data[idx]*(F.data[idx-1].b-F.data[idx].b);
        }
    }

//    % Right -> Left filter.
//    for i = w-1:-1:1
//        for c = 1:num_channels
//            F(:,i,c) = F(:,i,c) + V(:,i+1) .* ( F(:,i + 1,c) - F(:,i,c) );
//        end
//    end

    for (uint i=0; i<H; i++)

    {
        for (int j=W-2; j>=0; j--) // index starts from 1:w

        {

            uint idx = i*W + j;
            F.data[idx].r += V.data[idx+1]*(F.data[idx+1].r-F.data[idx].r);
            F.data[idx].g += V.data[idx+1]*(F.data[idx+1].g-F.data[idx].g);
            F.data[idx].b += V.data[idx+1]*(F.data[idx+1].b-F.data[idx].b);
        }
    }

//    delete V;
}


void filter(Mat2<float3> F, float sigma_s, float sigma_r, uint nIterations)
{
    //Estimate horizontal and vertical partial derivatives using finite differences.
    Mat2<float3> dIcdx = diffX(F);
    Mat2<float3> dIcdy = diffY(F);

    const uint W = F.width;
    const uint H = F.height;

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

//    Mat2<float> dIdyT = transposed(dIdy);
    transpose(dIdy);

    // Perform the filtering
    uint n = nIterations;

    float sigma_H = sigma_s;
    for (uint i=0; i<nIterations; i++)
    {
        float sigma_H_i = sigma_H*sqrt(3)*powf(2.0, n-(i+1)) / sqrt(powf(4,n)-1);
        recursiveFilter(F, dHdx, sigma_H_i);
        transpose(F);
        recursiveFilter(F, dVdy, sigma_H_i);
        transpose(F);
    }
    // ...

    dIcdx.free();
    dIcdy.free();
    dIdx.free();
    dIdy.free();
}

};


#endif // RF_H
