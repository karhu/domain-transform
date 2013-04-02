#ifndef NC_H
#define NC_H

#include "common.h"
namespace NC
{

void TransformedDomainBoxFilter(Mat2<float3>& img, const Mat2<float>& ct, float boxR)
{

//    TransformedDomainBoxFilter_Horizontal(I, xform_domain_position, box_radius)

//        [h w num_channels] = size(I);

//        % Compute the lower and upper limits of the box kernel in the transformed domain.
//        l_pos = xform_domain_position - box_radius;
//        u_pos = xform_domain_position + box_radius;
    for (uint i=0; i<H; i++)
    {
        for (uint j=0; j<W; j++)
        {
            uint idx = i*W + j;

        }
    }

//        % Find the indices of the pixels associated with the lower and upper limits
//        % of the box kernel.
//        %
//        % This loop is much faster in a compiled language.  If you are using a
//        % MATLAB version which supports the 'parallel for' construct, you can
//        % improve performance by replacing the following 'for' by a 'parfor'.

//        l_idx = zeros(size(xform_domain_position));
//        u_idx = zeros(size(xform_domain_position));



}

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



    // Compute the derivatives of the horizontal and vertical domain transforms.
    float s = sigma_s/sigma_r;
    for (uint i=0; i<H*W; i++)
    {
        dIdx.data[i] = 1.0f + s*dIdx.data[i];
        dIdy.data[i] = 1.0f + s*dIdy.data[i];
    }

    cumsumX(dIdx);
    cumsumY(dIdy);

    Mat2<float> dIdyT = transposed(dIdy);

    Mat2<float3> outImg(img.width,img.height);
    copy(img,outImg);

    for(uint i=0; i<nIterations; i++)
    {
        // Compute the sigma value for this iteration
        float sigmaHi = sigma_s * sqrt(3) * pow(2.0,(nIterations - (i+1))) / sqrt(4^nIterations -1);

        // Compute the radius of the box filter with the desired variance.
        float boxR = sqrt(3) * sigmaHi;

        F = TransformedDomainBoxFilter(img, dIdx, boxR);
        transpose(img);

        F = TransformedDomainBoxFilter(img, dIdy, boxR);
        transpose(img);
    }


    // cleanup
    dIdyT.free();
    dIcdx.free();
    dIcdy.free();
    dIdx.free();
    dIdy.free();
}



};

#endif // NC_H
