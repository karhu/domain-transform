#include <iostream>

#include "tclap/CmdLine.h"
#include "Image.h"
#include "mainwindow.h"

using namespace std;

int main(int argc, char** argv)
{
    string inputPath, outputPath;

    // Read Command Line Arguments /////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    try
    {
        TCLAP::CmdLine cmd("Edge aware image filtering using domain transform.", ' ', "0.1");
        TCLAP::ValueArg<string> inputArg("i","input","Input image",false,"input.png","string");
        TCLAP::ValueArg<string> outputArg("o","output","Output image",false,"output.png","string");
        cmd.add(inputArg);
        cmd.add(outputArg);
        cmd.parse( argc, argv );
        inputPath = inputArg.getValue();
        outputPath = outputArg.getValue();
    }
    catch (TCLAP::ArgException &e)
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    cout << "[Input]  \t" << inputPath << endl;
    cout << "[Output] \t" << outputPath << endl;

//    createWindow(argc, argv);

    Image img(inputPath);

    cout << "[Dimensions] \t" << img.width << " x " << img.height << endl;

    img.save(outputPath);
    img.free();

    return 0;
}

