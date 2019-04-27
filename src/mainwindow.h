#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTextCodec>
#include <QMutex>
#include <QSettings>
#include <QLineEdit>
#include "console.h"
#include "varlabel.h"
#include "settingsdialog.h"


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow()override;
    void showStatusMessage(const QString &);

protected:
    void closeEvent(QCloseEvent *e)override;
private slots:
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

    void openSerialPort();
    void closeSerialPort();
//    void about();
    void writeData();
    void readData();

    void handleError(QSerialPort::SerialPortError error);
    void handleSettingBtn();
    void handleSettingChanged(QColor *colors,int *mins,int *maxs,QString *names);

    void on_openBtn_clicked();

    void on_detectBtn_clicked();

    void on_RcvBtn_clicked();

    void on_SndClrBtn_clicked();

    void on_RcvClrBtn_clicked();


    void on_SndHex_clicked();

    void on_SndASCII_clicked();


    void on_dataRefreshBox_clicked(bool checked);

    void on_gridBox_clicked(bool checked);

    void on_cursorBox_clicked(bool checked);

    void on_dataClearBtn_clicked();

    void on_axisRestoreBtn_clicked();

    void on_dataSaveBtn_clicked();

    void on_dataLoadBtn_clicked();

    void on_prtScBtn_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *m_serial = nullptr;
    QTextCodec *tc = QTextCodec::codecForName("System");
    quint64 Stx=0;
    quint64 Srx=0;

    bool m_noFreshData=true;
    float m_values[20]={0};
    QMutex m_mutex;
    QByteArray m_dataBuf;
    QPalette m_palette ;
    QSettings *m_iniFile;
    Settingsdialog *m_settings;

    QString m_varNames[20];

    QList<QLineEdit*> m_varLineEdits;
    QList<Varlabel*> m_varLabels;

    void processData();
    void addLabelConnections();
    void handleLabel1(bool tick);
    void handleLabel2(bool tick);
    void handleLabel3(bool tick);
    void handleLabel4(bool tick);
    void handleLabel5(bool tick);
    void handleLabel6(bool tick);
    void handleLabel7(bool tick);
    void handleLabel8(bool tick);
    void handleLabel9(bool tick);
    void handleLabel10(bool tick);
    void handleLabel11(bool tick);
    void handleLabel12(bool tick);
    void handleLabel13(bool tick);
    void handleLabel14(bool tick);
    void handleLabel15(bool tick);
    void handleLabel16(bool tick);
    void handleLabel17(bool tick);
    void handleLabel18(bool tick);
    void handleLabel19(bool tick);
    void handleLabel20(bool tick);

    void readIni();

    void handleUpdateUi();

};

#endif // MAINWINDOW_H
