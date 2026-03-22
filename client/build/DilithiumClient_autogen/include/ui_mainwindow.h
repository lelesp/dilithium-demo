/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QHBoxLayout *serverLayout;
    QLabel *serverLabel;
    QLineEdit *serverUrlEdit;
    QFrame *signFrame;
    QVBoxLayout *signLayout;
    QLabel *signHeaderLabel;
    QHBoxLayout *selectFileLayout;
    QPushButton *selectFileBtn;
    QLabel *selectedFileLabel;
    QPushButton *signFileBtn;
    QFrame *verifyFrame;
    QVBoxLayout *verifyLayout;
    QLabel *verifyHeaderLabel;
    QHBoxLayout *loadZipLayout;
    QPushButton *selectSigFileBtn;
    QLabel *sigFileLabel;
    QLabel *pubKeyLabel;
    QPushButton *verifyBtn;
    QFrame *logFrame;
    QVBoxLayout *logLayout;
    QLabel *logHeaderLabel;
    QTextEdit *logText;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 650);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        serverLayout = new QHBoxLayout();
        serverLayout->setObjectName("serverLayout");
        serverLabel = new QLabel(centralwidget);
        serverLabel->setObjectName("serverLabel");

        serverLayout->addWidget(serverLabel);

        serverUrlEdit = new QLineEdit(centralwidget);
        serverUrlEdit->setObjectName("serverUrlEdit");

        serverLayout->addWidget(serverUrlEdit);


        verticalLayout->addLayout(serverLayout);

        signFrame = new QFrame(centralwidget);
        signFrame->setObjectName("signFrame");
        signFrame->setFrameShape(QFrame::StyledPanel);
        signFrame->setFrameShadow(QFrame::Raised);
        signLayout = new QVBoxLayout(signFrame);
        signLayout->setObjectName("signLayout");
        signHeaderLabel = new QLabel(signFrame);
        signHeaderLabel->setObjectName("signHeaderLabel");

        signLayout->addWidget(signHeaderLabel);

        selectFileLayout = new QHBoxLayout();
        selectFileLayout->setObjectName("selectFileLayout");
        selectFileBtn = new QPushButton(signFrame);
        selectFileBtn->setObjectName("selectFileBtn");

        selectFileLayout->addWidget(selectFileBtn);

        selectedFileLabel = new QLabel(signFrame);
        selectedFileLabel->setObjectName("selectedFileLabel");

        selectFileLayout->addWidget(selectedFileLabel);


        signLayout->addLayout(selectFileLayout);

        signFileBtn = new QPushButton(signFrame);
        signFileBtn->setObjectName("signFileBtn");

        signLayout->addWidget(signFileBtn);


        verticalLayout->addWidget(signFrame);

        verifyFrame = new QFrame(centralwidget);
        verifyFrame->setObjectName("verifyFrame");
        verifyFrame->setFrameShape(QFrame::StyledPanel);
        verifyFrame->setFrameShadow(QFrame::Raised);
        verifyLayout = new QVBoxLayout(verifyFrame);
        verifyLayout->setObjectName("verifyLayout");
        verifyHeaderLabel = new QLabel(verifyFrame);
        verifyHeaderLabel->setObjectName("verifyHeaderLabel");

        verifyLayout->addWidget(verifyHeaderLabel);

        loadZipLayout = new QHBoxLayout();
        loadZipLayout->setObjectName("loadZipLayout");
        selectSigFileBtn = new QPushButton(verifyFrame);
        selectSigFileBtn->setObjectName("selectSigFileBtn");

        loadZipLayout->addWidget(selectSigFileBtn);

        sigFileLabel = new QLabel(verifyFrame);
        sigFileLabel->setObjectName("sigFileLabel");

        loadZipLayout->addWidget(sigFileLabel);


        verifyLayout->addLayout(loadZipLayout);

        pubKeyLabel = new QLabel(verifyFrame);
        pubKeyLabel->setObjectName("pubKeyLabel");

        verifyLayout->addWidget(pubKeyLabel);

        verifyBtn = new QPushButton(verifyFrame);
        verifyBtn->setObjectName("verifyBtn");

        verifyLayout->addWidget(verifyBtn);


        verticalLayout->addWidget(verifyFrame);

        logFrame = new QFrame(centralwidget);
        logFrame->setObjectName("logFrame");
        logFrame->setFrameShape(QFrame::StyledPanel);
        logFrame->setFrameShadow(QFrame::Raised);
        logLayout = new QVBoxLayout(logFrame);
        logLayout->setObjectName("logLayout");
        logHeaderLabel = new QLabel(logFrame);
        logHeaderLabel->setObjectName("logHeaderLabel");

        logLayout->addWidget(logHeaderLabel);

        logText = new QTextEdit(logFrame);
        logText->setObjectName("logText");
        logText->setReadOnly(true);
        logText->setMaximumHeight(150);

        logLayout->addWidget(logText);


        verticalLayout->addWidget(logFrame);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "ML-DSA-65 Client - Post-Quantum Digital Signatures", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "<h2>Post-Quantum Digital Signature Demo (ML-DSA-65)</h2>", nullptr));
        serverLabel->setText(QCoreApplication::translate("MainWindow", "Server URL:", nullptr));
        serverUrlEdit->setText(QCoreApplication::translate("MainWindow", "http://localhost:8000", nullptr));
        signHeaderLabel->setText(QCoreApplication::translate("MainWindow", "<b>Sign File (Server-side)</b>", nullptr));
        selectFileBtn->setText(QCoreApplication::translate("MainWindow", "Select File", nullptr));
        selectedFileLabel->setText(QCoreApplication::translate("MainWindow", "No file selected", nullptr));
        signFileBtn->setText(QCoreApplication::translate("MainWindow", "Sign File (via Server)", nullptr));
        verifyHeaderLabel->setText(QCoreApplication::translate("MainWindow", "<b>Verify Signature (Local via liboqs)</b>", nullptr));
        selectSigFileBtn->setText(QCoreApplication::translate("MainWindow", "Load Package", nullptr));
        sigFileLabel->setText(QCoreApplication::translate("MainWindow", "No package loaded", nullptr));
        pubKeyLabel->setText(QCoreApplication::translate("MainWindow", "Public Key: -", nullptr));
        verifyBtn->setText(QCoreApplication::translate("MainWindow", "Verify Signature", nullptr));
        logHeaderLabel->setText(QCoreApplication::translate("MainWindow", "<b>Log</b>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
