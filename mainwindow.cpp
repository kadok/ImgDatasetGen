#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filemanipulation.h"
#include "imagetransformation.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(browse(int))) ;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/project/resources/novo_24x24.png"), tr("&Novo"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Criar novo arquivo"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAct = new QAction(tr("Sair"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Sair da aplicação"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    aboutAct = new QAction(tr("Sobre"), this);
    aboutAct->setStatusTip(tr("Mostra a janela de informação sobre a aplicação"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Arquivo"));
    fileMenu->addAction(newAct);

    fileMenu->addSeparator();

    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Ajuda"));
    helpMenu->addAction(aboutAct);

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Image Dataset Generator"),
                       tr("Versão 1.0 - © 2015 - Developed by Renato Moraes"));
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

void MainWindow::generateMain()
{
    const static QString originalPath = pOriginalPath->text();
    const static QString destinationPath = pDestinationPath->text();
    const static QString csvPath = pCSVPath->text();

    FileManipulation* manageFile = new FileManipulation();
    if (!manageFile->checkDirectory(originalPath)) {

    } else {

        manageFile->checkDirectoryAndCreate(destinationPath, this);
        manageFile->checkDirectoryAndCreate(csvPath, this);

        QDir directory(originalPath);
        directory.setSorting(QDir::Size | QDir::Reversed);

        ///Image type Filters List
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.bmp" << "*.tiff" << "*.gif";
        QFileInfoList list = directory.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);

        //std::cout << "     Bytes Filename" << std::endl;
        for (int i = 0; i < list.size(); ++i) {

            QFileInfo fileInfo = list.at(i);
            //std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()));
            //std::cout << std::endl;

            QString fileAbsolutePath = fileInfo.absoluteFilePath();

            Mat imageBase = imread(fileAbsolutePath.toStdString());

            ///Read CSV Files
//            QString csvFilenamePositive = manageFile->withoutExtension(fileAbsolutePath) + positive + csv;
//            Vector<mitosis> mitoses = manageFile->csvReaderMitosis(csvFilenamePositive, ",");
//            QString csvFilenameNegative = manageFile->withoutExtension(fileAbsolutePath) + negative + csv;
//            Vector<mitosis> notMitoses = manageFile->csvReaderMitosis(csvFilenameNegative, ",");


//            std::vector<cv::Point> iPositive, iiPositive, iNegative, iiNegative;

//            for (int j=0; j < mitoses.size(); j++)
//            {
//                iPositive.push_back(Point(mitoses[j].x,mitoses[j].y));
//                iiPositive.push_back(Point(mitoses[j].x,mitoses[j].y));
//                iNegative.push_back(Point(notMitoses[j].x,notMitoses[j].y));
//                iiNegative.push_back(Point(notMitoses[j].x+1,notMitoses[j].y+1));
//            }

        }
    }
}

void MainWindow::verifyForm(bool)
{
    bool ok = true;

    if ((pOriginalPath->text().isEmpty() || pOriginalPath->text().isNull()) ||
        (pDestinationPath->text().isEmpty() || pDestinationPath->text().isNull()))
    {
        ok = false;
    }
    else if (((!ui->cropCheckBox->isChecked()) && (ui->csvCheckBox->isChecked())) ||
             ((ui->cropCheckBox->isChecked()) && (!ui->csvCheckBox->isChecked())) )
    {
        ok = false;
    }
    else if ((!ui->cropCheckBox->isChecked()) && (pCSVPath->text().isEmpty() || pCSVPath->text().isNull()) ||
            ((ui->cropCheckBox->isChecked()) && (pCSVPath->text().isEmpty() || pCSVPath->text().isNull())) )
    {
        ok = false;
    }
    else if (ui->resizeCheckBox->isChecked())
    {
        if ((ui->widthLineEdit->text().isEmpty() || ui->widthLineEdit->text().isNull()) ||
            (ui->heightLineEdit->text().isEmpty() || ui->heightLineEdit->text().isNull()) )
        {
            ok = false;
        }

    }

    if (ok == true)
    {
        generateMain();
    }

}



