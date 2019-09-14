//
// Created by Dan Orel on 2019-09-14.
//
#include "hands_methods.h"


Mat ConstructHistogramFrom(std::string filename){
    Mat RGBImage;
    RGBImage = imread(filename, 1 );
    if( !RGBImage.data ){
        exit(-1);
    }
    
    /// Separate the image in 3 places ( B, G and R )
    std::vector<Mat> BGRPlanes;
    split(RGBImage, BGRPlanes );

    /// Establish the number of bins
    int HistSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* HistRange = { range };

    bool uniform = true;
    bool accumulate = false;

    Mat BlueHist, GreenHist, RedHist;

    /// Compute the histograms:
    calcHist(&BGRPlanes[0], 1, 0, Mat(), BlueHist, 1, &HistSize, &HistRange, uniform, accumulate );
    calcHist(&BGRPlanes[1], 1, 0, Mat(), GreenHist, 1, &HistSize, &HistRange, uniform, accumulate );
    calcHist(&BGRPlanes[2], 1, 0, Mat(), RedHist, 1, &HistSize, &HistRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int HistWidth = 512; int HistHeight = 400;
    int bin_w = cvRound((double) HistWidth / HistSize );

    Mat HistImage(
            HistHeight,
            HistWidth,
            CV_8UC3,
            Scalar(0, 0, 0)
    );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(BlueHist, BlueHist, 0, HistImage.rows, NORM_MINMAX, -1, Mat());
    normalize(GreenHist, GreenHist, 0, HistImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(RedHist, RedHist, 0, HistImage.rows, NORM_MINMAX, -1, Mat());

    /// Draw for each channel
    for(int i = 1; i < HistSize; i++ )
    {
        line(HistImage, Point(bin_w * (i - 1), HistHeight - cvRound(BlueHist.at<float>(i - 1)) ) ,
             Point( bin_w*(i), HistHeight - cvRound(BlueHist.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
        line(HistImage, Point(bin_w * (i - 1), HistHeight - cvRound(GreenHist.at<float>(i - 1)) ) ,
             Point( bin_w*(i), HistHeight - cvRound(GreenHist.at<float>(i)) ),
             Scalar( 0, 255, 0), 2, 8, 0  );
        line(HistImage, Point(bin_w * (i - 1), HistHeight - cvRound(RedHist.at<float>(i - 1)) ) ,
             Point( bin_w*(i), HistHeight - cvRound(RedHist.at<float>(i)) ),
             Scalar( 0, 0, 255), 2, 8, 0  );
    }

    return HistImage;
}
