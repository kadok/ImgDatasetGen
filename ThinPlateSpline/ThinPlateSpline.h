#ifndef THINPLATESPLINE
#define THINPLATESPLINE

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "CThinPlateSpline.h"
#include <QString>

#include "constants.h"

using namespace std;

//https://en.wikipedia.org/wiki/Transformation_matrix

//produces reflections around a line
/**
 * @brief randomReflection
 */
void randomReflection();
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

//random 2d affine transform generator
//intervalos iguais para x e y
/**
 * @brief random2DAffineTransform
 * @param Aff
 * @param scaleMin
 * @param scaleMax
 * @param rotMin
 * @param rotMax
 * @param transMin
 * @param transMax
 * @param orig_x
 * @param orig_y
 */
void random2DAffineTransform(double Aff[2][3], const double scaleMin, const double scaleMax,const int rotMin, const int rotMax, const double transMin, const double transMax, const double orig_x, const double orig_y);

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
 * @brief pointsRandomTransform
 * @param iP
 * @param iiP
 * @param w
 * @param h
 * @return
 */
int pointsRandomTransform(vector<cv::Point> iP, vector<cv::Point> &iiP, int w, int h);

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
 * @brief createSamplePoints
 * @param iP
 * @param n
 * @param w
 * @param h
 * @return
 */
int createSamplePoints(std::vector<cv::Point> &iP, const int n, const int w, const int h);

/**
 * @brief thinPlateSplineTransform
 * @param originalImage
 * @param destinyPath
 * @param fileName
 * @param number
 */
void thinPlateSplineTransform(Mat originalImage, QString destinyPath, QString fileName, QString number);

/**
 * @brief thinPlateSplineProcessing
 * @param originalImage
 * @param thinPlateSplineImage
 * @param iP
 * @param iiP
 * @return
 */
Mat thinPlateSplineProcessing(Mat originalImage, Mat thinPlateSplineImage, std::vector<cv::Point> iP, std::vector<cv::Point> iiP);

#endif // THINPLATESPLINE

