#include "imagetransformation.h"

ImageTransformation::ImageTransformation()
{

}

ImageTransformation::~ImageTransformation()
{

}

Mat ImageTransformation::rotateImage(Mat originalImage, double angle)
{
    Mat rotatedImage;
    Point2f pt(originalImage.cols/2., originalImage.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(originalImage, rotatedImage, r, Size(originalImage.cols, originalImage.rows));
    return rotatedImage;
}


Mat ImageTransformation::flipImage(Mat originalImage, int flipCode)
{
    cv::Mat flippedImage;
    cv::flip(originalImage,flippedImage,flipCode);
    return flippedImage;
}

Mat ImageTransformation::cropImage(Mat originalImage, Point coordinate, Size rectSize, int offset)
{
    cv::Mat tempImage(originalImage);
    cv::Rect rect = cv::Rect(coordinate.x - offset,coordinate.y - offset, rectSize.width, rectSize.height);
    cv::Mat croppedImage = tempImage(rect);
    return croppedImage;
}



