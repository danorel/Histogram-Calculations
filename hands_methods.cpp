//
// Created by Dan Orel on 2019-09-14.
//
#include "hands_methods.h"


void constructHistogram(std::string filename){
    Mat BlueHist, GreenHist, RedHist;
    Mat RGBImage = imread(filename, CV_8UC3);
    if(!RGBImage.data){
        std::cerr << "Error! Not such image exists!" << std::endl;
        return;
    }
    std::vector<Mat> RGBPlanes;
    split(RGBImage, RGBPlanes);

}
