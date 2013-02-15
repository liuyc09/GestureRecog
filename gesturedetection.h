#ifndef GESTUREDETECTION_H
#define GESTUREDETECTION_H

#include <QDialog>

namespace Ui {
class GestureDetection;
}

class GestureDetection : public QDialog
{
    Q_OBJECT
    
public:
    explicit GestureDetection(QWidget *parent = 0);
    ~GestureDetection();
    
private:
    Ui::GestureDetection *ui;
};

#endif // GESTUREDETECTION_H
