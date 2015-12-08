#include "filemanipulation.h"
#include <stdio.h>
#include <iostream>

FileManipulation::FileManipulation()
{

}

Vector<Point> FileManipulation::csvReaderPoints(QString fileName, const QString separator)
{
    Vector<Point> pointVector;
    pointVector.clear();

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);

        while( !textStream.atEnd() )
        {
            QString line = textStream.readLine();
            QStringList values = line.split(separator);

            double x = values[0].toDouble();
            double y = values[1].toDouble();

            Point imagePoint(x,y);
            pointVector.push_back(imagePoint);

        }
    }

    return pointVector;

}

Vector<coordinateInfo> FileManipulation::csvReaderCoordinates(QString fileName, const QString separator)
{
    Vector<coordinateInfo> pointVector;
    pointVector.clear();

    coordinateInfo mCoordinates;

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);

        while( !textStream.atEnd() )
        {
            QString line = textStream.readLine();
            QStringList values = line.split(separator);

            mCoordinates.x = values[0].toDouble();
            mCoordinates.y = values[1].toDouble();
            mCoordinates.grade = values[2].toDouble();

            pointVector.push_back(mCoordinates);

        }
    }

    return pointVector;

}

Vector<Vector<Point> > FileManipulation::csvReaderVectorPoints(QString fileName, const QString separator)
{
    Vector<Vector<Point> > vectorPointVector;
    Vector<Point> pointVector;
    vectorPointVector.clear();
    pointVector.clear();

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);

        while( !textStream.atEnd() )
        {
            QString line = textStream.readLine();
            QStringList values = line.split(separator);

            int vectorSize = values.size()/2;
            int vectorPairControl = 0;

            for (int i=0; i < values.size(); i++)
            {
                if (vectorPairControl < vectorSize) {

                    double x = values[i].toDouble();
                    i++;
                    double y = values[i].toDouble();

                    Point imagePoint(x,y);
                    pointVector.push_back(imagePoint);

                    vectorPairControl++;

                } else {

                    continue;

                }

            }

            vectorPointVector.push_back(pointVector);

        }
    }

    return vectorPointVector;

}

bool FileManipulation::checkDirectory(QString dir)
{
    bool exists = true;
    QDir directory(dir);

    if (!directory.exists()) {

        QMessageBox msgBox;
        msgBox.setText("The directory "+ dir + " not exist.");
        msgBox.exec();

        exists = false;
        return exists;

    }

    return exists;

}

bool FileManipulation::checkDirectoryAndCreate(QString dir, QWidget* widget)
{
    bool exists = true;

    QDir directory(dir);
    if (!directory.exists()) {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(widget, "Directory not exist.", "The directory"+ dir + " not exist. Do you want create it?", QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {

            directory.mkdir(dir);

        } else {

            exists = false;
            QMessageBox msgBox;
            msgBox.setText("The destination directory not exist. Use a valid destination directory");
            msgBox.exec();

        }
    }

    return exists;

}
