#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filemanipulation.h"
#include "imagetransformation.h"
#include "log.h"
#include "constants.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LogImgDataset::getInstance().Log(INFO,"Initializing ...");

    QApplication::setStyle(QStyleFactory::create("Plastique"));
    QApplication::setPalette(QApplication::style()->standardPalette());

    createActions();
    createMenus();

    pOriginalPath = ui->originalPathLineEdit;
    pDestinationPath = ui->destinationPathLineEdit;
    pCSVPath = ui->csvLineEdit;

    bFlipChecked = false;
    bRotateChecked = false;
    bResizeChecked = false;
    bGrayChecked = false;
    bTSPChecked = false;
    bCropChecked = false;
    bCSVChecked = false;

    QSignalMapper* signalMapper = new QSignalMapper (this) ;

    QPushButton* originalPathButton = ui->originalPathPushButton;
    connect (originalPathButton, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
    signalMapper -> setMapping (originalPathButton, ORIGINAL_PATH) ;

    QPushButton* destinationPathButton = ui->destinationPathPushButton;
    connect (destinationPathButton, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
    signalMapper -> setMapping (destinationPathButton, DESTINATION_PATH) ;

    QPushButton* csvPathButton = ui->csvPushButton;
    connect (csvPathButton, SIGNAL(clicked(bool)), signalMapper, SLOT(map())) ;
    signalMapper -> setMapping (csvPathButton, CSV_PATH) ;

    QPushButton* generatePathButton = ui->generatePushButton;
    connect (generatePathButton, SIGNAL(clicked(bool)), this, SLOT(verifyForm(bool)));

    connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(browse(int))) ;
    connect(ui->resizeCheckBox, SIGNAL(toggled(bool)), ui->widthLineEdit, SLOT(setEnabled(bool)));
    connect(ui->resizeCheckBox, SIGNAL(toggled(bool)), ui->heightLineEdit, SLOT(setEnabled(bool)));
    connect(ui->cropCheckBox, SIGNAL(toggled(bool)), ui->windowSizeLineEdit, SLOT(setEnabled(bool)));
    connect(ui->cropCheckBox, SIGNAL(toggled(bool)), ui->csvCheckBox, SLOT(setEnabled(bool)));
    connect(ui->csvCheckBox, SIGNAL(toggled(bool)), ui->csvLineEdit, SLOT(setEnabled(bool)));
    connect(ui->csvCheckBox, SIGNAL(toggled(bool)), ui->csvPushButton, SLOT(setEnabled(bool)));

}

MainWindow::~MainWindow()
{
    LogImgDataset::getInstance().Log(INFO,"Quiting ...");
    delete ui;
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Create new proccess"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newForm()));

    exitAct = new QAction(tr("Quit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Quit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setStatusTip(tr("Shows a window with information about this software"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);

    fileMenu->addSeparator();

    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Image Dataset Generator"),
                       tr("Version 1.2 - 2016 - Developed by Renato Moraes"));
}

void MainWindow::browse(int symbol)
{
    QString directory = QFileDialog::getExistingDirectory(this,
                            tr("Find Files"), QDir::currentPath());

    if (!directory.isEmpty()) {

        switch (symbol) {
            case ORIGINAL_PATH:
            {
                pOriginalPath->setText(directory);
                break;
            }
            case DESTINATION_PATH:
            {
                pDestinationPath->setText(directory);
                break;
            }
            case CSV_PATH:
            {
                pCSVPath->setText(directory);
                break;
            }
        }

    }
}

void MainWindow::generateImages()
{

    QString sourcePath = pOriginalPath->text();
    QString destinationPath = pDestinationPath->text();
    QString csvPath;

    LogImgDataset::getInstance().Log(INFO, "Starting generate images process ...");

    FileManipulation* manageFile = new FileManipulation();
    ImageTransformation* manageImage = new ImageTransformation();

    if (manageFile->checkDirectory(sourcePath))
    {

        manageFile->checkDirectoryAndCreate(destinationPath, this);
        manageFile->createSubDirs(destinationPath);

        QDir directory(sourcePath);
        directory.setSorting(QDir::Size | QDir::Reversed);

        //Image type Filters List
        QFileInfoList list = directory.entryInfoList(imageFilters, QDir::Files|QDir::NoDotAndDotDot);

        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo imageFileInfo = list.at(i);
            QString imageFileAbsolutePath = imageFileInfo.absoluteFilePath();
            QString imageFileName = manageFile->removeExtension(imageFileInfo.fileName());
            QString imageFileExtension = manageFile->getExtension(imageFileInfo.fileName());

            QString logMessage = "Process image: " + imageFileName;
            LogImgDataset::getInstance().Log(INFO, logMessage.toLocal8Bit().data());

            Mat imageBase = imread(imageFileAbsolutePath.toStdString());
            Mat imageCopy;
            imageBase.copyTo(imageCopy);

            ///Original Image
            imwrite((destinationPath + "/" + originalPath + imageFileName + imageFileExtension).toStdString(), imageCopy );

            if (bGrayChecked)
            {
                LogImgDataset::getInstance().Log(INFO, "Convert to grayscale");
                cv::cvtColor(imageCopy, imageCopy, CV_RGB2GRAY);
                imwrite((destinationPath + "/" + imageFileName + "_Gray" + imageFileExtension).toStdString(), imageCopy );
            }

            if (bCSVChecked)
            {
                csvPath = pCSVPath->text();
                manageFile->checkDirectoryAndCreate(csvPath, this);

                iWindowSize = ui->windowSizeLineEdit->text().toInt();
                iOffset = iWindowSize/2;
                iWindowSize = iWindowSize + 1;

                //Read CSV Files

                QDir csvDirectory(csvPath);
                csvDirectory.setSorting(QDir::Size | QDir::Reversed);

                //CSV type Filters List
                QStringList csvFilters = (QStringList() << imageFileName + csvFilter);
                QFileInfoList csvFileList = csvDirectory.entryInfoList(csvFilters, QDir::Files|QDir::NoDotAndDotDot);
                for (int i = 0; i < csvFileList.size(); ++i)
                {
                    QFileInfo csvFileInfo = csvFileList.at(i);
                    QString csvFileAbsolutePath = csvFileInfo.absoluteFilePath();
                    QString csvFileName = manageFile->removeExtension(csvFileInfo.fileName());

                    QString logMessage = "Reading CSV file: "+ csvFileName;
                    LogImgDataset::getInstance().Log(INFO, logMessage.toLocal8Bit().data());

                    manageImage->coordinates = manageFile->csvReaderCoordinates(csvFileAbsolutePath, ",");


                    //Write New Image Files
                    manageImage->writeImages(manageImage->coordinates, imageCopy, destinationPath, csvFileName, imageFileExtension, this);

                }

            }
            else
            {
                //Write New Image Files
                manageImage->writeImages(imageCopy, destinationPath, imageFileName, imageFileExtension, this);
            }


            if (bTSPChecked)
            {
                /// Generate some generic points
                /// Usually you would use a interest point detector such as SURF or SIFT
                LogImgDataset::getInstance().Log(INFO, "Applying Thin Plate Spline");

                std::vector<cv::Point> i, ii;

                i = manageImage->generateGrid(imageCopy,5,5, 0);
                ii = manageImage->generateGrid(imageCopy,5,5, pixelShift);


                Mat thinPlateSplineImage;
                thinPlateSplineImage = manageImage->thinPlateSplineProcessing(imageCopy, thinPlateSplineImage, i, ii);

                if (bCSVChecked)
                {
                    manageImage->writeImages(manageImage->coordinates, thinPlateSplineImage, thinPlateSplinesImagesPath, imageFileName, imageFileExtension, this);
                }
                else
                {
                    manageImage->writeImage(thinPlateSplineImage, destinationPath, imageFileName, imageFileExtension, this);
                }

            }

        }

        QMessageBox msgEndBox;
        msgEndBox.setText("Image processing complete.");
        msgEndBox.exec();

        LogImgDataset::getInstance().Log(INFO, "Image processing complete.");

    }
    else
    {
        QMessageBox msgBox;
        QString errorMsg = "Directory "+sourcePath+" not exist!";
        msgBox.setText(errorMsg);
        msgBox.exec();

        LogImgDataset::getInstance().Log(ERROR, errorMsg.toLocal8Bit().data());
    }
}

void MainWindow::verifyCheckBoxes()
{
    bCropChecked = ui->cropCheckBox->isChecked();
    bCSVChecked = ui->csvCheckBox->isChecked();
    bFlipChecked = ui->flipCheckBox->isChecked();
    bGrayChecked = ui->grayScaleCheckBox->isChecked();
    bResizeChecked = ui->resizeCheckBox->isChecked();
    bRotateChecked = ui->rotateCheckBox->isChecked();
    bTSPChecked = ui->thinPlateSplinesCheckBox->isChecked();

}

void MainWindow::verifyForm(bool)
{
    bool ok = true;

    LogImgDataset::getInstance().Log(INFO, "Validating form ...");

    verifyCheckBoxes();

    QMessageBox msgBox;
    QString errorMsg;

    if (pOriginalPath->text().isEmpty() || pOriginalPath->text().isNull())
    {
        ok = false;
        errorMsg = "The input original path is empty!";
        LogImgDataset::getInstance().Log(ERROR, errorMsg.toLocal8Bit().data());
    }
    else if (pDestinationPath->text().isEmpty() || pDestinationPath->text().isNull())
    {
        ok = false;
        errorMsg = "The input destination path is empty!";
        LogImgDataset::getInstance().Log(ERROR, errorMsg.toLocal8Bit().data());
    }
    else if (!bCropChecked && !bCSVChecked && !bFlipChecked && !bGrayChecked && !bResizeChecked && !bRotateChecked && !bTSPChecked)
    {
        ok = false;
        errorMsg = "Check at least one checkbox!";
        LogImgDataset::getInstance().Log(ERROR, errorMsg.toLocal8Bit().data());
    }
    else if (((bCropChecked) && (ui->windowSizeLineEdit->text().isEmpty() || ui->windowSizeLineEdit->text().isNull())))
    {
        ok = false;
        errorMsg = "The checkbox CSV is not checked! To crop an image you'll need a CSV file containing the coordinates for the crop.";
        LogImgDataset::getInstance().Log(ERROR, errorMsg.toLocal8Bit().data());
    }
    else if (((bCropChecked) && (!bCSVChecked)))
    {
        ok = false;
        errorMsg = "The checkbox CSV is not checked! To crop an image you'll need a CSV file containing the coordinates for the crop.";
        LogImgDataset::getInstance().Log(ERROR, errorMsg.toLocal8Bit().data());
    }
    else if ((bCropChecked) && (pCSVPath->text().isEmpty() || pCSVPath->text().isNull()))
    {
        ok = false;
        errorMsg = "The input CSV path is empty!";
        LogImgDataset::getInstance().Log(ERROR, errorMsg.toLocal8Bit().data());
    }
    else if (ui->resizeCheckBox->isChecked())
    {
        if (ui->widthLineEdit->text().isEmpty() || ui->widthLineEdit->text().isNull())
        {
            ok = false;
            errorMsg = "The input width is empty!";
            LogImgDataset::getInstance().Log(ERROR, errorMsg.toLocal8Bit().data());
        }
        else if (ui->heightLineEdit->text().isEmpty() || ui->heightLineEdit->text().isNull())
        {
            ok = false;
            errorMsg = "The input height is empty!";
            LogImgDataset::getInstance().Log(ERROR, errorMsg.toLocal8Bit().data());
        }
        else {
            iWidth = ui->widthLineEdit->text().toInt();
            iHeight = ui->heightLineEdit->text().toInt();
        }

    }

    if (ok == true)
    {
        generateImages();

    } else {
        msgBox.setText(errorMsg);
        msgBox.exec();
    }

}

void MainWindow::newForm()
{

    LogImgDataset::getInstance().Log(INFO, "New image proccessing form ...");

    ui->cropCheckBox->setChecked(false);
    ui->csvCheckBox->setChecked(false);
    ui->flipCheckBox->setChecked(false);
    ui->grayScaleCheckBox->setChecked(false);
    ui->resizeCheckBox->setChecked(false);

    pOriginalPath->clear();
    pDestinationPath->clear();
    ui->windowSizeLineEdit->clear();
    ui->widthLineEdit->clear();
    ui->heightLineEdit->clear();
    ui->csvLineEdit->clear();

}


