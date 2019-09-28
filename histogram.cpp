//
// Created by Dan Orel on 2019-09-14.
//
#include "histogram.h"


HistogramManager::HistogramManager(const Mat &Image) {
    /*
     * Image configurations
     */
    this->Image     = Image;
    this->ImageSize = Image.rows * Image.cols;
    this->alpha     = static_cast<float>(255. / ImageSize);
    /*
     * Histogram configurations
     */
    this->HistogramWidth  = 640;
    this->HistogramHeight = 480;
    this->BinWidth        = cvRound(static_cast<double>(this->HistogramWidth / size));
}



Mat HistogramManager::ProcessToHistogram() {
    // Initialization step
    Mat HistogramImage(
            this->HistogramHeight,
            this->HistogramWidth,
            CV_8UC1,
            Scalar(255, 255, 255)
    );
    // Histogram initialization
    int *histogram = CreateHistogram();

    // Finding the maximum intensity element from histogram
    int MaxIntensity = histogram[0];
    for(int i = 1; i < size; i++){
        if(MaxIntensity < histogram[i])
            MaxIntensity = histogram[i];
    }

    // Normalization of the histogram between 0 and 1 values
    for(int i = 0; i < size; i++)
        histogram[i] = static_cast<double>(histogram[i] / MaxIntensity) * HistogramImage.rows;

    // Draw the intensity line for histogram
    for(int i = 0; i < size; i++)
        line(
                HistogramImage,
                Point(BinWidth * (i), this->HistogramHeight),
                Point(BinWidth * (i), this->HistogramHeight - histogram[i]),
                Scalar(0,0,0),
                1,
                8,
                0
        );
    return HistogramImage;
}

Mat HistogramManager::EqualizeImage() {
    // Initializing the resultative image
    Mat EqualizedImage = this->Image.clone();

    // Generate cumulative frequency histogram
    int *histogram      = CreateHistogram();
    int *cumhistogram = CumulateHistogram(histogram);

    // Scale the cumulative histogram
    int *scaledhistogram = ScaleHistogram(cumhistogram);

    // Generate the equalized image
    for(int y = 0; y < Image.rows; y++)
        for(int x = 0; x < Image.cols; x++)
            EqualizedImage.at<uchar>(y, x) = saturate_cast<uchar>(scaledhistogram[Image.at<uchar>(y, x)]);
    return EqualizedImage;
}

Mat HistogramManager::EqualizeHistogram() {
    // Generate the histogram
    int *histogram = CreateHistogram();

    // Calculate the probability of each intensity
    float *IntensityProbability = new float[this->size];
    for(int i = 0; i < this->size; i++)
        IntensityProbability[i] = static_cast<double>(histogram[i] / this->ImageSize);

    // Initialize the equalized histogram
    float *EqualizedHist = new float[this->size];
    for(int i = 0; i < this->size; i++)
        EqualizedHist[i] = 0;

    // Generate the scaled histogram
    int *scaledhistogram = ScaleHistogram(histogram);

    // Generate the equalized histogram
    for(int i = 0; i < size; i++)
        EqualizedHist[scaledhistogram[i]] += IntensityProbability[i];

    int *ImageEqualizedHistogram = new int[size];
    for(int i = 0; i < size; i++)
        ImageEqualizedHistogram[i] = cvRound(EqualizedHist[i] * 255);

    // Create the Mat object of equalized histogram
    return Mat();
}

int *HistogramManager::CreateHistogram() {
    int *histogram = new int[this->size];
    // Initialization step
    for(int i = 0; i < this->size; i++)
        histogram[i] = 0;
    // Amount of pixels for each intensity values
    for(int y = 0; y < this->Image.rows; y++)
        for(int x = 0; x < this->Image.cols; x++)
            histogram[static_cast<int>(this->Image.at<uchar>(y, x))]++;
    return histogram;
}

int *HistogramManager::CumulateHistogram(int* histogram) {
    int *cumhistogram = new int[this->size];
    // Initialization step
    for(int i = 0; i < this->size; i++)
        cumhistogram[i] = 0;
    cumhistogram[0] = histogram[0];
    // Implementation of the cumulations
    for(int i = 1; i < this->size; i++)
        cumhistogram[i] = histogram[i] + cumhistogram[i - 1];
    return cumhistogram;
}

int *HistogramManager::ScaleHistogram(int *cumhistogram) {
    int *scaledhistogram = new int[this->size];
    for(int i = 0; i < this->size; i++)
        scaledhistogram[i] = cvRound(static_cast<double>(cumhistogram[i] * this->alpha));
    return scaledhistogram;
}

HistogramManager::~HistogramManager() {

}


