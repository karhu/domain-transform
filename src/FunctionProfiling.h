#ifndef FUNCTIONPROFILING_H
#define FUNCTIONPROFILING_H

#include <string>
#include "rdtsc.h"

namespace FunP
{
    struct Info
    {
        std::string name;
        uint call_counter;
        double total_cycles;
        tsc_counter start, end;
    };

///////////////////////////////////////////////////////
// needs edit if more functions should be profiled

    static const uint N_DATA_LINES = 10;

    static Info Data[N_DATA_LINES] = {
        {"all",0,0.0,{0},{0}},
        {"diffX",0,0.0,{0},{0}},
        {"diffY",0,0.0,{0},{0}},
        {"domainTransform",0,0.0,{0},{0}},
        {"transpose_float",0,0.0,{0},{0}},
        {"transpose_rgb",0,0.0,{0},{0}},
        {"cumsumX",0,0.0,{0},{0}},
        {"BoxFilterBounds",0,0.0,{0},{0}},
        {"computeRowSAT",0,0.0,{0},{0}},
        {"boxFilter",0,0.0,{0},{0}},
    };

    enum Id
    {
        ID_ALL = 0,
        ID_diffX,
        ID_diffY,
        ID_domainTransform,
        ID_transpose_float,
        ID_transpose_rgb,
        ID_cumsumX,
        ID_BoxFilterBounds,
        ID_computeRowSAT,
        ID_boxFilter,
    };

///////////////////////////////////////////////////////

    inline void StartCall(uint i)
    {
        CPUID(); RDTSC(Data[i].start);
    }

    inline void EndCall(uint i)
    {
        RDTSC(Data[i].end); CPUID();
        Data[i].total_cycles += ((double)COUNTER_DIFF(Data[i].end, Data[i].start));
        Data[i].call_counter++;
    }

    void PrintData()
    {
#ifdef DO_FUNCTION_PROFILING
        std::cout << "'runtime_data': [" << std::endl;
        double total = Data[0].total_cycles / 100.0;
        for (int i=0; i<N_DATA_LINES; i++)
        {
            double cycles_call = Data[i].call_counter == 0 ? 0.0 : Data[i].total_cycles / Data[i].call_counter;
            std::cout << "{" << std::endl;
            std::cout << "'data':        '" << Data[i].name << "',\n";
            std::cout << "'calls':        " << Data[i].call_counter << ",\n";
            std::cout << "'total_cycles': " << Data[i].total_cycles << ",\n";
            std::cout << "'cycles_call':  " << cycles_call << ",\n";
            std::cout << "'percentage':   " << Data[i].total_cycles / total << "\n";
            std::cout << "}," << std::endl;
        }
        std::cout << "]" << std::endl;
#endif
    }

    #if defined(DO_FUNCTION_PROFILING)
    #	define FP_CALL_START(I) FunP::StartCall(I)
    #   define FP_CALL_END(I) FunP::EndCall(I)
    #else
    #	define FP_CALL_START(I)
    #   define FP_CALL_END(I)
    #endif


}
#endif // FUNCTIONPROFILING_H
