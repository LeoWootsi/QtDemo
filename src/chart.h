#ifndef CHART_H
#define CHART_H

#include <QGraphicsView>


class Chart : public QWidget
{
    Q_OBJECT
public:
    explicit Chart(QWidget *parent = nullptr);

signals:
    void updateUi();

protected:
    void paintEvent(QPaintEvent *e) override ;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;
//private slots:
//    void drawUpdate();
public:
    QColor seriesColor[20];
    bool showSeris[20]={1,1,1,1,1 ,1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1};
    int listYmax[20]={0};
    int listYmin[20]={0};

    int xGridNum=10;
    int yGridNum=10;

    bool thickerLine =true;
    bool showGrid=true;
    bool showCursor=true;
    bool selfRefresh=true;

    void addPoint(int index,float data);
    void init();
    void restoreAxis();
    bool saveData(QString *names);
    void loadData();
    float yValue(int seriesIndex,int timeIndex);
    inline int currentCursorX(){return m_ixCurrent;}
private:
    QVector<float> m_flistX;//20个变量统一以时间为横坐标
    QVector<float> m_flistY[20];
    QVector<QPointF> m_pointsToDraw;

    float m_tmpDrawingTime=0;

    int m_ilistFirstIndex[20]={0};
    float m_drawingTime=0;
    int m_tmpEnd=0;
    /*int m_ixStartIni=0;*///初始x的start和end
    int m_ixIniLength=512;
    int m_ixCurrentLength=512;
    int m_ixStart=0;//可能经过缩放和拖动的x的start和end
    int m_ixEnd=512;
    int m_ixCurrent=0; //光标所在x坐标
    int m_iZoomInRatio=1;
    int m_iZoomOutRatio=1;
    QString m_ZoomRatio="x1";

    float m_fHoverX=0;
    float m_fOldX=0;
    float m_fRollBack=0;
    bool m_isRolling=false;
    bool m_xStarPosUpdate=false;


    bool changeXYToPoints(int index);

};

#endif // CHART_H
