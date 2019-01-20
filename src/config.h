#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QSysInfo>
#include <QSettings>

#include <QDebug>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    
    QVariant *getValue(QString selector);
    
private:
    QSettings *settings;
    QDir *deckpath;
signals:
    
public slots:
};

#endif // CONFIG_H
