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
    this->BinWidth        = cvRound(static_cast<double>(this->HistogramWidth / this->size));
}

Mat HistogramManager::HistogramImage() {
    int* histogram = CreateHistogram();
    Mat HistogramImage = ProcessToHistogram(histogram);
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
    Mat ImageEqualizedHistogram;
    // Generate the histogram
    int *histogram = CreateHistogram();

    // Cumulate the histogram
    int *cumhistogram = CumulateHistogram(histogram);

    // Generate the scaled histogram
    int *scaledhistogram = ScaleHistogram(cumhistogram);

    // Calculate the probability of each intensity
    float *IntensityProbability = new float[this->size];
    for(int i = 0; i < this->size; i++)
        IntensityProbability[i] = static_cast<double>(histogram[i]) / static_cast<double>(this->ImageSize);

    // Initialize the equalized histogram
    float *equalizedhistogram_float = new float[this->size];
    for(int i = 0; i < this->size; i++)
        equalizedhistogram_float[i] = 0.;

    // Generate the equalized histogram
    for(int i = 0; i < this->size; i++)
        equalizedhistogram_float[scaledhistogram[i]] += IntensityProbability[i];

    int *equalizedhistogram_int = new int[this->size];
    for(int i = 0; i < this->size; i++)
        equalizedhistogram_int[i] = cvRound(equalizedhistogram_float[i] * 255);

    // Create the Mat object of equalized histogram
    ImageEqualizedHistogram = ProcessToHistogram(equalizedhistogram_int);
    return ImageEqualizedHistogram;
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

Mat HistogramManager::ProcessToHistogram(int *histogram) {
    // Initialization step
    Mat HistogramImage(
            this->HistogramHeight,
            this->HistogramWidth,
            CV_8UC1,
            Scalar(255, 255, 255)
    );

    // Finding the maximum intensity element from histogram
    int MaxIntensity = histogram[0];
    for(int i = 1; i < this->size; i++){
        if(MaxIntensity < histogram[i])
            MaxIntensity = histogram[i];
    }

    // Normalization of the histogram between 0 and 1 values
    for(int i = 0; i < this->size; i++) {
        histogram[i] = (static_cast<float>(histogram[i]) / static_cast<float>(MaxIntensity)) * HistogramImage.rows;
    }

    // Draw the intensity line for histogram
    for(int i = 0; i < this->size; i++)
        line(
                HistogramImage,
                Point(this->BinWidth * (i), this->HistogramHeight),
                Point(this->BinWidth * (i), this->HistogramHeight - histogram[i]),
                Scalar(0,0,0),
                1,
                8,
                0
        );
    return HistogramImage;
}

HistogramManager::~HistogramManager() {

}