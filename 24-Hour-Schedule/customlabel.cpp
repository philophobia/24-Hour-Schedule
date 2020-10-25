#include "customlabel.h"

CustomLabel::CustomLabel(QWidget *parent):QLabel(parent)
{

}

void CustomLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    if(ev->button() == Qt::LeftButton)
    {
        emit clicked(index);
    }
}
