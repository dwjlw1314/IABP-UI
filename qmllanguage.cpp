#include "qmllanguage.h"

qmlLanguage::qmlLanguage(QApplication *app, QQmlApplicationEngine *qmlapp, QObject *parent)
    : QObject(parent), m_pApp(app), m_pQmlApp(qmlapp)
{
    //检测本系统语言自动装载翻译文件
    //使用QLocale类来获取系统本地语言环境, QTranslator的load方法装载语言包
    //QGuiApplication的实例化对象的installTranslator去实现语言翻译

    setLocalLanguage();

    //setFont(QStringLiteral(":/translator/Aileron-Italic-8.ttf"));

    //showFontFamilies();
}

void qmlLanguage::setLocalLanguage(int indexOfLanguage)
{
    QTranslator translator;
    QLocale locale;

    if (0 == indexOfLanguage)
    {
        translator.load(":/en_US.qm");
    }
    else if (1 == indexOfLanguage)
    {
        translator.load(":/zh_CN.qm");
    }

    m_pApp->installTranslator(&translator);
    //重新载入语言包
    m_pQmlApp->retranslate();
}

void qmlLanguage::showFontFamilies()
{
    QFontDatabase database;

    QTreeWidget fontTree;
    fontTree.setColumnCount(2);
    fontTree.setHeaderLabels(QStringList() << "Font" << "Smooth Sizes");

    qDebug() << "Chinese Fonts:";
    foreach(const QString &family, database.families(QFontDatabase::SimplifiedChinese))
    {
        QTreeWidgetItem *familyItem = new QTreeWidgetItem(&fontTree);
        familyItem->setText(0, family);

        foreach (const QString &style, database.styles(family))
        {
            QTreeWidgetItem *styleItem = new QTreeWidgetItem(familyItem);
            styleItem->setText(0, style);

            QString sizes;
            foreach (int points, database.smoothSizes(family, style))
              sizes += QString::number(points) + ' ';

            styleItem->setText(1, sizes.trimmed());
        }
        qDebug() << family;
    }

    qDebug() << "Support Fonts:";
    foreach(const QString &family, database.families())
    {
        QTreeWidgetItem *familyItem = new QTreeWidgetItem(&fontTree);
        familyItem->setText(0, family);

        foreach (const QString &style, database.styles(family))
        {
            QTreeWidgetItem *styleItem = new QTreeWidgetItem(familyItem);
            styleItem->setText(0, style);

            QString sizes;
            foreach (int points, database.smoothSizes(family, style))
              sizes += QString::number(points) + ' ';

            styleItem->setText(1, sizes.trimmed());
        }
        qDebug() << family;
    }
}

void qmlLanguage::setFont(QString path)
{
    int fontId = QFontDatabase::addApplicationFont(path);
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    if (fontFamilies.size() > 0)
    {
        QFont font(fontFamilies[0]); //设置全局字体
        m_pApp->setFont(font);

        qDebug()<<"fontfamilies:"<<fontFamilies;
    }
}
