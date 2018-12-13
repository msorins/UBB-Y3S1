/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QtCore/QObject>

#include "../RSAText.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *encryptPushButton;
    QPushButton *decryptPushButton;
    QPlainTextEdit *inputTextEdit;
    QLabel *outputLabel;
    QFrame *line;
    QLabel *pLabel;
    QLabel *qLabel;
    QLabel *nLabel;
    QLabel *eLabel;
    QLabel *dLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    RSAText rsaText;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(478, 256);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        encryptPushButton = new QPushButton(centralWidget);
        encryptPushButton->setObjectName(QString::fromUtf8("encryptPushButton"));
        encryptPushButton->setGeometry(QRect(30, 80, 91, 32));
        decryptPushButton = new QPushButton(centralWidget);
        decryptPushButton->setObjectName(QString::fromUtf8("decryptPushButton"));
        decryptPushButton->setGeometry(QRect(130, 80, 101, 32));
        inputTextEdit = new QPlainTextEdit(centralWidget);
        inputTextEdit->setObjectName(QString::fromUtf8("inputTextEdit"));
        inputTextEdit->setGeometry(QRect(30, 20, 191, 21));
        outputLabel =  new QLabel(centralWidget);
        outputLabel->setObjectName(QString::fromUtf8("outputLabel"));
        outputLabel->setGeometry(QRect(30, 150, 191, 21));
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(243, 10, 20, 171));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        pLabel = new QLabel(centralWidget);
        pLabel->setObjectName(QString::fromUtf8("pLabel"));
        pLabel->setGeometry(QRect(270, 20, 191, 16));
        qLabel = new QLabel(centralWidget);
        qLabel->setObjectName(QString::fromUtf8("qLabel"));
        qLabel->setGeometry(QRect(270, 50, 191, 16));
        nLabel = new QLabel(centralWidget);
        nLabel->setObjectName(QString::fromUtf8("nLabel"));
        nLabel->setGeometry(QRect(270, 80, 191, 16));
        eLabel = new QLabel(centralWidget);
        eLabel->setObjectName(QString::fromUtf8("eLabel"));
        eLabel->setGeometry(QRect(270, 110, 191, 16));
        dLabel = new QLabel(centralWidget);
        dLabel->setObjectName(QString::fromUtf8("dLabel"));
        dLabel->setGeometry(QRect(270, 140, 191, 16));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 478, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);

        this->setupRSA();
    } // setupUi

    void setupRSA() {
        string p = "p: " + to_string(this->rsaText.rsa.getQ());
        this->pLabel->setText( QString::fromStdString(p) );

        string q = "q: " + to_string(this->rsaText.rsa.getP());
        this->qLabel->setText( QString::fromStdString(q) );

        string n = "n: " + to_string(this->rsaText.rsa.getN());
        this->nLabel->setText( QString::fromStdString(n) );

        string e = "e: " + to_string(this->rsaText.rsa.getE());
        this->eLabel->setText( QString::fromStdString(e) );

        string d = "d: " + to_string(this->rsaText.rsa.getD());
        this->dLabel->setText( QString::fromStdString(d) );
    }

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        encryptPushButton->setText(QApplication::translate("MainWindow", "Encrypt", nullptr));
        decryptPushButton->setText(QApplication::translate("MainWindow", "Decrypt", nullptr));
        pLabel->setText(QApplication::translate("MainWindow", "pLabel", nullptr));
        qLabel->setText(QApplication::translate("MainWindow", "qLabel", nullptr));
        nLabel->setText(QApplication::translate("MainWindow", "nLabel", nullptr));
        eLabel->setText(QApplication::translate("MainWindow", "eLabel", nullptr));
        dLabel->setText(QApplication::translate("MainWindow", "dLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
