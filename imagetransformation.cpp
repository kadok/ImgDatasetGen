#include "imagetransformation.h"
#include "log.h"

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

void ImageTransformation::writeImages(Mat imageBase, QString destinationPath, QString fileName, QString extension, MainWindow* mainW)
{

    Mat imageTemp;
    imageBase.copyTo(imageTemp);

    Mat imageRotate0 = imageTemp;
    Mat imageRotate90;
    Mat imageRotate180;
    Mat imageRotate270;

    if (mainW->bResizeChecked)
    {
        int width = mainW->iWidth;
        int height = mainW->iHeight;
        imageTemp.resize(width,height);

        LogImgDataset::getInstance().Log(INFO, "Resize image");
    }

    if (mainW->bRotateChecked)
    {
        //Rotate Images 0 - 90 - 180 -270
        imageRotate0 = imageTemp;
        imageRotate90 = rotateImage(imageTemp, 90.0);
        imageRotate180 = rotateImage(imageTemp, 180.0);
        imageRotate270 = rotateImage(imageTemp, 270.0);

        imwrite((destinationPath + "/" + fileName + extension).toStdString(), imageRotate0 );
        imwrite((destinationPath + "/" + fileName + "_R90" + extension).toStdString(), imageRotate90 );
        imwrite((destinationPath + "/" + fileName + "_R180" + extension).toStdString(), imageRotate180 );
        imwrite((destinationPath + "/" + fileName + "_R270" + extension).toStdString(), imageRotate270 );

        LogImgDataset::getInstance().Log(INFO, "Rotate images: 0 - 90 - 180 - 270");

        if (mainW->bFlipChecked)
        {
            //Vertical Flip in Rotate Images
            Mat imageFlipV0 = flipImage(imageRotate0, 1);
            Mat imageFlipV90 = flipImage(imageRotate90, 1);
            Mat imageFlipV180 = flipImage(imageRotate180, 1);
            Mat imageFlipV270 = flipImage(imageRotate270, 1);

            imwrite((destinationPath + "/" + fileName + "_FlipV0" + extension).toStdString(), imageFlipV0 );
            imwrite((destinationPath + "/" + fileName + "_FlipV90" + extension).toStdString(), imageFlipV90 );
            imwrite((destinationPath + "/" + fileName + "_FlipV180" + extension).toStdString(), imageFlipV180 );
            imwrite((destinationPath + "/" + fileName + "_FlipV270" + extension).toStdString(), imageFlipV270 );

            LogImgDataset::getInstance().Log(INFO, "Fliping ...");
        }

    }
    else if (mainW->bFlipChecked)
    {
        //Vertical Flip in Rotate Images
        Mat imageFlipV0 = flipImage(imageRotate0, 1);
        imwrite((destinationPath + "/" + fileName + "_FlipV0" + ".jpg").toStdString(), imageFlipV0 );

        LogImgDataset::getInstance().Log(INFO, "Fliping ...");
    }

}

void ImageTransformation::writeImages(vector<coordinateInfo> coordinates, Mat imageBase, QString destinationPath, QString fileName, QString extension, MainWindow* mainW)
{

    //Crop the interest area
    for (uint j=0; j < coordinates.size(); j++)
    {
        QString number = "_" + QString::number(j);

        Mat imageRotate0;
        Mat imageRotate90;
        Mat imageRotate180;
        Mat imageRotate270;

        Mat imageTemp;
        imageBase.copyTo(imageTemp);

        if (mainW->bResizeChecked)
        {
            int iWidth = mainW->iWidth;
            int iHeight = mainW->iHeight;
            imageTemp.resize(iWidth,iHeight);

            LogImgDataset::getInstance().Log(INFO, "Resize image");
        }

        if (mainW->bCropChecked) {

            //Recorte da imagem a partir dos dados dos CSVs
            QString number = "_" + QString::number(j);
            Mat imageCrop = cropImage(imageTemp, Point(coordinates[j].x,coordinates[j].y), cv::Size(mainW->iWindowSize,mainW->iWindowSize), mainW->iOffset);
            imwrite((destinationPath + "/" + fileName + number + extension).toStdString(), imageCrop);
            imageTemp = imageCrop;

            LogImgDataset::getInstance().Log(INFO, "Crop image");
        }

        if (mainW->bRotateChecked)
        {
            //Rotate Images 0 - 90 - 180 -270
            imageRotate0 = imageTemp;
            imageRotate90 = rotateImage(imageTemp, 90.0);
            imageRotate180 = rotateImage(imageTemp, 180.0);
            imageRotate270 = rotateImage(imageTemp, 270.0);

            imwrite((destinationPath + "/" + fileName + number + extension).toStdString(), imageRotate0 );
            imwrite((destinationPath + "/" + fileName + number + "_R90" + extension).toStdString(), imageRotate90 );
            imwrite((destinationPath + "/" + fileName + number + "_R180" + extension).toStdString(), imageRotate180 );
            imwrite((destinationPath + "/" + fileName + number + "_R270" + extension).toStdString(), imageRotate270 );

            LogImgDataset::getInstance().Log(INFO, "Rotate images: 0 - 90 - 180 - 270");

            if (mainW->bFlipChecked)
            {
                //Vertical Flip in Rotate Images
                Mat imageFlipV0 = flipImage(imageRotate0, 1);
                Mat imageFlipV90 = flipImage(imageRotate90, 1);
                Mat imageFlipV180 = flipImage(imageRotate180, 1);
                Mat imageFlipV270 = flipImage(imageRotate270, 1);

                imwrite((destinationPath + "/" + fileName + number + "_FlipV0" + extension).toStdString(), imageFlipV0 );
                imwrite((destinationPath + "/" + fileName + number + "_FlipV90" + extension).toStdString(), imageFlipV90 );
                imwrite((destinationPath + "/" + fileName + number + "_FlipV180" + extension).toStdString(), imageFlipV180 );
                imwrite((destinationPath + "/" + fileName + number + "_FlipV270" + extension).toStdString(), imageFlipV270 );

                LogImgDataset::getInstance().Log(INFO, "Fliping ...");
            }

        }
        else if (mainW->bFlipChecked)
        {
            //Vertical Flip in Rotate Images
            Mat imageFlipV0 = flipImage(imageRotate0, 1);
            imwrite((destinationPath + "/" + fileName + number + "_FlipV0" + ".jpg").toStdString(), imageFlipV0 );

            LogImgDataset::getInstance().Log(INFO, "Fliping ...");

        }

    }
}

void ImageTransformation::thinPlateSplineProcessing(Mat originalImage, QString destinationPath, QString fileName, std::vector<cv::Point> iP, std::vector<cv::Point> iiP)
{
    CThinPlateSpline *tps = NULL;

    Mat thinPlateSplineImage;

    // Create thin plate spline object and put the vectors into the constructor
    tps= new CThinPlateSpline(iP,iiP);

    // Warp the image to dst
    tps->warpImage(originalImage,thinPlateSplineImage,0.01,INTER_CUBIC,BACK_WARP);

    imwrite((destinationPath + fileName + "_TPS" + ".jpg").toStdString(), thinPlateSplineImage );

    delete tps;
}




