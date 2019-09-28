//
// Created by Dan Orel on 2019-09-14.
//
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

#ifndef PRACTICE2_HANDS_METHODS_H
#define PRACTICE2_HANDS_METHODS_H
class HistogramManager{
// Interface
public:
    HistogramManager(const Mat&);
    Mat      EqualizeImage();
    Mat ProcessToHistogram();
    Mat  EqualizeHistogram();
    ~HistogramManager();
private:
    int*   CreateHistogram();
    int* CumulateHistogram(int*);
    int*    ScaleHistogram(int*);
// Variables
private:
    Mat Image;
    int ImageSize = 0;
    float alpha   = 0.;

private:
    const int size      = 256;
    int HistogramWidth  = 0,
        HistogramHeight = 0,
        BinWidth        = 0;
};
#endif //PRACTICE2_HANDS_METHODS_H
