/**
 * @brief 心脏辅助反博泵 SG-1000
 * version: 1.0.0.0
 *
 * author  SG-1000
*/

#if 0
#include <QGuiApplication>
#else
#include <QApplication>
#endif

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "dataprocess.h"
#include "qmllanguage.h"
#include "highspeeddata.h"
#include "lowspeeddata.h"
#include "senderdata.h"
#include "datashow.h"

#ifndef ARM
#include "technicaltest.h"
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    //QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qDebug() << "Main ThreadID: " << QThread::currentThreadId();
    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);

#ifndef ARM
    TechnicalTest test;
#endif

    //create language obj
    QSharedPointer<qmlLanguage> lobj = QSharedPointer<qmlLanguage>(new qmlLanguage(&app, &engine));

    //create pubic_data obj
    QSharedPointer<PublicData> pobj = QSharedPointer<PublicData>(new PublicData);

    //create sender_data obj
    QSharedPointer<SenderData> sobj = QSharedPointer<SenderData>(new SenderData);

    QSharedPointer<DataProcess> ecg_proc_obj = QSharedPointer<DataProcess>(new DataProcess);
    ecg_proc_obj->setBaseDataPointer(pobj->getReadSocketfd(), pobj);

    QSharedPointer<DataShow> ecg_show_obj = QSharedPointer<DataShow>(new DataShow);
    ecg_show_obj->setBaseDataPointer(pobj);

    //设置rootContext的变量
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("publicData", pobj.data());
    context->setContextProperty("senderdata", sobj.data());

    qRegisterMetaType<ALARM>("ALARM");

    engine.load(QUrl(QStringLiteral("qrc:/mainWindow/main-show.qml")));
    engine.load(QUrl(QStringLiteral("qrc:/mainWindow/main-control.qml")));

    //create ecg obj
    QSharedPointer<HighSpeedData> ecg = QSharedPointer<HighSpeedData>(new HighSpeedData);
    ecg->setBaseDataPointer(pobj->getWriteSocketfd(), pobj);

    QSharedPointer<LowSpeedData> aux = QSharedPointer<LowSpeedData>(new LowSpeedData);
    aux->setBaseDataPointer(pobj);

    QObject::connect(ecg.data(), SIGNAL(thread_status_change()),
        ecg_proc_obj.data(), SLOT(thread_run()));
    QObject::connect(ecg.data(), SIGNAL(thread_status_change()),
        ecg_show_obj.data(), SLOT(show_timer_change()));
    QObject::connect(aux.data(), SIGNAL(balloon_show(bool)),
        ecg_show_obj.data(), SLOT(show_balloon_change(bool)));
    QObject::connect(aux.data(), SIGNAL(reset_point_num()),
        ecg_show_obj.data(), SLOT(reset_point_num()));

    QObject::connect(sobj.data(), SIGNAL(setLanguageStyle(int)),
        lobj.data(), SLOT(setLocalLanguage(int)));

    QObject::connect(ecg.data(), SIGNAL(ass_serial_init()), aux.data(), SLOT(init()));

    QObject::connect(aux.data(), SIGNAL(ecg_start(bool)), ecg.data(), SLOT(ecg_control(bool)));

    QObject::connect(aux.data(), SIGNAL(resetMainThreadAff(int)), pobj.data(), SLOT(setCpuAffinity(int)));

    sobj->fillThreadObj(aux);

    emit ecg->invoke_signal();

    if (engine.rootObjects().isEmpty())
        return -1;

    pobj->setCpuAffinity(MAIN_CPU);
    return app.exec();
}
