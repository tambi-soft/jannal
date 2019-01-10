#ifndef Q_KULANDAY_MENUBAR_H
#define Q_KULANDAY_MENUBAR_H

#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QTransform>
#include <QImage>
#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QDir *decks_paths, QMenuBar *parent = nullptr);
    
private:
    QDir *decks_paths;
    void addFileMenu();
    void addPresentationMenu();
    
signals:
    void openFile(QString filepath);
    void newDecksOverviewTab();
    void runPresentation();
    
public slots:
    void emitOpenFile();
    void quitApplication();
    void emitNewDecksOverviewTab();
    void emitRunPresentation();
};

#endif // Q_KULANDAY_MENUBAR_H
