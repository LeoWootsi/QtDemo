#include "digitlineedit.h"

#include <QScrollBar>
#include <QRegExp>
#include <QRegExpValidator>
#include <QKeyEvent>

DigitLineEdit::DigitLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    //禁止中文输入
    setAttribute(Qt::WA_InputMethodEnabled, false);
}

void DigitLineEdit::keyPressEvent(QKeyEvent *e)
{

    int key=e->key();
    if((key>=Qt::Key_0 && key<=Qt::Key_9) || key==Qt::Key_Comma || key==Qt::Key_Minus
            ||key==Qt::Key_Enter||key==Qt::Key_Return||key==Qt::Key_Backspace||key==Qt::Key_Left||key==Qt::Key_Right
            ||key==Qt::Key_Up||key==Qt::Key_Down||key==Qt::Key_Home||key==Qt::Key_End)
        QLineEdit::keyPressEvent(e);
    else
        e->accept();

}


