#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (argc != 2) return INPUT_PAR_ERR;

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(DP_CPU,&mask);
    //CPU_SET(DS_CPU,&mask);

    sched_setaffinity(0, sizeof(cpu_set_t), &mask);

    QString path = argv[1];

    if (!w.read_file(path))
        return READ_FILE_ERR;

    //qDebug() << "file read finished!";

    w.exportChartPdf();

#ifdef DEBUG_MODE
    w.show();
    return a.exec();
#else
    return 0;
#endif
}
