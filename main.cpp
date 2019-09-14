#include <iostream>
#include "hands_methods.h"


int main() {
    std::string pathToImage = "/Users/danorel/Workspace/World of C/C++/KMA/MMPI/Histogram-Calculations/";
    std::string image = "monster.jpg";

    Mat HistImage = ConstructHistogramFrom(pathToImage + image);
    namedWindow("Histogram-Calculations", WINDOW_AUTOSIZE );
    imshow("Histogram-Calculations", HistImage );
    waitKey(0);
    return 0;
}