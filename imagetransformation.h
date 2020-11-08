#ifndef IMAGETRANSFORMATION_H
#define IMAGETRANSFORMATION_H

#include <QObject>
#include <QImage>

#include <opencv2/opencv.hpp>
#include <vector>

#include "mainwindow.h"

#include "CThinPlateSpline.h"
#include "constants.h"

/**
 * @brief Struct to define a coordinate extracted from a CSV file.
 */
struct coordinateInfo {
    double x; /*!< Interest region x coordinate. */
    double y; /*!< Interest region y coordinate. */
    double grade; /*!< Interest region grade. */
};

using namespace cv;
using namespace std;


/**
 * @file imagetransformation.h
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
 *
 */

/**
 * @class ImageTransformation
 * @brief The ImageTransformation class - The class ImageTransformation represents the transformations realized with images.
 */
class ImageTransformation : public QImage
{
    public:

        vector<coordinateInfo> coordinates; /*!< Interest area coordinates. */

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
         * @param destinationPath - Destination path.
         * @param fileName - Filename.
         * @param iP - Points Vector.
         * @param iiP - Points Vector.
         */
        void thinPlateSplineProcessing(Mat originalImage, QString destinationPath, QString fileName, std::vector<cv::Point> iP, std::vector<cv::Point> iiP);
        Mat thinPlateSplineProcessing(Mat originalImage, Mat thinPlateSplineImage, std::vector<cv::Point> iP, std::vector<cv::Point> iiP);

        /**
         * @brief writeImage - Write transformed image.
         * @param imageBase - Original image.
         * @param destinationPath - Destination path.
         * @param fileName - Filename.
         * @param extension - File extension.
         * @param mainW - MainWindow.
         */
        void writeImage(Mat imageBase, QString destinationPath, QString fileName, QString extension, MainWindow* mainW);

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
        void writeImages(vector<coordinateInfo> coordinates, Mat imageBase, QString destinationPath, QString fileName, QString extension, MainWindow* mainW);

        /**
         * @brief generateGrid Generate Grid for deformation
         * @param originalImage Original Image
         * @param cols Image columns size
         * @param rows Image rows size
         * @param inc pixel shift value
         * @return Return a vector of points
         */
        vector<Point> generateGrid(Mat originalImage, int cols, int rows, float inc);

        /**
         * @brief random2DAffineTransformParams
         * @param scaleMin
         * @param scaleMax
         * @param scale_x
         * @param scale_y
         * @param rotMin
         * @param rotMax
         * @param thetaRad
         * @param transMin
         * @param transMax
         * @param tx
         * @param ty
         */
        void random2DAffineTransformParams( const double scaleMin, const double scaleMax, double &scale_x, double &scale_y, const int rotMin, const int rotMax, double &thetaRad, const double transMin, const double transMax, double &tx, double &ty);

        /**
         * @brief param2matrix_2DAffineTransform
         * @param Aff
         * @param scale_x
         * @param scale_y
         * @param thetaRad
         * @param tx
         * @param ty
         * @param orig_x
         * @param orig_y
         */
        void param2matrix_2DAffineTransform(double Aff[2][3], double scale_x, double scale_y, double thetaRad, double tx, double  ty, const double orig_x, const double orig_y);

        /**
         * @brief pointsRandomTransformInLayers
         * @param iP
         * @param iiP
         * @param w
         * @param h
         * @return
         */
        int pointsRandomTransformInLayers(vector<cv::Point> iP, vector<cv::Point> &iiP, int w, int h);

        /**
         * @brief cropROI
         * @param points
         * @param image
         * @return
         */
        Mat cropROI(vector<Point> points, Mat image);

        /**
         * @brief centerCropROI
         * @param points
         * @return
         */
        Point centerCropROI(vector<Point> points);

        /**
         * @brief calculateDistance
         * @param pt1
         * @param pt2
         * @return
         */
        double calculateDistance(const Point& pt1, const Point& pt2);

        /**
         * @brief changeGrid
         * @param pointVector
         * @param mitosis
         * @return
         */
        vector<Point> changeGrid(vector<Point> pointVector, vector<coordinateInfo> mitosis);

        /**
         * @brief getPointsRandomTransformation
         * @param iP
         * @param mx
         * @param my
         * @return
         */
        vector<cv::Point> getPointsRandomTransformation(vector<cv::Point> iP, Mat_<float> mx, Mat_<float> my);


    private:
        Point coordinate; /*!< Point coordinate. */
        Size rectSize; /*!< Rect size. */
        int offset; /*!< Offset to adjust size. */

};


#endif // IMAGETRANSFORMATION_H
