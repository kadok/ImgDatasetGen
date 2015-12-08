#ifndef IMAGETRANSFORMATION_H
#define IMAGETRANSFORMATION_H

#include <QObject>
#include <QImage>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "mainwindow.h"

#include "ThinPlateSpline/CThinPlateSpline.h"
#include "ThinPlateSpline/constants.h"

///Struct to define a coordinate extracted from a CSV file.
struct coordinateInfo {
    double x;
    double y;
    double grade;
};

using namespace cv;


/**
 * @file
 * @author  Renato Moraes <rmoraes@inf.puc-rio.br>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The class ImageTransformation represents the transformations realized with images.
 */
class ImageTransformation : public QImage
{
    public:
        /**
         * @brief ImageTransformation - Constructor.
         */
        ImageTransformation();

        /**
         * @brief Destructor
         */
        virtual ~ImageTransformation();

        /**
         * @brief flipImage - Flip the image or matrix on Y axis.
         * @param originalImage - Original image.
         * @param flipCode - A flag to specify how to flip the array; 0 means flipping around the x-axis and positive value (for example, 1) means flipping around y-axis. Negative value (for example, -1) means flipping around both axes (see the discussion below for the formulas).
         * @return Return the image or matrix flipped.
         */
        Mat flipImage(Mat originalImage, int flipCode);

        /**
         * @brief rotateImage - Rotate the image or matrix for an especific angle.
         * @param originalImage - Original image.
         * @param angle - Angle for rotation.
         * @return Return the image or matrix rotated.
         */
        Mat rotateImage(Mat originalImage, double angle);

        /**
         * @brief cropImage - Crop a region on the image or matrix using the coordinates.
         * @param original - Original image.
         * @param coordinate - Coordinates for crop, center pixel.
         * @param rectSize - Rectangle size, window size for crop.
         * @param offset - Fine adjust pixels for crop.
         * @return Return the image or matrix cropped.
         */
        Mat cropImage(Mat original, Point coordinate, Size rectSize, int offset);

        /**
         * @brief thinPlateSplineProcessing - Process the Thin Plate Spline on image.
         * @param originalImage - Original image.
         * @param destinantionPath - Destination path.
         * @param fileName - Filename.
         * @param iP - Points Vector.
         * @param iiP - Points Vector.
         */
        void thinPlateSplineProcessing(Mat originalImage, QString destinationPath, QString fileName, std::vector<cv::Point> iP, std::vector<cv::Point> iiP);

        /**
         * @brief writeImages - Write transformed images.
         * @param imageBase - Original image.
         * @param destinationPath - Destination path.
         * @param fileName - Filename.
         * @param extension - File extension.
         * @param mainW - MainWindow.
         */
        void writeImages(Mat imageBase, QString destinationPath, QString fileName, QString extension, MainWindow* mainW);

        /**
         * @brief writeImages - Write transformed images.
         * @param coordinates - Coordinates vector.
         * @param imageBase - Original image.
         * @param destinationPath - Destination path.
         * @param fileName - Filename.
         * @param extension - File extension.
         * @param mainW - MainWindow.
         */
        void writeImages(Vector<coordinateInfo> coordinates, Mat imageBase, QString destinationPath, QString fileName, QString extension, MainWindow* mainW);


    private:
        Point coordinate;
        Size rectSize;
        int offset;

};


#endif // IMAGETRANSFORMATION_H
