#include "console.h"

#include <QScrollBar>
#include <QRegExp>
#include <QRegExpValidator>

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    document()->setMaximumBlockCount(100);
}

void Console::keyPressEvent(QKeyEvent *e)
{

    if(e->key()==Qt::Key_Return && e->modifiers()==Qt::ControlModifier)  //按住Ctrl+Enter 发送  个人习惯
        emit sendContent();
    if(hexCheck)
    {
        int key=e->key();
        if((key>=Qt::Key_0 && key<=Qt::Key_9)||(key>=Qt::Key_A && key<=Qt::Key_F)||key==Qt::Key_Enter||
                key==Qt::Key_Return||key==Qt::Key_Backspace||key==Qt::Key_Space||key==Qt::Key_Left||key==Qt::Key_Right
                ||key==Qt::Key_Up||key==Qt::Key_Down||key==Qt::Key_Home||key==Qt::Key_End)
            QPlainTextEdit::keyPressEvent(e);
        else
            e->accept();
    }
    else
        QPlainTextEdit::keyPressEvent(e);

}

