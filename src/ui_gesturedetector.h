/********************************************************************************
** Form generated from reading UI file 'gesturedetector.ui'
**
** Created: Mon Feb 18 15:05:29 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTUREDETECTOR_H
#define UI_GESTUREDETECTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_GestureDetector
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *lblImage;
    QTextEdit *textEdit;
    QPushButton *btnPause;

    void setupUi(QDialog *GestureDetector)
    {
        if (GestureDetector->objectName().isEmpty())
            GestureDetector->setObjectName(QString::fromUtf8("GestureDetector"));
        GestureDetector->resize(930, 424);
        buttonBox = new QDialogButtonBox(GestureDetector);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(830, 390, 81, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);
        lblImage = new QLabel(GestureDetector);
        lblImage->setObjectName(QString::fromUtf8("lblImage"));
        lblImage->setGeometry(QRect(20, 10, 481, 401));
        textEdit = new QTextEdit(GestureDetector);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(510, 10, 411, 371));
        QFont font;
        font.setFamily(QString::fromUtf8("Calibri"));
        font.setPointSize(11);
        textEdit->setFont(font);
        btnPause = new QPushButton(GestureDetector);
        btnPause->setObjectName(QString::fromUtf8("btnPause"));
        btnPause->setGeometry(QRect(540, 390, 71, 32));

        retranslateUi(GestureDetector);
        QObject::connect(buttonBox, SIGNAL(accepted()), GestureDetector, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GestureDetector, SLOT(reject()));

        QMetaObject::connectSlotsByName(GestureDetector);
    } // setupUi

    void retranslateUi(QDialog *GestureDetector)
    {
        GestureDetector->setWindowTitle(QApplication::translate("GestureDetector", "Dialog", 0, QApplication::UnicodeUTF8));
        lblImage->setText(QString());
        btnPause->setText(QApplication::translate("GestureDetector", "Pause", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GestureDetector: public Ui_GestureDetector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTUREDETECTOR_H
