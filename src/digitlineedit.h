#ifndef DIGITLINEEDIT_H
#define DIGITLINEEDIT_H

#include <QLineEdit>

class DigitLineEdit : public QLineEdit
{
public:
    explicit DigitLineEdit(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *e) override;
};

#endif // DIGITLINEEDIT_H
