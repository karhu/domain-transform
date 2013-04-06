#include <iostream>

#include "tclap/CmdLine.h"
#include "Image.h"
#include "mainwindow.h"

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

        cmd.parse( argc, argv );
        inputPath = inputArg.getValue();
        outputPath = outputArg.getValue();
        methodStringShort = methodArg.getValue();
        nIterations = iterationsArg.getValue();
        sigmaS = sigmaSArg.getValue();
        sigmaR = sigmaRArg.getValue();


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

    cout << "[Input]           " << inputPath << endl;
    cout << "[Output]          " << outputPath << endl;
    cout << "[Method]          " << methodString << endl;
    cout << "[Iterations]      " << nIterations << endl;
    cout << "[Spatial Sigma]   " << sigmaS << endl;
    cout << "[Range Sigma]     " << sigmaR << endl;

//    createWindow(argc, argv);

    Mat2<float3> img = LoadPNG(inputPath);

    cout << "[Dimensions]      " << img.width << " x " << img.height << endl;

    if (method == RF)
        RF::filter(img, sigmaS, sigmaR, nIterations);
    else if (method == NC)
        NC::filter(img, sigmaS, sigmaR, nIterations);

    SavePNG(outputPath,img);
    img.free();

    return 0;
}

