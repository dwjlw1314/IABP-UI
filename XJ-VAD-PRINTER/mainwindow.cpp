#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("SG-1000 Printer");
    this->setGeometry(0,0, 800, 600);

    move_right = 150;
    painter_y = 1500;
}

MainWindow::~MainWindow()
{
    delete ui;

    delete axisY_left;
    delete axisX_bottom;
    delete axisX_top;
    delete axisY_right;
    delete ecgLineSeries;
    delete arterialLineSeries;
    delete balloonLineSeries;
    delete ecgChart;
    delete chartView;
    delete painter;
    delete printer;
}

bool MainWindow::read_file(QString path)
{
    QFileInfo fileInfo(path);
    QString absolute_path = fileInfo.absolutePath();
    QString filename_without_suffix = fileInfo.baseName();

    pdf_name = absolute_path + "/" + filename_without_suffix + ".pdf";

    file.setFileName(path);

    if (!file.exists()) return false;

    file.open(QFile::ReadOnly);

    if (!file.isOpen()) return false;

    QByteArray data = file.readAll();

    ECG_SHOW_DATA ecg_show_data;

    //uint64_t count = data.size();
    memcpy(&printer_base_data, data.data(), sizeof(PRINTER_BASE_DATA));
    data.remove(0, sizeof(PRINTER_BASE_DATA));

    while(!data.isEmpty())
    {
        memcpy(&ecg_show_data, data.data(), sizeof(ECG_SHOW_DATA));
        data.remove(0, sizeof(ECG_SHOW_DATA));
        print_queue.enqueue(ecg_show_data);
    }

    file.close();

#ifndef DEBUG_MODE
    //file.remove();
#endif

    /** @todo mv增益后期通过算法计算得出 */
    h_gain_ecg = (AXISY_RANGE_MAX / 3) / 70;
    w_gain_ecg = AXISX_RANGE_MAX / (GAIN_ECG_SAM * TOTAL_TIMER);
    h_gain_art = AXISY_RANGE_MAX / (printer_base_data.Amax - printer_base_data.Amin);
    h_gain_bal = AXISY_RANGE_MAX / (printer_base_data.Bmax - printer_base_data.Bmin);
    high_pressure = printer_base_data.Amax;
    low_pressure = printer_base_data.Amin;
    avg_pressure = (printer_base_data.Amax + printer_base_data.Amin) / 2;
    avg_balloon = (printer_base_data.Bmax + printer_base_data.Bmin) / 2;

    initWaveLineChart();

    return true;
}

//设置chartView的边缘宽度
void MainWindow::setLineChartMargins(int margin)
{
    QMargins m_Margin;
    m_Margin.setLeft(margin);
    m_Margin.setBottom(margin);
    m_Margin.setRight(margin);
    m_Margin.setTop(margin);
    ecgChart->setMargins(m_Margin);
    //ecgChart->setContentsMargins(0, 0, 0, 0);
}

void MainWindow::initWaveLineChart()
{
    axisY_left = new QValueAxis;
    axisX_bottom = new QValueAxis;
    axisX_top = new QValueAxis;
    axisY_right = new QValueAxis;
    ecgLineSeries = new QLineSeries;
    arterialLineSeries = new QLineSeries;
    balloonLineSeries = new QLineSeries;
    ecgChart = new QChart;
    chartView = new QChartView(this);

    //设置坐标轴显示范围
    axisX_bottom->setRange(0, AXISX_RANGE_MAX);
    axisY_left->setRange(-AXISY_RANGE_MAX/2, AXISY_RANGE_MAX/2);
    axisX_bottom->setTickCount(276);
    axisY_left->setTickCount(31);
    axisX_top->setRange(0, AXISX_RANGE_MAX);
    axisY_right->setRange(-AXISY_RANGE_MAX/2, AXISY_RANGE_MAX/2);
    axisX_top->setTickCount(56);
    axisY_right->setTickCount(7);

    //设置坐标轴的颜色，样式, 粗细和设置网格显示
    axisX_bottom->setGridLinePen(QPen(Qt::red, 1, Qt::DashDotDotLine, Qt::SquareCap, Qt::RoundJoin));
    axisY_left->setGridLinePen(QPen(Qt::red, 1, Qt::DashDotDotLine, Qt::SquareCap, Qt::RoundJoin));
    axisX_bottom->setLinePen(QPen(Qt::red, 1, Qt::DashDotDotLine, Qt::SquareCap, Qt::RoundJoin));
    axisY_left->setLinePen(QPen(Qt::red, 1, Qt::DashDotDotLine, Qt::SquareCap, Qt::RoundJoin));
    axisX_top->setGridLinePen(QPen(Qt::red, 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    axisY_right->setGridLinePen(QPen(Qt::red, 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    axisX_top->setLinePen(QPen(Qt::red, 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    axisY_right->setLinePen(QPen(Qt::red, 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));

    //显示线框
    axisX_bottom->setGridLineVisible(true);
    axisY_left->setGridLineVisible(true);
    axisX_top->setGridLineVisible(true);
    axisY_right->setGridLineVisible(true);

    //不显示具体数值
    axisX_bottom->setLabelsVisible(false);
    axisY_left->setLabelsVisible(false);
    axisX_top->setLabelsVisible(false);
    axisY_right->setLabelsVisible(false);

    axisX_bottom->setTitleVisible(false);
    axisY_left->setTitleVisible(false);
    axisX_top->setTitleVisible(false);
    axisY_right->setTitleVisible(false);

    //突出的小段线不可见
    axisX_bottom->setLineVisible(false);
    axisY_left->setLineVisible(false);
    axisX_top->setLineVisible(false);
    axisY_right->setLineVisible(false);

    QPen pen;
    pen.setWidth(6);
    pen.setColor(Qt::red);
    ecgLineSeries->setPen(pen);
    ecgLineSeries->setUseOpenGL(true);
    ecgLineSeries->setColor(QColor(Qt::black));

    arterialLineSeries->setPen(pen);
    arterialLineSeries->setUseOpenGL(true);
    arterialLineSeries->setColor(QColor(Qt::black));

    balloonLineSeries->setPen(pen);
    balloonLineSeries->setUseOpenGL(true);
    balloonLineSeries->setColor(QColor(Qt::black));

    //不显示注释
    ecgChart->legend()->hide();
    ecgChart->setBackgroundVisible(false);
    setLineChartMargins(0);

    //把坐标轴添加到chart中，第二个参数是设置坐标轴的位置
    ecgChart->addAxis(axisX_bottom, Qt::AlignBottom);
    ecgChart->addAxis(axisY_left, Qt::AlignLeft);
    ecgChart->addAxis(axisX_top, Qt::AlignTop);
    ecgChart->addAxis(axisY_right, Qt::AlignRight);

    chartView->setGeometry(0, 0, CHARTVIEW_WIDTH, CHARTVIEW_HEIGHT);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(Qt::transparent);
    chartView->setChart(ecgChart);

    setChartComponent(ecgLineSeries);

    initPrinter();
}

void MainWindow::setChartComponent(QLineSeries *series)
{
    //添加曲线到chart中
    ecgChart->addSeries(series);

    //把曲线关联到坐标轴
    series->attachAxis(axisX_bottom);
    series->attachAxis(axisY_left);

    trigger_start = ecgChart->plotArea().x() + move_right;
}

void MainWindow::clearLineSeries(QLineSeries *series)
{
    series->clear();
    ecgChart->removeSeries(series);
}

void MainWindow::drawWaveData(const ECG_SHOW_DATA& node, int index)
{
    qreal x = index * w_gain_ecg;
    qreal ecg_y = node.ecgData * h_gain_ecg;
    qreal art_y = (node.ap_press - avg_pressure) * h_gain_art;
    qreal bal_y = (node.ball_data - avg_balloon) * h_gain_bal;


#ifdef DEBUG_MODE
    //qDebug() << "x-> " << x << " art_y-> " << art_y << " bal_y-> " << bal_y;
#endif

    ecgLineSeries->append(x, ecg_y);
    arterialLineSeries->append(x, art_y);
    balloonLineSeries->append(x, bal_y);

    if (node.trigger_state & '\x01')  //fill
    {
        trigger_start = x + ecgChart->plotArea().x() + move_right;
        //qDebug() << "node.trigger_state-> " << node.trigger_state;
    }
    else if (node.trigger_state & '\x02')  //clear
    {
        int trigger_end = x + ecgChart->plotArea().x() + move_right;
        painter->drawLine(trigger_start, painter_y + 1600, trigger_start, painter_y + 1650);
        painter->drawLine(trigger_start, painter_y + 1650, trigger_end, painter_y + 1650);
        painter->drawLine(trigger_end, painter_y + 1600, trigger_end, painter_y + 1650);
        //qDebug() << "node.trigger_state-> " << node.trigger_state;
    }
}

void MainWindow::initPrinter()
{
    printer = new QPrinter(QPrinter::HighResolution);

    //自定义纸张大小，角度, 特别重要，不然预览效果极差
    printer->setColorMode(QPrinter::Color);
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOrientation(QPrinter::Landscape);

    //printer.setPaperSize(QSizeF(200,210),QPrinter::Millimeter);

    printer->setPageSize(QPrinter::A4);  //210 x 297 mm
    printer->setPaperName("SG-1000");
    printer->setOutputFileName(pdf_name);

#ifdef DEBUG_MODE

    qDebug() << "pageRect-> " << printer->pageRect(QPrinter::DevicePixel);  //289*202mm
    qDebug() << "paperRect-> " << printer->paperRect(QPrinter::DevicePixel);  //297*210mm
    printer->getPageMargins(&leftMargin, nullptr, nullptr, nullptr, QPrinter::DevicePixel);  //3.53mm
    qDebug() << "printer leftMargin-> " << leftMargin;

#endif

    painter = new QPainter(printer);

    QPen pen;
    pen.setWidth(8);
    pen.setColor(Qt::red);
    painter->setPen(pen);

    //设置字体，下面设置字体属性
    painter_font.setFamily("Microsoft YaHei");//字体样式
    painter_font.setPointSize(8);//字体点大小
    painter_font.setItalic(true);//字体是否斜体

    painter->setFont(painter_font);
}

void MainWindow::drawCalibrationRuler(qreal y, qreal h)
{
    painter->drawLine(100, y, 150, y);
    painter->drawLine(100, y, 100, y+h);
    painter->drawLine(100, y+h, 150, y+h);
}

void MainWindow::drawArterialRange(int top, int mid, int bottom)
{
    painter_font.setPointSize(6);//字体点大小
    painter->setFont(painter_font);


    painter->drawText(0, top, QString::number(high_pressure));
    painter->drawText(0, mid, QString::number(avg_pressure));
    painter->drawText(0, bottom, QString::number(low_pressure));

    painter_font.setPointSize(8);//字体点大小
    painter->setFont(painter_font);
}

void MainWindow::drawBaseData()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString(tr("yyyy年MM月dd日 hh:mm:ss"));

    painter->drawText(50, TITLE_Y, current_date);

    painter->drawText(2500, TITLE_Y, "25mm/s");
    painter->drawText(3300, TITLE_Y, "10mm/mv");

    painter->drawText(6000, TITLE_Y, tr("病人信息:"));

    painter->drawText(12000, TITLE_Y, "SN:");
}

void MainWindow::drawTimeLine(qreal x, qreal y, qreal width, int sec)
{
    painter_font.setPointSize(6);//字体点大小
    painter->setFont(painter_font);

    painter->drawText(x+20, y-20, "0s");
    qreal avg = width / 11;
    for(int i = 0; i <= sec; i++)
    {
        painter->drawLine(x+i*avg, y, x+i*avg, y-50);
    }
    painter->drawLine(x, y, x + width, y);
    painter->drawText(x + width - 200, y - 20, "11s");

    painter_font.setPointSize(8);//字体点大小
    painter->setFont(painter_font);
}

void MainWindow::exportChartPdf()
{
    int nums = 0;
    const int total = GAIN_ECG_SAM*TOTAL_TIMER;

    drawBaseData();

    int count = 0;

    while(!print_queue.isEmpty())
    {
        //当页绘制完成，转新页面
        if (1 == count)
        {
            count = 0;
            painter_y = 1500;
            printer->newPage();
        }

        auto node = print_queue.dequeue();

        drawWaveData(node, nums++);

        //x轴 绘制完成 或者 绘制点为空
        if (nums == total || print_queue.isEmpty())
        {
            nums = 0;
            count++;

            QPixmap pix = chartView->grab();

            //获取图像的宽高
            qreal hei = pix.height();

#ifdef DEBUG_MODE

            //QImage qImgTemp;
            //qImgTemp.load("/home/xj/something.png");//用QImage 导入图片
            qDebug() << "pix width -> " << wid << " pix hei -> " << hei;
            qDebug() << "plotArea y -> " << ecgChart->plotArea().y();
            qDebug() << "plotArea h -> " << ecgChart->plotArea().height();
            qDebug() << "chartView h -> " << chartView->height();

#endif
            //pix = pix.scaled(wid,hei,Qt::IgnoreAspectRatio);//将图片宽高扩大两倍，且在矩形内保持宽高比值
            painter->drawText(200, painter_y, tr("心电波形"));
            painter->drawPixmap(move_right, painter_y+50, pix);

            qreal real_y = (ecgChart->plotArea().height() / 3) + painter_y+50 + ecgChart->plotArea().y();

            drawCalibrationRuler(real_y, AXISY_RANGE_MAX/3);

            clearLineSeries(ecgLineSeries);
            setChartComponent(arterialLineSeries);

            pix = chartView->grab();
            painter->drawText(200, painter_y + 350 + hei, tr("血压波形"));
            painter->drawPixmap(move_right, painter_y + 400 + hei, pix);

            //减去25的字体高度
            int top = painter_y + 430 + hei + ecgChart->plotArea().y();
            int mid = (ecgChart->plotArea().height() * 0.5) + painter_y + 430 + hei + ecgChart->plotArea().y();
            int bottom = ecgChart->plotArea().height() + painter_y + 430 + hei + ecgChart->plotArea().y();

            drawArterialRange(top, mid, bottom);

            clearLineSeries(arterialLineSeries);
            setChartComponent(balloonLineSeries);

            pix = chartView->grab();
            painter->drawText(200, painter_y + 550 + 2*hei, tr("球囊波形"));
            painter->drawPixmap(move_right, painter_y + 600 + 2*hei, pix);

            clearLineSeries(balloonLineSeries);
            setChartComponent(ecgLineSeries);

            qreal time_line_x = ecgChart->plotArea().x() + move_right;
            qreal time_line_y = painter_y + 600 + 3*hei + 1000;

            drawTimeLine(time_line_x, time_line_y, ecgChart->plotArea().width(), 11); //11s

            //painter_y += hei * 2 + 750;
        }
    }
    painter->end();
}
