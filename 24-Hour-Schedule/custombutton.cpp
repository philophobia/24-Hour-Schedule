#include "custombutton.h"

CustomButton::CustomButton(QWidget *parent):QPushButton(parent)
{

}

void CustomButton::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    if(ev->button() == Qt::LeftButton)
    {
        emit clicked(index);
    }
}
