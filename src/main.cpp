#include <iostream>

#include "tclap/CmdLine.h"
#include "Image.h"
#include "rdtsc.h"

#include "FunctionProfiling.h"

using namespace std;

#include "NC.h"
#include "RF.h"
int main(int argc, char** argv)
{
    // Settings Variables //////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////

    enum Method
    {
        Undefined = 0,
        RF = 1,
        NC = 2
    };

    string inputPath, outputPath;
    Method method = Undefined;
    string methodStringShort = "";
    string methodString = "";

    uint nIterations;
    float sigmaS;
    float sigmaR;
    float sigmaR_orig;
    bool benchmark;
    bool benchmarkWarmUp;
    uint benchmarkIterations;

    // Read Command Line Arguments /////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    try
    {
        TCLAP::CmdLine cmd("Edge aware image filtering using domain transform.", ' ', "0.1");

        TCLAP::ValueArg<string> inputArg("i","in","Input image",false,"input.png","string");
        cmd.add(inputArg);

        TCLAP::ValueArg<string> outputArg("o","out","Output image",false,"output.png","string");
        cmd.add(outputArg);

        vector<string> methodVals; methodVals.push_back("rf"); methodVals.push_back("nc");
        TCLAP::ValuesConstraint<string> methodConstr(methodVals);
        TCLAP::ValueArg<string> methodArg("m","method","Filtering method.",true,"nc",&methodConstr);
        cmd.add(methodArg);

        TCLAP::ValueArg<int> iterationsArg("n","nIter","Number of filtering iterations.",false,3,"integer");
        cmd.add(iterationsArg);

        TCLAP::ValueArg<float> sigmaSArg("s","sSigma","Filter spatial standard deviation.",false,40.0f,"float");
        cmd.add(sigmaSArg);

        TCLAP::ValueArg<float> sigmaRArg("r","rSigma","Filter range standard deviation.",false,0.5f,"float");
        cmd.add(sigmaRArg);

        TCLAP::SwitchArg benchmarkSwitch("b","benchmark","Benchmark application. No ouptut will be saved.", cmd, false);
        TCLAP::SwitchArg benchmarkWarmupSwitch("w","benchmarkwarmup","Warmup before benchmarking application.", cmd, false);

        TCLAP::ValueArg<int> benchmarkIterationsArg("t", "tTimes", "Number of executions to benchmark.", false, 1, "integer");
        cmd.add(benchmarkIterationsArg);

        cmd.parse( argc, argv );
        inputPath = inputArg.getValue();
        outputPath = outputArg.getValue();
        methodStringShort = methodArg.getValue();
        nIterations = iterationsArg.getValue();
        sigmaS = sigmaSArg.getValue();
        sigmaR_orig = sigmaRArg.getValue();
        sigmaR = sigmaR_orig*255.0;

        benchmark = benchmarkSwitch.getValue();
        benchmarkIterations = benchmarkIterationsArg.getValue();
        benchmarkWarmUp = benchmarkWarmupSwitch.getValue();

        if (methodStringShort == "nc")
        {
            method = NC;
            methodString = "NC (Normalized Convolution)";
        }
        else if (methodStringShort == "rf")
        {
            method = RF;
            methodString = "RF (Recursive Filtering)";
        }
    }
    catch (TCLAP::ArgException &e)
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    // We parse the benchmark code
    if (!benchmark) {
        cout << "[Input]           " << inputPath << endl;
        cout << "[Output]          " << outputPath << endl;
        cout << "[Method]          " << methodString << endl;
        cout << "[Iterations]      " << nIterations << endl;
        cout << "[Spatial Sigma]   " << sigmaS << endl;
        cout << "[Range Sigma]     " << sigmaR << endl;
    }

    Mat2<float3> img = LoadPNG(inputPath);

    if (!benchmark)
    {
        //    createWindow(argc, argv);
        cout << "[Dimensions]      " << img.width << " x " << img.height << endl;

        if (method == RF)
            RF::filter(img, sigmaS, sigmaR, nIterations);
        else if (method == NC)
            NC::filter(img, sigmaS, sigmaR, nIterations);

        SavePNG(outputPath,img);
        img.free();
        return 0;
    }

    // Benchmarking
    tsc_counter start, end;
    double sumX;
    double sumY;
    double sumZ;
    CPUID(); RDTSC(start); RDTSC(end);
    CPUID(); RDTSC(start); RDTSC(end);
    CPUID(); RDTSC(start); RDTSC(end);

    Mat2<float3> tmpImg(img.width,img.height);
    double total_cycles = 0;


    if (method==RF)
    {
        if (benchmarkWarmUp)
        {
            RF::filter(img, sigmaS, sigmaR, nIterations);
        }

        for (int i=0; i<benchmarkIterations; i++)
        {
            copy(img,tmpImg);
            CPUID(); RDTSC(start);
            RF::filter(tmpImg, sigmaS, sigmaR, nIterations);
            RDTSC(end); CPUID();
            total_cycles += ((double)COUNTER_DIFF(end, start));
        }

    } else if (method == NC)
    {
        if (benchmarkWarmUp)
        {
            NC::filter(img, sigmaS, sigmaR, nIterations);
        }
        for (int i=0; i<benchmarkIterations; i++)
        {
            copy(img,tmpImg);
            CPUID(); RDTSC(start);
            NC::filter(tmpImg, sigmaS, sigmaR, nIterations);
            RDTSC(end); CPUID();
            total_cycles += ((double)COUNTER_DIFF(end, start));
        }
    }
    uint width = img.width;
    for (uint x=0; x<width; x++) {
        for (uint y=0; y<img.height; y++)
        {
            sumX += img.data[y*width+x].r;
            sumY += img.data[y*width+x].g;
            sumZ += img.data[y*width+x].b;
        }
    }

    double cycles = total_cycles / ((double) benchmarkIterations);

    // we emit a python hash to allow for easy parsing
    // import ast
    // o = subprocess.check_output(["./"+command_name])
    // result = ast.literal_eval(o)

    cout << "{'method':        '" << methodStringShort << "'" << std::endl;
    cout << ", 'description':  '" << methodString << "'"<< std::endl;
    cout << ", 'input':        '" << inputPath << "'"<< std::endl;
    cout << ", 'width':        " << img.width << std::endl;
    cout << ", 'height':       " << img.height << std::endl;
    cout << ", 'iterations':   " << nIterations<< std::endl;
    cout << ", 'sigma_s':      " << sigmaS<< std::endl;
    cout << ", 'sigma_r':      " << sigmaR_orig<< std::endl;
    cout << ", 'cycles':       " << cycles<< std::endl;
    cout << ", 'total_cycles': " << total_cycles<< std::endl;
    cout << ", 'benchmark_iterations': " << benchmarkIterations<< std::endl;
    cout << ", 'benchmark_warmup':     " << benchmarkWarmUp<< std::endl;
    cout << ", 'sum': [" << sumX << ", " << sumY << ", " << sumZ << "],"<< std::endl;
    FunP::PrintData();
    cout << "}" << endl;

    img.free();
    tmpImg.free();
    return 0;
}

