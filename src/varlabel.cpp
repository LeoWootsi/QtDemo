#include "varlabel.h"

Varlabel::Varlabel(QWidget *parent, Qt::WindowFlags f):
    QLabel (parent,f)
{
    this->setAutoFillBackground(true);
}

Varlabel::Varlabel(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel (text,parent,f)
{
    this->setAutoFillBackground(true);
}

void Varlabel::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    this->m_tick=!this->m_tick;
    emit clicked(this->m_tick);
}
