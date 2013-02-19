/********************************************************************************
** Form generated from reading UI file 'maskselection.ui'
**
** Created: Mon Feb 18 14:28:05 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MASKSELECTION_H
#define UI_MASKSELECTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaskSelection
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton_OpenImage;
    QPushButton *pushButton_Process;
    QLabel *label;
    QLabel *label_Color4;
    QPushButton *pushButton_Camera;
    QPushButton *pushButton_Histogram;
    QSlider *verticalSlider_MaxHue;
    QSlider *verticalSlider_MaxSat;
    QSlider *verticalSlider_MaxValue;
    QLabel *label_Color5;
    QLabel *label_Color6;
    QLabel *label_MaxHue;
    QLabel *label_MaxSat;
    QLabel *label_MaxValue;
    QPushButton *pushButton_Detect;
    QLabel *label_Max;
    QLabel *label_Min;
    QSlider *verticalSlider_MinHue;
    QSlider *verticalSlider_MinSat;
    QLabel *label_Color2;
    QLabel *label_Color3;
    QLabel *label_MinSat;
    QLabel *label_MinHue;
    QLabel *label_MinValue;
    QLabel *label_Color1;
    QSlider *verticalSlider_MinValue;
    QFrame *line;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MaskSelection)
    {
        if (MaskSelection->objectName().isEmpty())
            MaskSelection->setObjectName(QString::fromUtf8("MaskSelection"));
        MaskSelection->resize(857, 609);
        centralWidget = new QWidget(MaskSelection);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton_OpenImage = new QPushButton(centralWidget);
        pushButton_OpenImage->setObjectName(QString::fromUtf8("pushButton_OpenImage"));
        pushButton_OpenImage->setGeometry(QRect(70, 30, 221, 41));
        pushButton_Process = new QPushButton(centralWidget);
        pushButton_Process->setObjectName(QString::fromUtf8("pushButton_Process"));
        pushButton_Process->setGeometry(QRect(390, 460, 131, 51));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(360, 10, 481, 431));
        label->setFrameShape(QFrame::Box);
        label->setFrameShadow(QFrame::Raised);
        label->setLineWidth(2);
        label_Color4 = new QLabel(centralWidget);
        label_Color4->setObjectName(QString::fromUtf8("label_Color4"));
        label_Color4->setGeometry(QRect(180, 490, 41, 21));
        pushButton_Camera = new QPushButton(centralWidget);
        pushButton_Camera->setObjectName(QString::fromUtf8("pushButton_Camera"));
        pushButton_Camera->setGeometry(QRect(70, 0, 221, 41));
        pushButton_Histogram = new QPushButton(centralWidget);
        pushButton_Histogram->setObjectName(QString::fromUtf8("pushButton_Histogram"));
        pushButton_Histogram->setGeometry(QRect(520, 460, 131, 51));
        verticalSlider_MaxHue = new QSlider(centralWidget);
        verticalSlider_MaxHue->setObjectName(QString::fromUtf8("verticalSlider_MaxHue"));
        verticalSlider_MaxHue->setGeometry(QRect(200, 110, 21, 361));
        verticalSlider_MaxHue->setMaximum(180);
        verticalSlider_MaxHue->setValue(136);
        verticalSlider_MaxHue->setOrientation(Qt::Vertical);
        verticalSlider_MaxSat = new QSlider(centralWidget);
        verticalSlider_MaxSat->setObjectName(QString::fromUtf8("verticalSlider_MaxSat"));
        verticalSlider_MaxSat->setGeometry(QRect(260, 110, 21, 361));
        verticalSlider_MaxSat->setMaximum(255);
        verticalSlider_MaxSat->setValue(136);
        verticalSlider_MaxSat->setOrientation(Qt::Vertical);
        verticalSlider_MaxValue = new QSlider(centralWidget);
        verticalSlider_MaxValue->setObjectName(QString::fromUtf8("verticalSlider_MaxValue"));
        verticalSlider_MaxValue->setGeometry(QRect(320, 110, 21, 361));
        verticalSlider_MaxValue->setMaximum(255);
        verticalSlider_MaxValue->setValue(136);
        verticalSlider_MaxValue->setOrientation(Qt::Vertical);
        label_Color5 = new QLabel(centralWidget);
        label_Color5->setObjectName(QString::fromUtf8("label_Color5"));
        label_Color5->setGeometry(QRect(230, 490, 71, 21));
        label_Color6 = new QLabel(centralWidget);
        label_Color6->setObjectName(QString::fromUtf8("label_Color6"));
        label_Color6->setGeometry(QRect(310, 490, 51, 21));
        label_MaxHue = new QLabel(centralWidget);
        label_MaxHue->setObjectName(QString::fromUtf8("label_MaxHue"));
        label_MaxHue->setGeometry(QRect(190, 470, 41, 21));
        label_MaxSat = new QLabel(centralWidget);
        label_MaxSat->setObjectName(QString::fromUtf8("label_MaxSat"));
        label_MaxSat->setGeometry(QRect(240, 470, 51, 21));
        label_MaxValue = new QLabel(centralWidget);
        label_MaxValue->setObjectName(QString::fromUtf8("label_MaxValue"));
        label_MaxValue->setGeometry(QRect(300, 470, 51, 21));
        pushButton_Detect = new QPushButton(centralWidget);
        pushButton_Detect->setObjectName(QString::fromUtf8("pushButton_Detect"));
        pushButton_Detect->setGeometry(QRect(650, 450, 191, 71));
        label_Max = new QLabel(centralWidget);
        label_Max->setObjectName(QString::fromUtf8("label_Max"));
        label_Max->setGeometry(QRect(240, 520, 62, 16));
        label_Min = new QLabel(centralWidget);
        label_Min->setObjectName(QString::fromUtf8("label_Min"));
        label_Min->setGeometry(QRect(60, 520, 62, 16));
        verticalSlider_MinHue = new QSlider(centralWidget);
        verticalSlider_MinHue->setObjectName(QString::fromUtf8("verticalSlider_MinHue"));
        verticalSlider_MinHue->setGeometry(QRect(20, 110, 21, 361));
        verticalSlider_MinHue->setMaximum(180);
        verticalSlider_MinHue->setValue(136);
        verticalSlider_MinHue->setOrientation(Qt::Vertical);
        verticalSlider_MinSat = new QSlider(centralWidget);
        verticalSlider_MinSat->setObjectName(QString::fromUtf8("verticalSlider_MinSat"));
        verticalSlider_MinSat->setGeometry(QRect(80, 110, 21, 361));
        verticalSlider_MinSat->setMaximum(255);
        verticalSlider_MinSat->setValue(136);
        verticalSlider_MinSat->setOrientation(Qt::Vertical);
        label_Color2 = new QLabel(centralWidget);
        label_Color2->setObjectName(QString::fromUtf8("label_Color2"));
        label_Color2->setGeometry(QRect(50, 490, 71, 21));
        label_Color3 = new QLabel(centralWidget);
        label_Color3->setObjectName(QString::fromUtf8("label_Color3"));
        label_Color3->setGeometry(QRect(120, 490, 51, 21));
        label_MinSat = new QLabel(centralWidget);
        label_MinSat->setObjectName(QString::fromUtf8("label_MinSat"));
        label_MinSat->setGeometry(QRect(60, 470, 51, 21));
        label_MinHue = new QLabel(centralWidget);
        label_MinHue->setObjectName(QString::fromUtf8("label_MinHue"));
        label_MinHue->setGeometry(QRect(10, 470, 41, 21));
        label_MinValue = new QLabel(centralWidget);
        label_MinValue->setObjectName(QString::fromUtf8("label_MinValue"));
        label_MinValue->setGeometry(QRect(120, 470, 51, 21));
        label_Color1 = new QLabel(centralWidget);
        label_Color1->setObjectName(QString::fromUtf8("label_Color1"));
        label_Color1->setGeometry(QRect(0, 490, 41, 21));
        verticalSlider_MinValue = new QSlider(centralWidget);
        verticalSlider_MinValue->setObjectName(QString::fromUtf8("verticalSlider_MinValue"));
        verticalSlider_MinValue->setGeometry(QRect(140, 110, 21, 361));
        verticalSlider_MinValue->setMaximum(255);
        verticalSlider_MinValue->setValue(136);
        verticalSlider_MinValue->setOrientation(Qt::Vertical);
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(170, 100, 20, 451));
        line->setLineWidth(3);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        MaskSelection->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MaskSelection);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 857, 22));
        MaskSelection->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MaskSelection);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MaskSelection->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MaskSelection);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MaskSelection->setStatusBar(statusBar);

        retranslateUi(MaskSelection);

        QMetaObject::connectSlotsByName(MaskSelection);
    } // setupUi

    void retranslateUi(QMainWindow *MaskSelection)
    {
        MaskSelection->setProperty("Mask Selection", QVariant(QApplication::translate("MaskSelection", "MaskSelection", 0, QApplication::UnicodeUTF8)));
        pushButton_OpenImage->setText(QApplication::translate("MaskSelection", "Open Image", 0, QApplication::UnicodeUTF8));
        pushButton_Process->setText(QApplication::translate("MaskSelection", "Process", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_Color4->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">Hue</p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton_Camera->setText(QApplication::translate("MaskSelection", "Show Camera", 0, QApplication::UnicodeUTF8));
        pushButton_Histogram->setText(QApplication::translate("MaskSelection", "Show Histogram", 0, QApplication::UnicodeUTF8));
        label_Color5->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">Saturation</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Color6->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">Value</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MaxHue->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MaxSat->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MaxValue->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton_Detect->setText(QApplication::translate("MaskSelection", "Set Mask and Detect", 0, QApplication::UnicodeUTF8));
        label_Max->setText(QApplication::translate("MaskSelection", "Max HSV", 0, QApplication::UnicodeUTF8));
        label_Min->setText(QApplication::translate("MaskSelection", "Min HSV", 0, QApplication::UnicodeUTF8));
        label_Color2->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">Saturation</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Color3->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">Value</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MinSat->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MinHue->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_MinValue->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">136</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Color1->setText(QApplication::translate("MaskSelection", "<html><head/><body><p align=\"center\">Hue</p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MaskSelection: public Ui_MaskSelection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MASKSELECTION_H
