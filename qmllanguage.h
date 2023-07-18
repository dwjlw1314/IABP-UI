#ifndef QMLLANGUAGE_H
#define QMLLANGUAGE_H

#include <QObject>
#include <QDebug>
#include <QTreeWidget>
#include <QFontDatabase>
#include <QTranslator>
#include <QApplication>
#include <QQmlApplicationEngine>

class qmlLanguage : public QObject
{
    Q_OBJECT
public:
    explicit qmlLanguage(QApplication *app, QQmlApplicationEngine *qmlapp, QObject *parent = nullptr);

    void setFont(QString path);

signals:

public slots:

    /**
     * @brief 设置系统字体类型
     * @par indexOfLanguage type
     * @value 0-> en-US  1-> zh-CN
    */
    void setLocalLanguage(int indexOfLanguage = 0);

private:

    /** @brief 获取系统可支持的字体类型 */
    void showFontFamilies();

    QApplication *m_pApp;
    QQmlApplicationEngine *m_pQmlApp;

};

#endif // QMLLANGUAGE_H
