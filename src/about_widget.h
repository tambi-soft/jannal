#ifndef ABOUT_WIDGET_H
#define ABOUT_WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>

class AboutWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AboutWidget(QWidget *parent = nullptr);
    
private:
    QVBoxLayout *layout;
    QTabWidget *tab_widget;
    void showAbout();
    void showLibraries();
    void showAuthors();
    void showThanks();
    void showLicense();
    
signals:
    
public slots:
};

#endif // ABOUT_WIDGET_H
