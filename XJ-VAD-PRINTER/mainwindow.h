#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPainter>
#include <QtCharts>
#include <QPrinter>
#include <QFile>
//#include <QFileInfo>

#include "../datastruct.h"

#define INPUT_PAR_ERR 1     //传入参数错误
#define READ_FILE_ERR 2     //文件读取错误

#define DP_CPU 2
//#define DS_CPU 3

//#define DEBUG_MODE 1

#define GAIN_ECG_SAM 250
#define TOTAL_TIMER 11

#define AXISX_RANGE_MAX 13500.0
#define AXISY_RANGE_MAX 1465.5

#define CHARTVIEW_WIDTH 13600
#define CHARTVIEW_HEIGHT 1532

#define TITLE_Y 40

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /** @brief 打印功能,初始化波形线图表 */
    void initWaveLineChart();

    void exportChartPdf();

    bool read_file(QString path);

private:

    void drawWaveData(const ECG_SHOW_DATA& node, int index);

    void setChartComponent(QLineSeries *series);

    void clearLineSeries(QLineSeries *series);

    void setLineChartMargins(int margin);

    void drawCalibrationRuler(qreal y, qreal h);

    void drawArterialRange(int top, int mid, int bottom);

    void drawTimeLine(qreal x, qreal y, qreal width, int sec);

    void drawBaseData();

    void initPrinter();

private:
    Ui::MainWindow *ui;

    //wave相关
    QChartView *chartView;
    QChart *ecgChart;
    QValueAxis *axisX_bottom;
    QValueAxis *axisY_left;
    QValueAxis *axisX_top;
    QValueAxis *axisY_right;
    //QSplineSeries *splineSeries;
    QLineSeries *ecgLineSeries;
    QLineSeries *arterialLineSeries;
    QLineSeries *balloonLineSeries;

    QFile file;
    QFont painter_font;
    QPrinter *printer;
    QPainter *painter;

    qreal leftMargin;

    qreal h_gain_ecg;
    qreal w_gain_ecg;
    qreal h_gain_art;
    qreal h_gain_bal;

    int high_pressure;
    int low_pressure;
    int avg_pressure;

    int avg_balloon;

    int painter_y;
    int move_right;

    int trigger_start;

    QString pdf_name;
    QQueue<ECG_SHOW_DATA> print_queue;
    PRINTER_BASE_DATA printer_base_data;
};

#endif // MAINWINDOW_H
