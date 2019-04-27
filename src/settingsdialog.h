#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QWidget>
#include <QLineEdit>
#include "varlabel.h"

namespace Ui {
class Settingsdialog;
}

class Settingsdialog : public QWidget
{
    Q_OBJECT
signals:
    void settingsChanged(QColor colors[20],int mins[20],int maxs[20],QString names[20]) ;
public:
    explicit Settingsdialog(QWidget *parent = nullptr);
    ~Settingsdialog()override;

    void setInitValue(QColor *colors,int *maxs,int *mins, QString *names);

private slots:

    void on_pushButton_clicked();

private:
    Ui::Settingsdialog *ui;
    QColor m_colors[20];
    int m_mins[20];
    int m_maxs[20];
    QString m_names[20];


    QList<QLineEdit*> rangeWidges;
    QList<QLineEdit*> nameWidges;
    QList<Varlabel*> colorWidges;

    void handleColor1();
    void handleColor2();
    void handleColor3();
    void handleColor4();
    void handleColor5();
    void handleColor6();
    void handleColor7();
    void handleColor8();
    void handleColor9();
    void handleColor10();
    void handleColor11();
    void handleColor12();
    void handleColor13();
    void handleColor14();
    void handleColor15();
    void handleColor16();
    void handleColor17();
    void handleColor18();
    void handleColor19();
    void handleColor20();


};

#endif // SETTINGSDIALOG_H
