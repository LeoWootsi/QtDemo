#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QDateTime>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serial(new QSerialPort(this)),
    m_iniFile(new QSettings(QCoreApplication::applicationDirPath()+"/config.ini", QSettings::IniFormat)),
    m_settings(new Settingsdialog)
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    ui->setupUi(this);

    QTextCodec::setCodecForLocale(tc);

    ui->baudRateCombo->setCurrentIndex(3);

    m_varLineEdits<<ui->val1<<ui->val2<<ui->val3<<ui->val4<<ui->val5<<ui->val6<<ui->val7
                    <<ui->val8<<ui->val9<<ui->val10<<ui->val11<<ui->val12<<ui->val13<<ui->val14
                      <<ui->val15<<ui->val16<<ui->val17<<ui->val18<<ui->val19<<ui->val20;
    m_varLabels<<ui->param1<<ui->param2<<ui->param3<<ui->param4<<ui->param5<<ui->param6<<ui->param7
              <<ui->param8<<ui->param9<<ui->param10<<ui->param11<<ui->param12<<ui->param13<<ui->param14
                <<ui->param15<<ui->param16<<ui->param17<<ui->param18<<ui->param19<<ui->param20;

    for(int i=0;i<20;i++)
    {
        m_varNames[i]=tr("VAR%1").arg(i+1);
    }

    this->readIni();


    //颜色初始化
    for(int i=0;i<20;i++)
    {
        m_palette.setColor(QPalette::Base,ui->chart->seriesColor[i]);
        m_varLineEdits[i]->setPalette(m_palette);
        m_varLabels[i]->setText(m_varNames[i]);
    }

    connect(ui->chart,&Chart::updateUi,this,&MainWindow::handleUpdateUi);
    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(ui->SndBtn, &QPushButton::clicked, this, &MainWindow::writeData);
    connect(ui->settingBtn,&QPushButton::clicked,this,&MainWindow::handleSettingBtn);
    connect(m_settings,&Settingsdialog::settingsChanged,this,&MainWindow::handleSettingChanged);
    connect(ui->SndTextEdit,&Console::sendContent,this,&MainWindow::writeData);

    addLabelConnections();

    m_settings->setEnabled(false);
    ui->SndBtn->setEnabled(false);
    this->on_detectBtn_clicked();


    //窗口可以最小化却不能最大化  这样处理的原因是调整各控件随窗体大小变化很麻烦，干脆让窗体大小固定
    this->setFixedSize(this->size());
}

void MainWindow::readIni()
{
    //Color
    m_iniFile->beginGroup("Color");
    QStringList value;
    for(int i=0;i<20;i++)
    {


        value=m_iniFile->value(tr("VAL%1").arg(i+1),QString::number(ui->chart->seriesColor[i].red())+","+QString::number(ui->chart->seriesColor[i].green())
                               +","+QString::number(ui->chart->seriesColor[i].blue())).toString().split(',');
        ui->chart->seriesColor[i].setRed(value[0].toInt());
        ui->chart->seriesColor[i].setGreen(value[1].toInt());
        ui->chart->seriesColor[i].setBlue(value[2].toInt());

    }
    m_iniFile->endGroup();

    //Min
    m_iniFile->beginGroup("Min");
    for(int i=0;i<20;i++)
        ui->chart->listYmin[i]=m_iniFile->value(tr("VAL%1").arg(i+1),ui->chart->listYmin[i]).toInt();
    m_iniFile->endGroup();

    //Max
    m_iniFile->beginGroup("Max");
    for(int i=0;i<20;i++)
        ui->chart->listYmax[i]=m_iniFile->value(tr("VAL%1").arg(i+1),ui->chart->listYmax[i]).toInt();
    m_iniFile->endGroup();

    //Name
    m_iniFile->beginGroup("Name");
    for(int i=0;i<20;i++)
        m_varNames[i]=m_iniFile->value(tr("VAL%1").arg(i+1),m_varNames[i]).toString();
    m_iniFile->endGroup();
}

void MainWindow::handleUpdateUi()
{

    for(int i=0;i<20;i++)
    {
        m_varLineEdits[i]->setText(QString::number(     //串口关闭时的显示
        ui->chart->showCursor?static_cast<double>(ui->chart->yValue(i,ui->chart->currentCursorX())) : static_cast<double>(this->m_values[i])));
    }

}
MainWindow::~MainWindow()
{
    delete this->m_iniFile;
    delete this->m_settings;
    delete ui;
}

void MainWindow::openSerialPort()
{
    m_serial->setPortName(ui->portnameCombo->currentText());
    m_serial->setBaudRate(static_cast<QSerialPort::BaudRate>(ui->baudRateCombo->currentText().toInt()));
    m_serial->setDataBits(static_cast<QSerialPort::DataBits>(8));
    m_serial->setParity(static_cast<QSerialPort::Parity>(0));
    m_serial->setStopBits(static_cast<QSerialPort::StopBits>(1));
    m_serial->setFlowControl(static_cast<QSerialPort::FlowControl>(0));
    if (m_serial->open(QIODevice::ReadWrite))
    {
        ui->SndBtn->setEnabled(true);
        ui->detectBtn->setEnabled(false);
        showStatusMessage(tr("打开 %1 : %2")
                          .arg(ui->portnameCombo->currentText()).arg(ui->baudRateCombo->currentText()));
        ui->SndBtn->setEnabled(true);

        ui->openBtn->setText("关闭");
        ui->chart->selfRefresh=false;

        ui->baudRateCombo->setEnabled(false);
        ui->portnameCombo->setEnabled(false);
    } else {
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());

        showStatusMessage(tr("串口打开失败"));
    }
}



void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    ui->SndBtn->setEnabled(false);
    ui->detectBtn->setEnabled(true);
    ui->openBtn->setText("打开");
    ui->chart->selfRefresh=true;
    ui->baudRateCombo->setEnabled(true);
    ui->portnameCombo->setEnabled(true);
    showStatusMessage(tr("串口关闭"));
}


void MainWindow::writeData()
{
    if (ui->SndHex->isChecked()== true)	//“HEX发送” 按钮
    {
        QString sndStr =  ui->SndTextEdit->toPlainText().remove(' ').remove('\n').toUpper();

        if (sndStr.length() % 2 != 0)
        {

              QMessageBox::critical(this, tr("Error"), tr("HEX发送模式下,字符数量\n应为偶数,请注意补零！"));
            return;
        }

        int len = sndStr.length()/2;
        QByteArray tmp1(len*2,' ');
        QByteArray tmp2(len,' ');

        for (int i = 0; i < len*2; i++)
        {
            //对只读数据,at 比 []要快，因为不会导致深层次的拷贝
            if (sndStr.at(i) >= '0' && sndStr.at(i) <= '9')
                tmp1[i] = sndStr.at(i).toLatin1() - '0';           //0~9
            else
                tmp1[i] = sndStr.at(i).toLatin1() - 'A' + 10;      //10~15
        }
        for(int i=0;i<len;i++)
        {
            tmp2[i] = tmp1.at(i * 2) * 16 + tmp1.at(i * 2 + 1);
        }
        m_serial->write(tmp2);

       Stx += static_cast<quint64>(len);
     }
    else		//以字符串形式发送时
    {
        QByteArray tmpQStr=tc->fromUnicode(ui->SndTextEdit->toPlainText().replace('\n',"\r\n").toUtf8());
        m_serial->write(tmpQStr);    //写入数据
        Stx += static_cast<quint64>(tmpQStr.length());
    }
    ui->SndCnt->setText(QString::number(Stx));
}

void MainWindow::readData()
{
    const QByteArray data = m_serial->readAll();

    if(ui->RcvBtn->text()=="暂停" && ui->stackedWidget->currentIndex()==1)
    {
        if(ui->RcvHex->isChecked())
        {
            QString tmpQStr;
            char    tmpByte;
            foreach(char byte ,data)
            {
                tmpByte=(byte&0xF0)>>4;
                tmpQStr += (tmpByte < 10)?tmpByte+'0':tmpByte+'A'-10;

                tmpByte=byte&0x0F;
                tmpQStr += (tmpByte < 10)?tmpByte+'0':tmpByte+'A'-10;
                tmpQStr += ' ';
            }

            ui->RcvTextEdit->insertPlainText(tmpQStr);
        }
        else
        {
            QString tmpQStr = tc->toUnicode(data);
            ui->RcvTextEdit->insertPlainText(tmpQStr);
        }
    }

    ui->RcvTextEdit->verticalScrollBar()->setValue(ui->RcvTextEdit->verticalScrollBar()->maximum());
    Srx += static_cast<quint64>(data.length());
    ui->RcvCnt->setText(QString::number(Srx));

    m_mutex.lock();
    this->m_dataBuf.append(data);
    m_mutex.unlock();

    m_mutex.lock();
    this->processData();
    m_mutex.unlock();
}

void MainWindow::processData()
{
    while (m_dataBuf.length() > 4)//至少要包含头（2字节）+长度（1字节）+校验（1字节）
    {
        //帧头0x44 0x77
        if (m_dataBuf.at(0) == 0x44 && m_dataBuf.at(1) == 0x77)
        {
            int len = m_dataBuf[2];//数据长度,/4之后为变量个数
            if (m_dataBuf.length() < len + 4) break;

            //数据异或校验
            char checksum = 0;
            for (int i = 0; i < len + 3; i++)//len+3表示校验之前的位置
            {
                checksum ^= m_dataBuf[i];
            }
            if (checksum != m_dataBuf[len + 3]) //如果数据校验失败，丢弃这一包数据
            {
                this->m_dataBuf.remove(0, len + 4);//从缓存中删除错误数据
                continue;//继续下一次循环
            }

            //刷新各变量数据

            int j = m_dataBuf.at(2) / 4;
            for (int i = 0; i < j; i++)
            {
                union {
                 float dataFloat;
                 char databyte[4];
                } data;

                data.databyte[0] = m_dataBuf.at(i * 4 + 3);
                data.databyte[1] = m_dataBuf.at(i * 4 + 4);
                data.databyte[2] = m_dataBuf.at(i * 4 + 5);
                data.databyte[3] = m_dataBuf.at(i * 4 + 6);
                this->m_values[i]=data.dataFloat;
                ui->chart->addPoint(i,data.dataFloat);

            }

            if (ui->dataRefreshBox->isChecked() && ui->stackedWidget->currentIndex()==0 )
            {
            //实时显示变量数值
                ui->chart->update();
                this->m_noFreshData=false;
                ui->chart->selfRefresh=false;//有新数据则由主窗口负责刷新，关闭chart自刷新



            }

            this->m_dataBuf.remove(0, len + 4);//正确分析一条数据，从缓存中移除数据。
        }
        else
        {//缓冲中第一个字节不属于帧，丢弃
            this->m_dataBuf.remove(0,1);
            this->m_noFreshData=true;
            ui->chart->selfRefresh=true;
        }
    }
}

void MainWindow::addLabelConnections()
{
    connect(ui->param1,&Varlabel::clicked,this,&MainWindow::handleLabel1);
    connect(ui->param2,&Varlabel::clicked,this,&MainWindow::handleLabel2);
    connect(ui->param3,&Varlabel::clicked,this,&MainWindow::handleLabel3);
    connect(ui->param4,&Varlabel::clicked,this,&MainWindow::handleLabel4);
    connect(ui->param5,&Varlabel::clicked,this,&MainWindow::handleLabel5);
    connect(ui->param6,&Varlabel::clicked,this,&MainWindow::handleLabel6);
    connect(ui->param7,&Varlabel::clicked,this,&MainWindow::handleLabel7);
    connect(ui->param8,&Varlabel::clicked,this,&MainWindow::handleLabel8);
    connect(ui->param9,&Varlabel::clicked,this,&MainWindow::handleLabel9);
    connect(ui->param10,&Varlabel::clicked,this,&MainWindow::handleLabel10);
    connect(ui->param11,&Varlabel::clicked,this,&MainWindow::handleLabel11);
    connect(ui->param12,&Varlabel::clicked,this,&MainWindow::handleLabel12);
    connect(ui->param13,&Varlabel::clicked,this,&MainWindow::handleLabel13);
    connect(ui->param14,&Varlabel::clicked,this,&MainWindow::handleLabel14);
    connect(ui->param15,&Varlabel::clicked,this,&MainWindow::handleLabel15);
    connect(ui->param16,&Varlabel::clicked,this,&MainWindow::handleLabel16);
    connect(ui->param17,&Varlabel::clicked,this,&MainWindow::handleLabel17);
    connect(ui->param18,&Varlabel::clicked,this,&MainWindow::handleLabel18);
    connect(ui->param19,&Varlabel::clicked,this,&MainWindow::handleLabel19);
    connect(ui->param20,&Varlabel::clicked,this,&MainWindow::handleLabel20);

}

void MainWindow::handleLabel1(bool tick)
{
    ui->chart->showSeris[0]=!ui->chart->showSeris[0];

    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[0]);
    ui->val1->setPalette(m_palette);
}
void MainWindow::handleLabel2(bool tick)
{
    ui->chart->showSeris[1]=!ui->chart->showSeris[1];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[1]);
    ui->val2->setPalette(m_palette);
}
void MainWindow::handleLabel3(bool tick)
{
    ui->chart->showSeris[2]=!ui->chart->showSeris[2];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[2]);
    ui->val3->setPalette(m_palette);
}
void MainWindow::handleLabel4(bool tick)
{
    ui->chart->showSeris[3]=!ui->chart->showSeris[3];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[3]);
    ui->val4->setPalette(m_palette);
}
void MainWindow::handleLabel5(bool tick)
{
    ui->chart->showSeris[4]=!ui->chart->showSeris[4];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[4]);
    ui->val5->setPalette(m_palette);
}
void MainWindow::handleLabel6(bool tick)
{
    ui->chart->showSeris[5]=!ui->chart->showSeris[5];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[5]);
    ui->val6->setPalette(m_palette);
}
void MainWindow::handleLabel7(bool tick)
{
    ui->chart->showSeris[6]=!ui->chart->showSeris[6];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[6]);
    ui->val7->setPalette(m_palette);
}
void MainWindow::handleLabel8(bool tick)
{
    ui->chart->showSeris[7]=!ui->chart->showSeris[7];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[7]);
    ui->val8->setPalette(m_palette);
}
void MainWindow::handleLabel9(bool tick)
{
    ui->chart->showSeris[8]=!ui->chart->showSeris[8];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[8]);
    ui->val9->setPalette(m_palette);
}
void MainWindow::handleLabel10(bool tick)
{
    ui->chart->showSeris[9]=!ui->chart->showSeris[9];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[9]);
    ui->val10->setPalette(m_palette);
}

void MainWindow::handleLabel11(bool tick)
{
    ui->chart->showSeris[10]=!ui->chart->showSeris[10];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[10]);
    ui->val11->setPalette(m_palette);
}
void MainWindow::handleLabel12(bool tick)
{
    ui->chart->showSeris[11]=!ui->chart->showSeris[11];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[11]);
    ui->val12->setPalette(m_palette);
}
void MainWindow::handleLabel13(bool tick)
{
    ui->chart->showSeris[12]=!ui->chart->showSeris[12];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[12]);
    ui->val13->setPalette(m_palette);
}
void MainWindow::handleLabel14(bool tick)
{
    ui->chart->showSeris[13]=!ui->chart->showSeris[13];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[13]);
    ui->val14->setPalette(m_palette);
}
void MainWindow::handleLabel15(bool tick)
{
    ui->chart->showSeris[14]=!ui->chart->showSeris[14];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[14]);
    ui->val15->setPalette(m_palette);
}
void MainWindow::handleLabel16(bool tick)
{
    ui->chart->showSeris[15]=!ui->chart->showSeris[15];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[15]);
    ui->val16->setPalette(m_palette);
}
void MainWindow::handleLabel17(bool tick)
{
    ui->chart->showSeris[16]=!ui->chart->showSeris[16];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[16]);
    ui->val17->setPalette(m_palette);
}
void MainWindow::handleLabel18(bool tick)
{
    ui->chart->showSeris[17]=!ui->chart->showSeris[17];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[17]);
    ui->val18->setPalette(m_palette);
}
void MainWindow::handleLabel19(bool tick)
{
    ui->chart->showSeris[18]=!ui->chart->showSeris[18];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[18]);
    ui->val19->setPalette(m_palette);
}
void MainWindow::handleLabel20(bool tick)
{
    ui->chart->showSeris[19]=!ui->chart->showSeris[19];
    m_palette.setColor(QPalette::Base, tick?Qt::white:ui->chart->seriesColor[19]);
    ui->val20->setPalette(m_palette);
}


void MainWindow::handleSettingBtn()
{
    //先传递m_settings显示所需的必要参数 再显示
    m_settings->setInitValue(ui->chart->seriesColor,ui->chart->listYmax,ui->chart->listYmin,m_varNames);
    m_settings->setEnabled(true);
    m_settings->show();
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::handleSettingChanged(QColor *colors, int *mins, int *maxs, QString *names)
{
    for(int i=0;i<20;i++)
    {
        ui->chart->seriesColor[i]=colors[i];
        ui->chart->listYmin[i]=mins[i];
        ui->chart->listYmax[i]=maxs[i];
        m_varNames[i]=names[i];
        if(ui->chart->showSeris[i])
        {
            m_palette.setColor(QPalette::Base,ui->chart->seriesColor[i]);
            m_varLineEdits[i]->setPalette(m_palette);
        }
        m_varLabels[i]->setText(m_varNames[i]);
    }
    m_settings->setEnabled(false);
    m_settings->close();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showStatusMessage(const QString &message)
{
    ui->statusLineEdit->setText(message);
}

void MainWindow::closeEvent(QCloseEvent *e)
{

    if(this->m_settings->isEnabled())this->m_settings->close();
    for(int i=0;i<20;i++)
    {
        m_iniFile->beginGroup("Color");
        m_iniFile->setValue(tr("VAL%1").arg(i+1),QString::number(ui->chart->seriesColor[i].red())+","+QString::number(ui->chart->seriesColor[i].green())
                            +","+QString::number(ui->chart->seriesColor[i].blue()));
        m_iniFile->endGroup();
        m_iniFile->beginGroup("Min");
        m_iniFile->setValue(tr("VAL%1").arg(i+1),ui->chart->listYmin[i]);
        m_iniFile->endGroup();
        m_iniFile->beginGroup("Max");
        m_iniFile->setValue(tr("VAL%1").arg(i+1),ui->chart->listYmax[i]);
        m_iniFile->endGroup();
        m_iniFile->beginGroup("Name");
        m_iniFile->setValue(tr("VAL%1").arg(i+1),m_varNames[i]);
        m_iniFile->endGroup();


    }
    QMainWindow::closeEvent(e);
}

void MainWindow::on_openBtn_clicked()
{
    if(ui->openBtn->text()=="打开")
    {
        this->openSerialPort();
    }
    else
    {
        this->closeSerialPort();
    }
}

void MainWindow::on_detectBtn_clicked()
{
    QString info;
    const auto infos = QSerialPortInfo::availablePorts();
    QStringList allPorts;
    for (const QSerialPortInfo &info : infos) {
        allPorts << info.portName();

    }

    for (int i = 0; i < ui->portnameCombo->count(); i++)
    {
        QString ex = ui->portnameCombo->itemText(i);
        if (allPorts.contains(ex) == false)
        {
            info += ui->portnameCombo->itemText(i) + "移除";
            ui->portnameCombo->removeItem(i);
            i--;
        }
    }
    foreach (QString s , allPorts)
    {
        int i=ui->portnameCombo->count();
        for(;i!=0;i--)
                if(ui->portnameCombo->itemText(i-1) == s) break;
        if (i==0)
        {
            ui->portnameCombo->addItem(s);
            info += "新添" + s;
        }
    }

    if (ui->portnameCombo->currentIndex() == -1 && ui->portnameCombo->count() != 0)
    {
        ui->portnameCombo->setCurrentIndex(0);
    }
    if(ui->portnameCombo->count() == 0)
    {
        info += ("没有可用串口");
    }
    this->showStatusMessage(info);
}

void MainWindow::on_RcvBtn_clicked()
{
    if(ui->RcvBtn->text()=="暂停")
        ui->RcvBtn->setText("显示");
    else
        ui->RcvBtn->setText("暂停");
}

void MainWindow::on_SndClrBtn_clicked()
{
    ui->SndTextEdit->clear();
}

void MainWindow::on_RcvClrBtn_clicked()
{
    ui->RcvTextEdit->clear();
}



void MainWindow::on_SndHex_clicked()
{
    ui->SndTextEdit->hexCheck=true;
    ui->SndTextEdit->clear();
    //不支持非拉丁语输入
    ui->SndTextEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
}

void MainWindow::on_SndASCII_clicked()
{
    ui->SndTextEdit->hexCheck=false;
    ui->SndTextEdit->clear();
    //支持拉丁语输入
    ui->SndTextEdit->setAttribute(Qt::WA_InputMethodEnabled, true);
}



void MainWindow::on_dataRefreshBox_clicked(bool checked)
{
    //只有当串口无(新的)绘图数据或关闭主窗口刷新的时候才会启用 图表自刷新
    ui->chart->selfRefresh= !checked || this->m_noFreshData|| !m_serial->isOpen();
    ui->chart->update();
}

void MainWindow::on_gridBox_clicked(bool checked)
{
    ui->chart->showGrid=checked;
    ui->chart->update();
}

void MainWindow::on_cursorBox_clicked(bool checked)
{
    ui->chart->showCursor=checked;
    ui->chart->update();
}

void MainWindow::on_dataClearBtn_clicked()
{
    ui->chart->init();
}

void MainWindow::on_axisRestoreBtn_clicked()
{
    ui->chart->restoreAxis();
}

void MainWindow::on_dataSaveBtn_clicked()
{

    if(ui->chart->saveData(m_varNames))
    {
        this->showStatusMessage("保存成功");
    }
    else
        this->showStatusMessage("保存失败");

}

void MainWindow::on_dataLoadBtn_clicked()
{
    m_serial->close();
    ui->chart->loadData();
}

void MainWindow::on_prtScBtn_clicked()
{

    QString folder = QCoreApplication::applicationDirPath() + "/SavedImage";
    QString fileName = "/JLU" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz") + ".png";
    QDir dir(folder);

    if (!dir.exists())
    {
        dir.mkdir(folder);
    }
   QPixmap p =  this->grab(QRect(0,0,width(),height()));

   if(p.save(folder + fileName,"png"))
   {
       this->showStatusMessage("保存成功");
   }
   else
   {
       this->showStatusMessage("保存失败");
   }

}


void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    QFile file(":/info/ReadMe.txt");
    file.open(QIODevice::ReadOnly);
    ui->readMe->setPlainText(file.readAll());
    ui->readMe->setReadOnly(true);
}
