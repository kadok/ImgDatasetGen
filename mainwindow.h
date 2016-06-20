#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QStyleFactory>
#include <QMessageBox>
#include <QFileDialog>
#include <QSignalMapper>
#include <QDir>


enum PATH
{
    ORIGINAL_PATH,
    DESTINATION_PATH,
    CSV_PATH
};

namespace Ui {
class MainWindow;
}

/**
 * @file mainwindow.h
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
 */

/**
 * @class MainWindow
 * @brief The MainWindow class - The class MainWindow represents the main window of the system.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow - Class Constructor.
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief ~MainWindow - Destructor.
     */
    ~MainWindow();

    /**
     * @brief generateImages - Generate Image Transformations.
     */
    void generateImages();

    /**
     * @brief verifyCheckBoxes - Verify the Checkboxes.
     */
    void verifyCheckBoxes();

    bool bFlipChecked; /*!< Flag to verify if Flip checkbox is checked declaration. */
    bool bRotateChecked; /*!< Flag to verify if Rotate checkbox is checked declaration. */
    bool bResizeChecked; /*!< Flag to verify if Resize checkbox is checked declaration. */
    bool bGrayChecked; /*!< Flag to verify if Grayscale checkbox is checked declaration. */
    bool bTSPChecked; /*!< Flag to verify if Thin Plane Spline checkbox is checked declaration. */
    bool bCropChecked; /*!< Flag to verify if Crop checkbox is checked declaration. */
    bool bCSVChecked; /*!< Flag to verify if CSV checkbox is checked declaration. */

    int iWidth; /*!< Rect width */
    int iHeight; /*!< Rect height */

    int iWindowSize; /*!< Window size */
    int iOffset; /*!< Windown offset */

private:
    Ui::MainWindow *ui; /*!< Ui declaration. */
    QPushButton* pButton; /*!< Generic button. */
    QLineEdit* pOriginalPath; /*!< Original path form input */
    QLineEdit* pDestinationPath; /*!< Destination path form input */
    QLineEdit* pCSVPath; /*!< CSV path form input */


protected:
    /**
     * @brief createActions - Create main menu actions.
     */
    virtual void createActions();

    /**
     * @brief createMenus - Create main menu.
     */
    virtual void  createMenus();


    QMenu *fileMenu; /*!< Main menu declaration */
    QMenu *helpMenu; /*!< Help menu declaration */

    QAction *newAct; /*!< New action declaration. */
    QAction *exitAct; /*!< Exit action declaration. */
    QAction *aboutAct; /*!< About action declaration. */

private slots:
    /**
     * @brief about - About the software.
     */
    void about();

    /**
     * @brief browse - Browse the directories.
     * @param symbol - Directory symbol.
     */
    void browse(int symbol);

    /**
     * @brief verifyForm - Verify the form submit.
     */
    void verifyForm(bool);

    /**
     * @brief newForm - Clear the form.
     */
    void newForm();
};

#endif // MAINWINDOW_H
