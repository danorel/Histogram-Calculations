#include <iostream>
#include "histogram.h"


int main() {
    std::string pathToImage = "/Users/danorel/Workspace/World of C/C++/KMA/MMPI/Histogram-Calculations/";
    std::string image = "image1.jpg";
    std::string ImageLabel = "Original Image";
    std::string ImageHistogramLabel = "Original Histogram";
    std::string ImageEqualizedLabel = "Equalized Image";
    std::string ImageEqualizedHistogramLabel = "Equalized Histogram";

    Mat Image = imread(pathToImage + image, IMREAD_GRAYSCALE);

    HistogramManager Histogram = HistogramManager(Image);
    Mat ImageEqualized          = Histogram.EqualizeImage();
    Mat ImageHistogram          = Histogram.HistogramImage();
    Mat ImageEqualizedHistogram = Histogram.EqualizeHistogram();

    imshow(ImageLabel, Image);
    imshow(ImageHistogramLabel, ImageHistogram);
    imshow(ImageEqualizedLabel, ImageEqualized);
    imshow(ImageEqualizedHistogramLabel, ImageEqualizedHistogram);
    waitKey();
    return 0;
}