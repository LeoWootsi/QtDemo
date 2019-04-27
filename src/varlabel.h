#ifndef VARLABEL_H
#define VARLABEL_H

#include <QWidget>
#include <QLabel>

class Varlabel : public QLabel
{
    Q_OBJECT

signals:
    void clicked(bool);
public:
    explicit Varlabel(QWidget *parent=nullptr, Qt::WindowFlags f=nullptr);
    explicit Varlabel(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=nullptr);
protected:
    void mousePressEvent(QMouseEvent *e)override;
private:
    bool m_tick=0;


};

#endif // VARLABEL_H
