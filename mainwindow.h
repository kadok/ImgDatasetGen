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
 * The class MainWindow represents the main window of the system.
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

    bool bFlipChecked;
    bool bRotateChecked;
    bool bResizeChecked;
    bool bGrayChecked;
    bool bTSPChecked;
    bool bCropChecked;
    bool bCSVChecked;

    int iWidth;
    int iHeight;

    int iWindowSize;
    int iOffset;

private:
    Ui::MainWindow *ui;
    QPushButton* pButton;
    QLineEdit* pOriginalPath;
    QLineEdit* pDestinationPath;
    QLineEdit* pCSVPath;


protected:
    /**
     * @brief createActions - Create main menu actions.
     */
    virtual void createActions();

    /**
     * @brief createMenus - Create main menu.
     */
    virtual void  createMenus();


    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *exitAct;
    QAction *aboutAct;

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
     * @return Return boolean.
     */
    void verifyForm(bool);
};

#endif // MAINWINDOW_H
