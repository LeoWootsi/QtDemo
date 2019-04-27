#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QColorDialog>
#include <QCloseEvent>
#include <QList>
#include "varlabel.h"
#include <QMessageBox>


Settingsdialog::Settingsdialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settingsdialog)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);

    colorWidges<<ui->color1<<ui->color1_2<<ui->color1_3<<ui->color1_4<<ui->color1_5<<ui->color1_6<<ui->color1_7
                    <<ui->color1_8<<ui->color1_9<<ui->color1_10<<ui->color1_11<<ui->color1_12<<ui->color1_13
                       <<ui->color1_14<<ui->color1_15<<ui->color1_16<<ui->color1_17<<ui->color1_18<<ui->color1_19<<ui->color1_20;

    nameWidges<<ui->name1<<ui->name1_2<<ui->name1_3<<ui->name1_4<<ui->name1_5<<ui->name1_6<<ui->name1_7
                    <<ui->name1_8<<ui->name1_9<<ui->name1_10<<ui->name1_11<<ui->name1_12<<ui->name1_13<<ui->name1_14
                      <<ui->name1_15<<ui->name1_16<<ui->name1_17<<ui->name1_18<<ui->name1_19<<ui->name1_20;

    rangeWidges<<ui->range1<<ui->range1_2<<ui->range1_3<<ui->range1_4<<ui->range1_5<<ui->range1_6<<ui->range1_7
                    <<ui->range1_8<<ui->range1_9<<ui->range1_10<<ui->range1_11<<ui->range1_12<<ui->range1_13<<ui->range1_14
                      <<ui->range1_15<<ui->range1_16<<ui->range1_17<<ui->range1_18<<ui->range1_19<<ui->range1_20;


    connect(ui->color1,&Varlabel::clicked,this,&Settingsdialog::handleColor1);
    connect(ui->color1_2,&Varlabel::clicked,this,&Settingsdialog::handleColor2);
    connect(ui->color1_3,&Varlabel::clicked,this,&Settingsdialog::handleColor3);
    connect(ui->color1_4,&Varlabel::clicked,this,&Settingsdialog::handleColor4);
    connect(ui->color1_5,&Varlabel::clicked,this,&Settingsdialog::handleColor5);
    connect(ui->color1_6,&Varlabel::clicked,this,&Settingsdialog::handleColor6);
    connect(ui->color1_7,&Varlabel::clicked,this,&Settingsdialog::handleColor7);
    connect(ui->color1_8,&Varlabel::clicked,this,&Settingsdialog::handleColor8);
    connect(ui->color1_9,&Varlabel::clicked,this,&Settingsdialog::handleColor9);
    connect(ui->color1_10,&Varlabel::clicked,this,&Settingsdialog::handleColor10);
    connect(ui->color1_11,&Varlabel::clicked,this,&Settingsdialog::handleColor11);
    connect(ui->color1_12,&Varlabel::clicked,this,&Settingsdialog::handleColor12);
    connect(ui->color1_13,&Varlabel::clicked,this,&Settingsdialog::handleColor13);
    connect(ui->color1_14,&Varlabel::clicked,this,&Settingsdialog::handleColor14);
    connect(ui->color1_15,&Varlabel::clicked,this,&Settingsdialog::handleColor15);
    connect(ui->color1_16,&Varlabel::clicked,this,&Settingsdialog::handleColor16);
    connect(ui->color1_17,&Varlabel::clicked,this,&Settingsdialog::handleColor17);
    connect(ui->color1_18,&Varlabel::clicked,this,&Settingsdialog::handleColor18);
    connect(ui->color1_19,&Varlabel::clicked,this,&Settingsdialog::handleColor19);
    connect(ui->color1_20,&Varlabel::clicked,this,&Settingsdialog::handleColor20);

}

Settingsdialog::~Settingsdialog()
{
    delete ui;
}

void Settingsdialog::setInitValue(QColor *colors, int *maxs, int *mins ,QString *names)
{
    QPalette p;


    for(int i=0;i<20;i++)
    {
        colorWidges[i]->setAutoFillBackground(true);
        m_colors[i]=colors[i];
        m_maxs[i]=maxs[i];
        m_mins[i]=mins[i];
        m_names[i]=names[i];
        p.setColor(QPalette::Window,m_colors[i]);
        colorWidges[i]->setPalette(p);
        rangeWidges[i]->setText(QString::number(m_mins[i])+","+QString::number(m_maxs[i]));
        nameWidges[i]->setText(names[i]);
    }
}



void Settingsdialog::handleColor1()
{
    ui->color1->setAutoFillBackground(true);
    QPalette p=ui->color1->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[0]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1->setPalette(p);
}
void Settingsdialog::handleColor2()
{
    ui->color1_2->setAutoFillBackground(true);
    QPalette p=ui->color1_2->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
     m_colors[1]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_2->setPalette(p);
}
void Settingsdialog::handleColor3()
{

    ui->color1_3->setAutoFillBackground(true);
    QPalette p=ui->color1_3->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[2]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_3->setPalette(p);
}
void Settingsdialog::handleColor4()
{
    ui->color1_4->setAutoFillBackground(true);
    QPalette p=ui->color1_4->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[3]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_4->setPalette(p);
}
void Settingsdialog::handleColor5()
{
    ui->color1_5->setAutoFillBackground(true);
    QPalette p=ui->color1_5->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[4]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_5->setPalette(p);
}
void Settingsdialog::handleColor6()
{
    ui->color1_6->setAutoFillBackground(true);
    QPalette p=ui->color1_6->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[5]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_6->setPalette(p);
}
void Settingsdialog::handleColor7()
{
    ui->color1_7->setAutoFillBackground(true);
    QPalette p=ui->color1_7->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[6]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_7->setPalette(p);
}
void Settingsdialog::handleColor8()
{
    ui->color1_8->setAutoFillBackground(true);
    QPalette p=ui->color1_8->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[7]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_8->setPalette(p);
}
void Settingsdialog::handleColor9()
{
    ui->color1_9->setAutoFillBackground(true);
    QPalette p=ui->color1_9->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[8]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_9->setPalette(p);
}
void Settingsdialog::handleColor10()
{
    ui->color1_10->setAutoFillBackground(true);
    QPalette p=ui->color1_10->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[9]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_10->setPalette(p);
}
void Settingsdialog::handleColor11()
{
    ui->color1_11->setAutoFillBackground(true);
    QPalette p=ui->color1_11->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[10]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_11->setPalette(p);
}
void Settingsdialog::handleColor12()
{
    ui->color1_12->setAutoFillBackground(true);
    QPalette p=ui->color1_12->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[11]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_12->setPalette(p);
}
void Settingsdialog::handleColor13()
{
    ui->color1_13->setAutoFillBackground(true);
    QPalette p=ui->color1_13->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[12]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_13->setPalette(p);
}
void Settingsdialog::handleColor14()
{
    ui->color1_14->setAutoFillBackground(true);
    QPalette p=ui->color1_14->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[13]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_14->setPalette(p);
}
void Settingsdialog::handleColor15()
{
    ui->color1_15->setAutoFillBackground(true);
    QPalette p=ui->color1_15->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[14]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_15->setPalette(p);
}
void Settingsdialog::handleColor16()
{
    ui->color1_16->setAutoFillBackground(true);
    QPalette p=ui->color1_16->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[15]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_16->setPalette(p);
}
void Settingsdialog::handleColor17()
{
    ui->color1_17->setAutoFillBackground(true);
    QPalette p=ui->color1_17->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[16]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_17->setPalette(p);
}
void Settingsdialog::handleColor18()
{
    ui->color1_18->setAutoFillBackground(true);
    QPalette p=ui->color1_18->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[17]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_18->setPalette(p);
}
void Settingsdialog::handleColor19()
{
    ui->color1_19->setAutoFillBackground(true);
    QPalette p=ui->color1_19->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[18]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_19->setPalette(p);
}
void Settingsdialog::handleColor20()
{
    ui->color1_20->setAutoFillBackground(true);
    QPalette p=ui->color1_20->palette();
    QColor color=QColorDialog::getColor(p.color(QPalette::Window),this);
    if(color.isValid())
    {
        m_colors[19]=color;
     p.setColor(QPalette::Window,color);
     p.setColor(QPalette::Base,color);
    }
    ui->color1_20->setPalette(p);
}


void Settingsdialog::on_pushButton_clicked()
{
    QStringList tmp;
    for(int i=0;i<20;i++)
    {
        tmp=rangeWidges[i]->text().split(',');
        if(tmp.length()<2)
        {
            QMessageBox::critical(this, tr("Error"), tr("请检查变量范围输入的格式     "));
            return;
        }
        else
        {
            m_mins[i]=tmp[0].toInt();
            m_maxs[i]=tmp[1].toInt();
        }

        m_names[i]=nameWidges[i]->text();
    }
    emit settingsChanged(m_colors,m_mins,m_maxs,m_names);
}
