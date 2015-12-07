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
void randomReflection();

void random2DAffineTransformParams( const double scaleMin, const double scaleMax, double &scale_x, double &scale_y, const int rotMin, const int rotMax, double &thetaRad, const double transMin, const double transMax, double &tx, double &ty);

//random 2d affine transform generator
//intervalos iguais para x e y
void random2DAffineTransform(double Aff[2][3], const double scaleMin, const double scaleMax,const int rotMin, const int rotMax, const double transMin, const double transMax, const double orig_x, const double orig_y);

void param2matrix_2DAffineTransform(double Aff[2][3], double scale_x, double scale_y, double thetaRad, double tx, double  ty, const double orig_x, const double orig_y);

int pointsRandomTransform(vector<cv::Point> iP, vector<cv::Point> &iiP, int w, int h);

int pointsRandomTransformInLayers(vector<cv::Point> iP, vector<cv::Point> &iiP, int w, int h);

int createSamplePoints(std::vector<cv::Point> &iP, const int n, const int w, const int h);

void thinPlateSplineTransform(Mat originalImage, QString destinyPath, QString fileName, QString number);

Mat thinPlateSplineProcessing(Mat originalImage, Mat thinPlateSplineImage, std::vector<cv::Point> iP, std::vector<cv::Point> iiP);



#endif // THINPLATESPLINE

