#ifndef COMMON_H
#define COMMON_H

//#define DO_INLINE

#include <cmath>
#include "types.h"
#include "Mat2.h"

#include "FunctionProfiling.h"

/** has an additional column at the end with random data **/
#ifdef DO_INLINE
inline
#endif
Mat2<float3> diffX(Mat2<float3>& input)
{
    FP_CALL_START(FunP::ID_diffX);

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

    FP_CALL_END(FunP::ID_diffX);
    return output;
}

/** has an additional row at the end with random data **/
#ifdef DO_INLINE
inline
#endif
Mat2<float3> diffY(Mat2<float3>& input)
{
    FP_CALL_START(FunP::ID_diffY);

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

    FP_CALL_END(FunP::ID_diffY);
    return output;
}

/** In place cumulative sum along width/X/rows **/
#ifdef DO_INLINE
inline
#endif
void cumsumX(Mat2<float>& img)
{
    FP_CALL_START(FunP::ID_cumsumX);

    //TODO: optimize for instruction level parallelism
    uint H = img.height;
    uint W = img.width;
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

    FP_CALL_END(FunP::ID_cumsumX);
}

/** In place cumulative sum along height/Y/cols **/
#ifdef DO_INLINE
inline
#endif
void cumsumY(Mat2<float>& img)
{
    uint H = img.height;
    uint W = img.width;
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

// please recheck this function
#ifdef DO_INLINE
inline
#endif
void transpose(Mat2<float3>& in)
{
    FP_CALL_START(FunP::ID_transpose_rgb);

    uint H = in.height;
    uint W = in.width;

    float3 *m = in.data;
    float3 tmp;
// Code adapted from
// http://rosettacode.org/wiki/Matrix_transposition#C
    int start, next, i;
    for (start = 0; start <= W * H - 1; start++) {
        next = start;
        i = 0;
        do {	i++;
            next = (next % H) * W + next / H;
        } while (next > start);
        if (next < start || i == 1) continue;

        tmp = m[next = start];
        do {
            i = (next % H) * W + next / H;
            m[next] = (i == start) ? tmp : m[i];
            next = i;
        } while (next > start);
    }

    in.height = W;
    in.width = H;

    FP_CALL_END(FunP::ID_transpose_rgb);
}

#ifdef DO_INLINE
inline
#endif
void transpose(Mat2<float>& in, Mat2<float>& out)
{
    FP_CALL_START(FunP::ID_transpose_float);

    assert(in.height == out.width);
    assert(in.width == out.height);

    uint H = in.height;
    uint W = in.width;

    for (uint y=0; y<H; y++)
    {
        for (uint x=0; x<W; x++)
        {
            uint idx = y*W + x;
            uint idxT = x*H + y;
            out.data[idxT] = in.data[idx];
        }
    }


    FP_CALL_END(FunP::ID_transpose_float);
}

#ifdef DO_INLINE
inline
#endif
void transposeB(Mat2<float>& in, Mat2<float>& out)
{
    FP_CALL_START(FunP::ID_transpose_float);

    assert(in.height == out.width);
    assert(in.width == out.height);

    uint H = in.height;
    uint W = in.width;

    const uint BLOCK = 16;

    uint Hmod = in.height - in.height%BLOCK;
    uint Wmod = in.width - in.width%BLOCK;

    uint yo,xo;
    for (yo=0; yo<Hmod; yo+=BLOCK)
    {
        for (xo=0; xo<Wmod; xo+=BLOCK)
        {
            for (uint y=yo; y<yo+BLOCK; y++)
            {
                for (uint x=xo; x<xo+BLOCK; x++)
                {
                    uint idx = y*W + x;
                    uint idxT = x*H + y;
                    out.data[idxT] = in.data[idx];
                }
            }
        }
        // right most block
        xo = Wmod;
        for (uint y=yo; y<yo+BLOCK; y++)
        {
            for (uint x=xo; x<W; x++)
            {
                uint idx = y*W + x;
                uint idxT = x*H + y;
                out.data[idxT] = in.data[idx];
            }
        }
    }
    // bottom row
    yo = Hmod;
    for (xo=0; xo<Wmod; xo+=BLOCK)
    {
        for (uint y=yo; y<H; y++)
        {
            for (uint x=xo; x<xo+BLOCK; x++)
            {
                uint idx = y*W + x;
                uint idxT = x*H + y;
                out.data[idxT] = in.data[idx];
            }
        }
    }
    // bottom right block
    for (uint y=yo; y<H; y++)
    {
        for (uint x=xo; x<W; x++)
        {
            uint idx = y*W + x;
            uint idxT = x*H + y;
            out.data[idxT] = in.data[idx];
        }
    }

    FP_CALL_END(FunP::ID_transpose_float);
}

#ifdef DO_INLINE
inline
#endif
void transpose(Mat2<float3>& in, Mat2<float3>& out)
{
    FP_CALL_START(FunP::ID_transpose_rgb);

    assert(in.height == out.width);
    assert(in.width == out.height);

    uint H = in.height;
    uint W = in.width;

    for (uint y=0; y<H; y++)
    {
        for (uint x=0; x<W; x++)
        {
            uint idx = y*W + x;
            uint idxT = x*H + y;
            out.data[idxT] = in.data[idx];
        }
    }

    FP_CALL_END(FunP::ID_transpose_rgb);
}


#ifdef DO_INLINE
inline
#endif
void transposeB(Mat2<float3>& in, Mat2<float3>& out)
{
    FP_CALL_START(FunP::ID_transpose_rgb);

    assert(in.height == out.width);
    assert(in.width == out.height);

    uint H = in.height;
    uint W = in.width;

    const uint BLOCK = 64;

    uint Hmod = in.height - in.height%BLOCK;
    uint Wmod = in.width - in.width%BLOCK;

    uint yo,xo;
    for (yo=0; yo<Hmod; yo+=BLOCK)
    {
        for (xo=0; xo<Wmod; xo+=BLOCK)
        {
            for (uint y=yo; y<yo+BLOCK; y++)
            {
                for (uint x=xo; x<xo+BLOCK; x++)
                {
                    uint idx = y*W + x;
                    uint idxT = x*H + y;
                    out.data[idxT] = in.data[idx];
                }
            }
        }
        // right most block
        xo = Wmod;
        for (uint y=yo; y<yo+BLOCK; y++)
        {
            for (uint x=xo; x<W; x++)
            {
                uint idx = y*W + x;
                uint idxT = x*H + y;
                out.data[idxT] = in.data[idx];
            }
        }
    }
    // bottom row
    yo = Hmod;
    for (xo=0; xo<Wmod; xo+=BLOCK)
    {
        for (uint y=yo; y<H; y++)
        {
            for (uint x=xo; x<xo+BLOCK; x++)
            {
                uint idx = y*W + x;
                uint idxT = x*H + y;
                out.data[idxT] = in.data[idx];
            }
        }
    }
    // bottom right block
    for (uint y=yo; y<H; y++)
    {
        for (uint x=xo; x<W; x++)
        {
            uint idx = y*W + x;
            uint idxT = x*H + y;
            out.data[idxT] = in.data[idx];
        }
    }

    FP_CALL_END(FunP::ID_transpose_rgb);
}


#ifdef DO_INLINE
inline
#endif
void transpose(Mat2<float>& in)
{
    FP_CALL_START(FunP::ID_transpose_float);

    uint H = in.height;
    uint W = in.width;

    float *m = in.data;
    float tmp;
// Code adapted from
// http://rosettacode.org/wiki/Matrix_transposition#C
    int start, next, i;
    for (start = 0; start <= W * H - 1; start++) {
        next = start;
        i = 0;
        do {	i++;
            next = (next % H) * W + next / H;
        } while (next > start);
        if (next < start || i == 1) continue;

        tmp = m[next = start];
        do {
            i = (next % H) * W + next / H;
            m[next] = (i == start) ? tmp : m[i];
            next = i;
        } while (next > start);
    }

    in.height = W;
    in.width = H;

    FP_CALL_END(FunP::ID_transpose_float);
}


#ifdef DO_INLINE
inline
#endif
void copy(const Mat2<float3>& source, Mat2<float3>& target)
{
    const uint H = source.height;
    const uint W = source.width;

    assert(source.height == target.height);
    assert(source.width == target.width);

    for (uint i=0; i<W*H; i++)
    {
        target.data[i] = source.data[i];
    }
}



#endif // COMMON_H
