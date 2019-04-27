#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

class Console : public QPlainTextEdit
{
    Q_OBJECT
signals:
    void sendContent();
public:
    explicit Console(QWidget *parent = nullptr);
    bool hexCheck=false;

protected:
    void keyPressEvent(QKeyEvent *e) override;
//    void mousePressEvent(QMouseEvent *e) override;
//    void mouseDoubleClickEvent(QMouseEvent *e) override;
};


#endif // CONSOLE_H
