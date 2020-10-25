#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CustomButton(QWidget *parent);
    int index;
protected:
    virtual void mouseReleaseEvent(QMouseEvent * e);
signals:
    void clicked(int);
};

#endif // CUSTOMBUTTON_H
