#include "chart.h"
#include <QPainter>
#include <QRgb>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileDialog>


Chart::Chart(QWidget *parent):
    QWidget(parent)
{
    this->setAutoFillBackground(true);
//    QTimer* timer =new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(drawUpdate()));
//    timer->start(10);

    for(int i=0;i<20;i++)
    {
        listYmax[i]=300;
        listYmin[i]=-300;
    }
    seriesColor[0]=Qt::darkYellow;
    seriesColor[1]=Qt::green;
    seriesColor[2]=Qt::blue;
    seriesColor[3]=Qt::darkGray;
    seriesColor[4]=Qt::gray;
    seriesColor[5]=Qt::cyan;
    seriesColor[6]=Qt::magenta;
    seriesColor[7]=Qt::yellow;
    seriesColor[8]=Qt::darkGreen;
    seriesColor[9]=Qt::darkBlue;
    seriesColor[10]=Qt::darkRed;
    seriesColor[11]=Qt::darkCyan;
    seriesColor[12]=Qt::darkMagenta;
    seriesColor[13]=Qt::darkYellow;
    seriesColor[14]=Qt::lightGray;
    seriesColor[15]=Qt::color0;
    seriesColor[16]=Qt::color1;
    seriesColor[17]=Qt::white;
    seriesColor[18]=Qt::white;
    seriesColor[19]=Qt::white;
}

void Chart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    QPen pen(QColor::fromRgba(qRgba(255,255,255,127)), 1, Qt::DashLine);//, Qt::RoundCap, Qt::RoundJoin);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing, true);

    int width=this->width();
    int widthGridStep=width/xGridNum;
    int height=this->height();
    int heightGridStep=height/yGridNum;

    //辅助显示信息
    p.drawText(width/2,height/2,m_ZoomRatio);
    //画栅格
    if(this->showGrid)
    {
        for(int i=0;i<width;i+=widthGridStep)
        {
            p.drawLine(i,0,i,height);
        }
        for(int j=0;j<height;j+=heightGridStep)
        {
            p.drawLine(0, j, width, j);
        }
    }

    //调整坐标显示
    //选出各系列中横轴最大的值
    int tmpMax;

    m_tmpDrawingTime=selfRefresh?m_tmpDrawingTime:m_drawingTime;

    tmpMax=static_cast<int>((m_tmpDrawingTime>m_ixIniLength)? m_tmpDrawingTime :m_ixIniLength);


    m_ixEnd=static_cast<int>(tmpMax - m_fRollBack);

    m_ixStart=static_cast<int>(m_ixEnd- m_iZoomOutRatio*1.0f / m_iZoomInRatio * m_ixIniLength);

    p.drawText(0,height/2,QString::number(m_ixStart));
    p.drawText(width-30,height/2,QString::number(m_ixEnd));

    //画光标
    if (this->showCursor)
    {
        m_ixCurrent =static_cast<int>( m_ixStart + (m_ixEnd - m_ixStart) * m_fHoverX / width);
        pen.setColor(QColor::fromRgba(qRgba(255,255,0,185)));
        p.setPen(pen);

        p.drawText(static_cast<int>(m_fHoverX),10,QString::number( m_ixCurrent));
        p.drawLine(static_cast<int>(m_fHoverX),0,static_cast<int>(m_fHoverX),height);
    }


    //遍历每条曲线并画出
    pen.setWidth(static_cast<int>(thickerLine)+1);
    pen.setStyle(Qt::SolidLine);
    for (int i = 0; i < 20; i++)
    {
        if (m_flistY[i].size() == 0 || showSeris[i] == false) continue;
        m_pointsToDraw.clear();
        //装载坐标
        if (!changeXYToPoints(i))
        {
            continue;
        }
        //装载颜色
        pen.setColor(seriesColor[i]);
        p.setPen(pen);
        try
        {
            //绘制线
            if (m_pointsToDraw.size() == 1)
                continue;
            p.drawLines(m_pointsToDraw);
        }
        catch (int)
        {
            //发生数据溢出错误
        }
    }
    m_xStarPosUpdate = false;

    emit this->updateUi();
}

void Chart::mousePressEvent(QMouseEvent *e)
{
    this->m_isRolling=true;
    this->m_fOldX=e->x();
}

void Chart::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    this->m_isRolling = false;
    this->m_xStarPosUpdate = true;
    if(this->selfRefresh )
        this->update();
}

void Chart::mouseMoveEvent(QMouseEvent *e)
{
   if (this->m_isRolling == true)
   {
       this->m_xStarPosUpdate = true;
       this->m_fRollBack += (e->x() - this->m_fOldX) *1.0f /this->width() * m_iZoomOutRatio / m_iZoomInRatio * m_ixIniLength;
       this->m_fOldX = e->x();
   }
   this->m_fHoverX = e->x();

   if(this->selfRefresh )
       this->update();
}

void Chart::wheelEvent(QWheelEvent *e)
{

    if (e->delta() > 115)  //放大
    {
        if (m_iZoomOutRatio == 1)
        {
            m_iZoomInRatio *= 2;
            m_ZoomRatio="x"+QString::number(m_iZoomInRatio);
        }
        else
        {
            m_iZoomOutRatio /= 2;
            m_ZoomRatio= (m_iZoomOutRatio==1)?"x1":("x1"+QString::number(m_iZoomOutRatio));
        }
        m_iZoomInRatio = (m_iZoomInRatio > 8) ? 8 : m_iZoomInRatio;
    }
    else if(e->delta() < -115) //缩小
    {
        if (m_iZoomInRatio == 1)
        {
            m_iZoomOutRatio *= 2;
            m_ZoomRatio="x1/"+QString::number(m_iZoomOutRatio);
        }
        else
        {
            m_iZoomInRatio /= 2;
            m_ZoomRatio="x"+QString::number(m_iZoomInRatio);
        }
        m_iZoomOutRatio = (m_iZoomOutRatio > 8) ? 8 : m_iZoomOutRatio;
    }

    m_xStarPosUpdate = true;

    if(this->selfRefresh)
        this->update();
}

void Chart::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->setMouseTracking(true);
}

void Chart::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->setMouseTracking(false);
}


//确保index大于等于0小于20
void Chart::addPoint(int index,float data)
{
    index=(index<20)?index:19;
    if(index==0)m_flistX.append(m_drawingTime);
    m_flistY[index].append(data);
    m_drawingTime+=static_cast<float>(index==0);
}

void Chart::init()
{
    m_flistX.clear();//20个变量统一以时间为横坐标
    for(int i=0;i<20;i++)
    {
        m_flistY[i].clear();
        m_ilistFirstIndex[0]=0;

    }

    m_tmpDrawingTime=0;


    m_drawingTime=0;
    m_tmpEnd=0;
    /*int m_ixStartIni=0;*///初始x的start和end
    m_ixIniLength=512;
    m_ixCurrentLength=512;
    m_ixStart=0;//可能经过缩放和拖动的x的start和end
    m_ixEnd=512;

    m_ixCurrent=0; //光标所在x坐标
    m_iZoomInRatio=1;
    m_iZoomOutRatio=1;
    m_ZoomRatio="x1";

    m_fHoverX=0;

    m_fOldX=0;
    m_fRollBack=0;
    m_isRolling=false;
    m_xStarPosUpdate=true;
}

void Chart::restoreAxis()
{
    m_iZoomInRatio=1;
    m_iZoomOutRatio=1;
    m_ZoomRatio="x1";
    m_isRolling=false;
    m_fRollBack=0;
    m_xStarPosUpdate=true;
}

bool Chart::saveData(QString *names)
{
    QString folder = QCoreApplication::applicationDirPath() + "/SavedData";
    QString fileName = "/JLU" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz") + ".csv";
    QDir dir(folder);

    if (!dir.exists())
    {
        dir.mkdir(folder);
    }

    QFile file(folder+fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        QString content;

        //**变量名**
        content.append("Time"); content.append(',');
        for(int i=0;i<20;i++)
        {
            content.append(names[i]);
            content.append(',');
        }
        content.append('\n');

        //**数据**
        int length=static_cast<int>(m_drawingTime);
        int index;
        for(index=0;index<20;index++)
            if(m_flistY[index].size()==0) break;
        for (int i = 0; i < length; i++)
        {
            content.append(QString::number(static_cast<double>(m_flistX[i])));
            content.append(',');
            for(int j=0;j<index;j++)
            {
                content.append(QString::number(static_cast<double>( m_flistY[j].at(i))));
                content.append(',');
            }
            //先去掉多余的逗号
            //content.remove(content.length()-1);//这里没有去掉多余的逗号，因为去掉之后会出现不可预知的数据保存错误
            content.append('\n');

            file.write(content.toLocal8Bit());
            content.clear();
        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }

}

void Chart::loadData()
{
    this->init();
    QString filePath=QFileDialog::getOpenFileName(this,"open",
                        QCoreApplication::applicationDirPath() + "/SavedData","csv(*.csv)");
    if(!filePath.isEmpty())
    {
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly))
        {
            file.readLine();//第一行 变量名 这里舍弃,也可以读入后显示，这里不做实现

            QString tmp;
            float X=0;
            for (; !(tmp=file.readLine()).isEmpty();)
            {
                QStringList dataStr = tmp.split(',');

                X = dataStr[0].toFloat();
                int paramCount = dataStr.size()-2;//去掉开头的时间变量/////////和多余的‘,’产生的多余数据  参见数据保存函数//

                m_flistX.append(X);//所有变量的时间X索引

                for (int j=0;j<paramCount;j++)
                {

                    m_flistY[j].append(dataStr[j+1].toFloat());
                }
            }
            m_tmpEnd=static_cast<int>(m_tmpDrawingTime =m_drawingTime=X);
        }

    }
}

float Chart::yValue(int seriesIndex,int timeIndex)
{
    return (m_flistY[seriesIndex].size()>timeIndex && timeIndex>0 &&timeIndex<=m_tmpEnd) ? m_flistY[seriesIndex].at(timeIndex) : 0;
}

bool Chart::changeXYToPoints(int index)
{
    QPointF currentPointF(0,0);
    bool first=true;

    m_ilistFirstIndex[index] = (m_xStarPosUpdate) ? 0 : m_ilistFirstIndex[index];//有左滚回或缩放则取0


    //遍历并转换为坐标值
    int length = static_cast<int>(m_drawingTime);
    int i;
    for ( i = m_ilistFirstIndex[index]; i < length; i++)
    {
        if (m_flistX[i] < m_ixStart)
        {
            m_ilistFirstIndex[index] = i;//记录最后一个到横坐标起点的XList项索引
            continue;
        }

        m_tmpEnd=selfRefresh?m_tmpEnd:m_ixEnd;
        if (m_flistX[i] > m_tmpEnd)
        {
            return true;
        }
        m_tmpEnd=static_cast<int>(selfRefresh?m_tmpEnd:m_flistX[i]);

        m_flistY[index][i] = (m_flistY[index][i] > listYmax[index]) ? listYmax[index] : m_flistY[index][i];
        m_flistY[index][i] = (m_flistY[index][i] < listYmin[index]) ? listYmin[index] : m_flistY[index][i];

        //转换为像素坐标
        currentPointF.setX(static_cast<qreal>((m_flistX[i] - m_ixStart) * this->width() *1.0f/ (m_ixEnd - m_ixStart)));
        currentPointF.setY(static_cast<qreal>( this->height() - ((m_flistY[index][i] - listYmin[index]) * this->height() *1.0f/ (listYmax[index] - listYmin[index]))));//*_iZoomRatio;

        //装载坐标
        if(first) {m_pointsToDraw.append(currentPointF);first=false;}
        m_pointsToDraw.append(currentPointF);
        m_pointsToDraw.append(currentPointF);

    }

    return true;
}
