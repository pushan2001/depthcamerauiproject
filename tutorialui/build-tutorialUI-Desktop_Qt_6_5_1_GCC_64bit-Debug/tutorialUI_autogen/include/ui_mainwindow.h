/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *imageLabel;
    QComboBox *frameSelectionBox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(848, 480);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        imageLabel = new QLabel(centralwidget);
        imageLabel->setObjectName("imageLabel");
        imageLabel->setGeometry(QRect(0, 30, 848, 480));
        frameSelectionBox = new QComboBox(centralwidget);
        frameSelectionBox->addItem(QString());
        frameSelectionBox->addItem(QString());
        frameSelectionBox->setObjectName("frameSelectionBox");
        frameSelectionBox->setGeometry(QRect(750, 70, 79, 23));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 848, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        imageLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        frameSelectionBox->setItemText(0, QCoreApplication::translate("MainWindow", "Depth", nullptr));
        frameSelectionBox->setItemText(1, QCoreApplication::translate("MainWindow", "Colour", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
