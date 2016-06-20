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

void ImageTransformation::writeImage(Mat imageBase, QString destinationPath, QString fileName, QString extension, MainWindow* mainW)
{
    Mat imageTemp;
    imageBase.copyTo(imageTemp);
    imwrite((destinationPath + "/" + thinPlateSplinesImagesPath + fileName + extension).toStdString(), imageTemp );
    LogImgDataset::getInstance().Log(INFO, "Thin Plate Splines image");
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
        resize(imageTemp,imageTemp,Size(width,height), 0, 0, INTER_CUBIC);

        imwrite((destinationPath + "/" + resizesPath + fileName + extension).toStdString(), imageTemp );

        LogImgDataset::getInstance().Log(INFO, "Resize image");
    }

    if (mainW->bRotateChecked)
    {
        //Rotate Images 0 - 90 - 180 -270
        imageRotate0 = imageTemp;
        imageRotate90 = rotateImage(imageTemp, 90.0);
        imageRotate180 = rotateImage(imageTemp, 180.0);
        imageRotate270 = rotateImage(imageTemp, 270.0);

        imwrite((destinationPath + "/" + rotateImagesPath + fileName + extension).toStdString(), imageRotate0 );
        imwrite((destinationPath + "/" + rotateImagesPath + fileName + "_R90" + extension).toStdString(), imageRotate90 );
        imwrite((destinationPath + "/" + rotateImagesPath + fileName + "_R180" + extension).toStdString(), imageRotate180 );
        imwrite((destinationPath + "/" + rotateImagesPath + fileName + "_R270" + extension).toStdString(), imageRotate270 );

        LogImgDataset::getInstance().Log(INFO, "Rotate images: 0 - 90 - 180 - 270");

        if (mainW->bFlipChecked)
        {
            //Vertical Flip in Rotate Images
            Mat imageFlipV0 = flipImage(imageRotate0, 1);
            Mat imageFlipV90 = flipImage(imageRotate90, 1);
            Mat imageFlipV180 = flipImage(imageRotate180, 1);
            Mat imageFlipV270 = flipImage(imageRotate270, 1);

            imwrite((destinationPath + "/" + rotateImagesPath + fileName + "_F0" + extension).toStdString(), imageFlipV0 );
            imwrite((destinationPath + "/" + rotateImagesPath + fileName + "_F90" + extension).toStdString(), imageFlipV90 );
            imwrite((destinationPath + "/" + rotateImagesPath + fileName + "_F180" + extension).toStdString(), imageFlipV180 );
            imwrite((destinationPath + "/" + rotateImagesPath + fileName + "_F270" + extension).toStdString(), imageFlipV270 );

            LogImgDataset::getInstance().Log(INFO, "Fliping ...");
        }

    }
    else if (mainW->bFlipChecked)
    {
        //Vertical Flip in Rotate Images
        Mat imageFlipV0 = flipImage(imageRotate0, 1);
        imwrite((destinationPath + "/" + rotateImagesPath + fileName + "_F0" + extension).toStdString(), imageFlipV0 );

        LogImgDataset::getInstance().Log(INFO, "Fliping ...");
    }

}

void ImageTransformation::writeImages(vector<coordinateInfo> coordinates, Mat imageBase, QString destinationPath, QString fileName, QString extension, MainWindow* mainW)
{

    ///Crop the interest area
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
            resize(imageTemp,imageTemp,Size(iWidth,iHeight), 0, 0, INTER_CUBIC);

            imwrite((destinationPath + "/" + resizesPath + fileName + extension).toStdString(), imageTemp );

            LogImgDataset::getInstance().Log(INFO, "Resize image");
        }

        if (mainW->bCropChecked)
        {

            if (mainW->iWindowSize < imageTemp.cols && mainW->iWindowSize < imageTemp.rows)
            {
                if ( coordinates[j].x <= imageTemp.cols && coordinates[j].y <= imageTemp.rows)
                {
                    //Recorte da imagem a partir dos dados dos CSVs
                    QString number = "_" + QString::number(j);
                    Mat imageCrop = cropImage(imageBase, Point(coordinates[j].x,coordinates[j].y), cv::Size(mainW->iWindowSize,mainW->iWindowSize), mainW->iOffset);
                    imwrite((destinationPath + "/" + cropPath + fileName + number + extension).toStdString(), imageCrop);
                    imageTemp = imageCrop;

                    LogImgDataset::getInstance().Log(INFO, "Crop image");
                } else
                {
                    LogImgDataset::getInstance().Log(ERROR, "Crop image failed");
                    LogImgDataset::getInstance().Log(ERROR, "The crop is out of range of the image");
                }
            } else
            {
                LogImgDataset::getInstance().Log(ERROR, "Crop image failed");
                LogImgDataset::getInstance().Log(ERROR, "The window size is bigger than image size.");
            }

        }

        if (mainW->bRotateChecked)
        {
            //Rotate Images 0 - 90 - 180 -270
            imageRotate0 = imageTemp;
            imageRotate90 = rotateImage(imageTemp, 90.0);
            imageRotate180 = rotateImage(imageTemp, 180.0);
            imageRotate270 = rotateImage(imageTemp, 270.0);

            imwrite((destinationPath + "/" + rotateImagesPath + fileName + number + extension).toStdString(), imageRotate0 );
            imwrite((destinationPath + "/" + rotateImagesPath +  fileName + number + "_R90" + extension).toStdString(), imageRotate90 );
            imwrite((destinationPath + "/" + rotateImagesPath +  fileName + number + "_R180" + extension).toStdString(), imageRotate180 );
            imwrite((destinationPath + "/" + rotateImagesPath +  fileName + number + "_R270" + extension).toStdString(), imageRotate270 );

            LogImgDataset::getInstance().Log(INFO, "Rotate images: 0 - 90 - 180 - 270");

            if (mainW->bFlipChecked)
            {
                //Vertical Flip in Rotate Images
                Mat imageFlipV0 = flipImage(imageRotate0, 1);
                Mat imageFlipV90 = flipImage(imageRotate90, 1);
                Mat imageFlipV180 = flipImage(imageRotate180, 1);
                Mat imageFlipV270 = flipImage(imageRotate270, 1);

                imwrite((destinationPath + "/" + rotateImagesPath +  fileName + number + "_F0" + extension).toStdString(), imageFlipV0 );
                imwrite((destinationPath + "/" + rotateImagesPath +  fileName + number + "_F90" + extension).toStdString(), imageFlipV90 );
                imwrite((destinationPath + "/" + rotateImagesPath +  fileName + number + "_F180" + extension).toStdString(), imageFlipV180 );
                imwrite((destinationPath + "/" + rotateImagesPath +  fileName + number + "_F270" + extension).toStdString(), imageFlipV270 );

                LogImgDataset::getInstance().Log(INFO, "Fliping ...");
            }

        }
        else if (mainW->bFlipChecked)
        {
            //Vertical Flip in Rotate Images
            Mat imageFlipV0 = flipImage(imageRotate0, 1);
            imwrite((destinationPath + "/" + rotateImagesPath +  fileName + number + "_F0" + extension).toStdString(), imageFlipV0 );

            LogImgDataset::getInstance().Log(INFO, "Fliping ...");

        }

    }
}

void ImageTransformation::thinPlateSplineProcessing(Mat originalImage, QString destinationPath, QString fileName, std::vector<cv::Point> iP, std::vector<cv::Point> iiP)
{
    CThinPlateSpline *tps = NULL;

    Mat thinPlateSplineImage;

    /// Create thin plate spline object and put the vectors into the constructor
    tps= new CThinPlateSpline(iP,iiP);

    /// Warp the image to dst
    tps->warpImage(originalImage,thinPlateSplineImage,0.01,INTER_CUBIC,BACK_WARP);

    //imwrite((destinationPath + fileName + "_TPS" + extension).toStdString(), thinPlateSplineImage );

    delete tps;
}

Mat ImageTransformation::thinPlateSplineProcessing(Mat originalImage, Mat thinPlateSplineImage, std::vector<cv::Point> iP, std::vector<cv::Point> iiP)
{

    CThinPlateSpline *tps = NULL;

//    int w = originalImage.cols;
//    int h = originalImage.rows;

    /// Create thin plate spline object and put the vectors into the constructor
    tps= new CThinPlateSpline(iP,iiP);

    //pointsRandomTransformInLayers(iP, iiP, w, h);

    /// Warp the image to dst
    tps->warpImage(originalImage,thinPlateSplineImage,0.01,INTER_CUBIC,BACK_WARP);

    //Mat mapX, mapY;
    //tps->getMaps(mapX, mapY);

    //vector<cv::Point> pSource = getPointsRandomTransformation(pSource, mapY, mapX);

    delete tps;

    return thinPlateSplineImage;

}

vector<Point> ImageTransformation::generateGrid(Mat originalImage, int cols, int rows, float inc=0)
{
    vector<Point> pointVector;
    int colspacing = originalImage.cols / cols;
    int rowspacing = originalImage.rows / rows;


    Point tmpPoint = Point(0, 0);

    for (int i = 0; i < cols; ++i )
    {
        tmpPoint.x = tmpPoint.x + colspacing + inc;
        for (int j = 0; j < rows; ++j)
        {
            tmpPoint.y = tmpPoint.y + rowspacing + inc;
            pointVector.push_back(tmpPoint);
        }
    }

    return pointVector;
}


int ImageTransformation::pointsRandomTransformInLayers(vector<cv::Point> iP, vector<cv::Point> &iiP, int w, int h){

    int npts = 0;
    double A[C_NLAYERS][2][3] ;
    double  orig_x = w/2.0,  orig_y= h/2.0;
    double scale_x, scale_y, thetaRad, tx, ty;

    random2DAffineTransformParams(C_SCALE_MIN, C_SCALE_MAX, scale_x, scale_y, C_ROT_MIN, C_ROT_MAX, thetaRad, C_TRANS_MIN, C_TRANS_MAX, tx,ty);//, orig_x, orig_y);

    for(int i =0; i<C_NLAYERS; i++){
        param2matrix_2DAffineTransform(A[i], scale_x, scale_y, (i+1.0)*thetaRad/C_NLAYERS, (i+1.0)*tx/C_NLAYERS, (i+1.0)*ty/C_NLAYERS, orig_x, orig_y);
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
    printf("Num points transformed: %d\n", npts );

    return 0;
}

void ImageTransformation::random2DAffineTransformParams( const double scaleMin, const double scaleMax, double &scale_x, double &scale_y, const int rotMin, const int rotMax, double &thetaRad, const double transMin, const double transMax, double &tx, double &ty)
{
    ///Ramdom generates theta within the range min max
    int thetaGrades = (rand() % (1+ rotMax-rotMin) ) + rotMin;
    thetaRad = thetaGrades* PIover180 ; //*PI)/180.0;


    tx= (rotMax-rotMin) * (rand()/(double)RAND_MAX) + rotMin;
    ty= (rotMax-rotMin) * (rand()/(double)RAND_MAX) + rotMin;



    scale_x = (scaleMax-scaleMin) * (rand()/(double)RAND_MAX) + scaleMin;
    scale_y = (scaleMax-scaleMin) * (rand()/(double)RAND_MAX) + scaleMin;

}

void ImageTransformation::param2matrix_2DAffineTransform(double Aff[2][3], double scale_x, double scale_y, double thetaRad, double tx, double  ty, const double orig_x, const double orig_y)
{
    double cosT= cos(thetaRad);
    double sinT= sin(thetaRad);


    Aff[0][0] = scale_x	* cosT;
    Aff[1][0] = scale_x * sinT;


    Aff[0][1] = -scale_y* sinT;
    Aff[1][1] = scale_y	* cosT;

    Aff[0][2] = tx*scale_x	*cosT - ty*scale_y*sinT ;
    Aff[1][2] = tx*scale_x	*sinT + ty*scale_y*cosT ;
}

Mat ImageTransformation::cropROI(vector<Point> points, Mat image)
{
    /// From the points, figure out the size of the ROI
    int left, right, top, bottom;
    for (int i = 0; i < points.size(); i++)
    {
        if (i == 0) /// initialize corner values
        {
            left = right = points[i].x;
            top = bottom = points[i].y;
        }

        if (points[i].x < left)
            left = points[i].x;

        if (points[i].x > right)
            right = points[i].x;

        if (points[i].y < top)
            top = points[i].y;

        if (points[i].y > bottom)
            bottom = points[i].y;
    }
    std::vector<cv::Point> box_points;
    box_points.clear();
    box_points.push_back(cv::Point(left, top));
    box_points.push_back(cv::Point(left, bottom));
    box_points.push_back(cv::Point(right, bottom));
    box_points.push_back(cv::Point(right, top));

    /// Compute minimal bounding box for the ROI
    /// Note: for some unknown reason, width/height of the box are switched.
    cv::RotatedRect box = cv::minAreaRect(cv::Mat(box_points));
    //std::cout << "box w:" << box.size.width << " h:" << box.size.height << std::endl;

    /// Set the ROI to the area defined by the box
    /// Note: because the width/height of the box are switched,
    /// they were switched manually in the code below:
    cv::Rect roi;
    roi.x = box.center.x - (box.size.height / 2);
    roi.y = box.center.y - (box.size.width / 2);
    roi.width = box.size.height;
    roi.height = box.size.width;
    //std::cout << "roi @ " << roi.x << "," << roi.y << " " << roi.width << "x" << roi.height << std::endl;

    /// Crop the original image to the defined ROI
    cv::Mat crop = image(roi);

    return crop;
}

Point ImageTransformation::centerCropROI(vector<Point> points)
{
    /// From the points, figure out the size of the ROI
    int left, right, top, bottom;
    for (int i = 0; i < points.size(); i++)
    {
        if (i == 0) /// initialize corner values
        {
            left = right = points[i].x;
            top = bottom = points[i].y;
        }

        if (points[i].x < left)
            left = points[i].x;

        if (points[i].x > right)
            right = points[i].x;

        if (points[i].y < top)
            top = points[i].y;

        if (points[i].y > bottom)
            bottom = points[i].y;
    }
    std::vector<cv::Point> box_points;
    box_points.clear();
    box_points.push_back(cv::Point(left, top));
    box_points.push_back(cv::Point(left, bottom));
    box_points.push_back(cv::Point(right, bottom));
    box_points.push_back(cv::Point(right, top));

    /// Compute minimal bounding box for the ROI
    /// Note: for some unknown reason, width/height of the box are switched.
    cv::RotatedRect box = cv::minAreaRect(cv::Mat(box_points));
    //std::cout << "box w:" << box.size.width << " h:" << box.size.height << std::endl;

    /// Set the ROI to the area defined by the box
    /// Note: because the width/height of the box are switched,
    /// they were switched manually in the code below:
    cv::Rect roi;
    roi.x = box.center.x - (box.size.height / 2);
    roi.y = box.center.y - (box.size.width / 2);
    roi.width = box.size.height;
    roi.height = box.size.width;
    //std::cout << "roi @ " << roi.x << "," << roi.y << " " << roi.width << "x" << roi.height << std::endl;

    cv::Point roiCenter;
    roiCenter.x = roi.x;
    roiCenter.y = roi.y;

    return roiCenter;
}

double ImageTransformation::calculateDistance(const Point& pt1, const Point& pt2)
{
    double deltaX = pt1.x - pt2.x;
    double deltaY = pt1.y - pt2.y;
    return sqrt(pow(deltaX,2) + pow(deltaY,2));
}

vector<Point> ImageTransformation::changeGrid(vector<Point> pointVector, vector<coordinateInfo> mitosis)
{

    Point tmpPoint = Point(0, 0);

    for (int l = 0; l < mitosis.size(); ++l)
    {
        int x = mitosis[l].x;
        int y = mitosis[l].y;
        tmpPoint.x = x;
        tmpPoint.y = y;

        pointVector.push_back(tmpPoint);


        int pos = std::find(pointVector.begin(), pointVector.end(), tmpPoint) - pointVector.begin();

        if (pos > 0 && pos < (int)pointVector.size()) {

            double posL1 = calculateDistance(pointVector[pos], pointVector[pos-1]);
            double posP1 = calculateDistance(pointVector[pos], pointVector[pos+1]);

            if (posL1 < posP1)
                pointVector.erase(pointVector.begin()+pos-1);
            else
                pointVector.erase(pointVector.begin()+pos+1);
        }
        else {
            if (pos == 0)
                pointVector.erase(pointVector.begin()+pos+1);
            if (pos == (int) pointVector.size())
                pointVector.erase(pointVector.begin()+pos-1);
        }

    }

    return pointVector;
}

vector<cv::Point> ImageTransformation::getPointsRandomTransformation(vector<cv::Point> iP, Mat_<float> mx, Mat_<float> my)
{
    vector<Point> iiP;

    for(int i = 0; i < iP.size(); ++i)
    {
        int x = mx(iP[i].x,iP[i].y);
        int y = my(iP[i].x,iP[i].y);
//        qDebug()<< "x:" << iP[i].x << "to" << x;
//        qDebug()<< "y:" << iP[i].y << "to" << y;
        Point dPoint(x,y);
        iiP.push_back(dPoint);
    }

    return iiP;
}
