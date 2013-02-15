#include "gesturedetection.h"
#include "ui_gesturedetection.h"

GestureDetection::GestureDetection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GestureDetection)
{
    ui->setupUi(this);
}

GestureDetection::~GestureDetection()
{
    delete ui;
}
