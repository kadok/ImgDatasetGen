#ifndef FILEMANIPULATION_H
#define FILEMANIPULATION_H

#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>

#include "imagetransformation.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


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
 * The class FileManipulation represents the manipulations realized with files and directories.
 */
class FileManipulation : public QDir
{
    public:
        /**
         * @brief FileManipulation - Constructor.
         */
        FileManipulation();

        /**
         * @brief removeExtension - Remove the file extension from filename.
         * @param fileName - Filename.
         * @return Return the filename without the file extension.
         */
        inline QString removeExtension(const QString & fileName) {
            return fileName.left(fileName.lastIndexOf("."));
        }

        /**
         * @brief getExtension - Get the file extension.
         * @param fileName - Filename.
         * @return Return the file extension.
         */
        inline QString getExtension(const QString & fileName) {
            return fileName.mid(fileName.lastIndexOf("."),fileName.length());
        }

        /**
         * @brief csvReaderPoints - Read CSV file to getting defined points.
         * @param fileName - Filename.
         * @param separator - Separator.
         * @return Return a vector with points found on CSV file.
         */
        Vector<Point> csvReaderPoints(QString fileName, const QString separator);

        /**
         * @brief csvReaderCoordinates - Read CSV file to getting the image coordinates.
         * @param fileName - Filename.
         * @param separator - Separator.
         * @return Return a vector with coordinates found on CSV file.
         */
        Vector<coordinateInfo> csvReaderCoordinates(QString fileName, const QString separator);

        /**
         * @brief csvReaderVectorPoints - Read CSV file to getting especific coordinates points.
         * @param fileName - Filename.
         * @param separator - Separator.
         * @return Return a vector with vector of especific coordinates points.
         */
        Vector<Vector<Point> > csvReaderVectorPoints(QString fileName, const QString separator);

        /**
         * @brief checkDirectory - Check directory.
         * @param dir - Directory.
         * @return Return boolean.
         */
        bool checkDirectory(QString dir);

        /**
         * @brief checkDirectoryAndCreate - Check directory and if not exists can create it.
         * @param dir - Directory.
         * @return Return boolean.
         */
        bool checkDirectoryAndCreate(QString dir, QWidget* widget);

    private:
        QString fileExtension;
        QString fileName;


};

#endif // FILEMANIPULATION_H
