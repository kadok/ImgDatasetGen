// ThinPlateSpline.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "ThinPlateSpline.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>		// sin cos

using namespace std;

//https://en.wikipedia.org/wiki/Transformation_matrix


//produces reflections around a line
void randomReflection(){

}
 
void random2DAffineTransformParams( const double scaleMin, const double scaleMax, double &scale_x, double &scale_y,
							 //rot min and max in grades:
							 const int rotMin, const int rotMax, double &thetaRad ,
							 const double transMin, const double transMax, double &tx, double &ty){

	//ramdom generates theta within the range min max
	int thetaGrades = (rand() % (1+ rotMax-rotMin) ) + rotMin;
	thetaRad = thetaGrades* PIover180 ; //*PI)/180.0;
	
	
	tx= (rotMax-rotMin) * (rand()/(double)RAND_MAX) + rotMin;
	ty= (rotMax-rotMin) * (rand()/(double)RAND_MAX) + rotMin;


	
	scale_x = (scaleMax-scaleMin) * (rand()/(double)RAND_MAX) + scaleMin;
	scale_y = (scaleMax-scaleMin) * (rand()/(double)RAND_MAX) + scaleMin;

}

//random 2d affine transform generator
//intervalos iguais para x e y		 
void random2DAffineTransform(double Aff[2][3] ,
							 const double scaleMin, const double scaleMax, 
							 //in grades:
							 const int rotMin, const int rotMax, 
							 const double transMin, const double transMax, 
							 const double orig_x, const double orig_y){

	double cosT, sinT;
	double	scale_x=0.0, scale_y=0.0, thetaRad =0.0, tx,ty;

	random2DAffineTransformParams(scaleMin, scaleMax, scale_x, scale_y, 
		rotMin, rotMax, thetaRad, 
		transMin, transMax, tx, ty);
	
	cosT= cos(thetaRad);
	sinT= sin(thetaRad);
	

	Aff[0][0] = scale_x	* cosT;
	Aff[1][0] = scale_x * sinT;
	
	
	Aff[0][1] = -scale_y* sinT;
	Aff[1][1] = scale_y	* cosT;

	Aff[0][2] = tx*scale_x	*cosT - ty*scale_y*sinT ;
	Aff[1][2] = tx*scale_x	*sinT + ty*scale_y*cosT ;
}

void param2matrix_2DAffineTransform(double Aff[2][3] ,
						double scale_x, double scale_y, double thetaRad, double tx, double  ty,
							 const double orig_x, const double orig_y){

	double cosT= cos(thetaRad);
	double sinT= sin(thetaRad);
	

	Aff[0][0] = scale_x	* cosT;
	Aff[1][0] = scale_x * sinT;
	
	
	Aff[0][1] = -scale_y* sinT;
	Aff[1][1] = scale_y	* cosT;

	Aff[0][2] = tx*scale_x	*cosT - ty*scale_y*sinT ;
	Aff[1][2] = tx*scale_x	*sinT + ty*scale_y*cosT ;
}

int pointsRandomTransform(vector<cv::Point> iP, vector<cv::Point> &iiP, int w, int h){

	int npts = 0;
	double A[2][3] ;
	double  orig_x = w/2.0,  orig_y= h/2.0;
	
	random2DAffineTransform(A,
							 0.9, 1.1, 
							 //in grades:
							 -15, 15,
							 -10, +10, orig_x, orig_y);

	iiP.clear();

	int x,y, xf, yf;
	vector<cv::Point>::iterator itPts = iP.begin();
	for (; itPts!= iP.end(); ++itPts) {
		x = itPts->x - orig_x;
		y = itPts->y - orig_y;
		xf = x*A[0][0] +y*A[0][1] + A[0][2];
		yf = x*A[1][0] +y*A[1][1] + A[1][2];
	
		iiP.push_back(cv::Point(xf+orig_x,yf +orig_y)  );
		npts ++;
	}
    //printf("Num points transformed: %d", npts );

	return 0;
}

int pointsRandomTransformInLayers(vector<cv::Point> iP, vector<cv::Point> &iiP, int w, int h){

	int npts = 0;
	double A[C_NLAYERS][2][3] ;
	double  orig_x = w/2.0,  orig_y= h/2.0;
	double scale_x, scale_y, thetaRad, tx, ty;

	random2DAffineTransformParams(
							 C_SCALE_MIN, C_SCALE_MAX,
							 scale_x, scale_y,
							 C_ROT_MIN, C_ROT_MAX, 
							 thetaRad,
							 C_TRANS_MIN, C_TRANS_MAX,
							 tx,ty);//, orig_x, orig_y);
	
	for(int i =0; i<C_NLAYERS; i++){
		param2matrix_2DAffineTransform(A[i], scale_x, scale_y, 
			(i+1.0)*thetaRad/C_NLAYERS, (i+1.0)*tx/C_NLAYERS, (i+1.0)*ty/C_NLAYERS, orig_x, orig_y);
	}

	iiP.clear();

	int x,y, xf, yf;
	vector<cv::Point>::iterator itPts = iP.begin();
	for (; itPts!= iP.end(); ++itPts) {
		x = itPts->x - orig_x;
		y = itPts->y - orig_y;

		float dist= sqrt((float)(x*x+ y*y));
		dist =  2*dist /(orig_x+orig_y);
		int i = min(C_NLAYERS-1,   (int)(dist * C_NLAYERS));
		xf = x*A[i][0][0] +y*A[i][0][1] + A[i][0][2];
		yf = x*A[i][1][0] +y*A[i][1][1] + A[i][1][2];
	
		iiP.push_back(cv::Point(xf+orig_x,yf +orig_y)  );
		npts ++;
        //printf("A[%d] ", i);
	}
    //printf("Num points transformed: %d\n", npts );

	return 0;
}

int createSamplePoints(std::vector<cv::Point> &iP, const int n, const int w, const int h){

	float dx = w/(n+1.0);
	float dy = h/(n+1.0);

	iP.clear();


	for(float y = dy; y<h; y+= dy+0.5)
		for(float x = dx; x<w; x+= dx+0.5){
			iP.push_back(cv::Point((int)x,(int)y));
		}

	return 0;
}

void thinPlateSplineTransform(Mat originalImage, QString destinyPath, QString fileName, QString number)
{
	srand (time(NULL));

	// generate some generic points
	// usually you would use a interest point detector such as SURF or SIFT
	std::vector<cv::Point> iP, iiP;

    int w = originalImage.cols;
    int h = originalImage.rows;
	createSamplePoints(iP, N_SAMPLES_1D, w, h);
	
    Mat ThinPlateSplineImage;
	CThinPlateSpline *tps = NULL;

    //pointsRandomTransform
    pointsRandomTransformInLayers(iP, iiP, w, h);

    // create thin plate spline object and put the vectors into the constructor
    tps= new CThinPlateSpline(iP,iiP);

    // warp the image to dst
    tps->warpImage(originalImage,ThinPlateSplineImage,0.01,INTER_CUBIC,BACK_WARP);

    imwrite((destinyPath + fileName + number + "_TPS" + ".jpg").toStdString(), ThinPlateSplineImage );

    delete tps;

}

Mat thinPlateSplineProcessing(Mat originalImage, Mat thinPlateSplineImage, std::vector<cv::Point> iP, std::vector<cv::Point> iiP)
{

    CThinPlateSpline *tps = NULL;

    // create thin plate spline object and put the vectors into the constructor
    tps= new CThinPlateSpline(iP,iiP);

    // warp the image to dst
    tps->warpImage(originalImage,thinPlateSplineImage,0.01,INTER_CUBIC,BACK_WARP);

    delete tps;

    return thinPlateSplineImage;

}

