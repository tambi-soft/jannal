#include "config.h"

Config::Config(QObject *parent) : QObject(parent)
{
    QDir *config_dir = new QDir();
    QFile *config_file = new QFile();
    
    if (QSysInfo::kernelType() == "darwin" || QSysInfo::kernelType() == "linux")
    {
        if (QSysInfo::productType() != "ios" || QSysInfo::productType() != "android")
        {
            config_dir->setPath(QDir::homePath() + "/.jannal/");
            if (! config_dir->exists())
            {
                config_dir->mkdir(config_dir->absolutePath());
            }
        }
    }
    else if (QSysInfo::kernelType() == "winnt")
    {
        config_dir = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        if (! config_dir->exists())
        {
            config_dir->mkdir(config_dir->absolutePath());
        }
    }
        
    config_file->setFileName(config_dir->absolutePath() + "/" + "config.ini");
    if (! config_file->exists())
    {
        QFile::copy(":default_config", config_dir->absolutePath() + "/config.ini");
        QFile::setPermissions(config_file->fileName(),
                              QFileDevice::ReadOwner |
                              QFileDevice::ReadGroup |
                              QFileDevice::ReadOther |
                              QFileDevice::WriteOwner);
    }
    
    this->settings = new QSettings(config_file->fileName(), QSettings::IniFormat);
    //QVariant deckpath_var = settings->value("kulanday/deckpath");
    
}

QSettings *Config::getSettings()
{
    return this->settings;
}

QVariant *Config::getValue(QString selector)
{
    //return this->settings->value(selector);
}
