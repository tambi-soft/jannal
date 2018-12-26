
#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QIcon>

#include <src/main_window.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QApplication::setApplicationName("jannal");
    
    QMarboxMainWindow mainWin;
    
#ifdef __linux__
#else
        QIcon::setThemeSearchPaths(QStringList(QList<QString>() << "./Adwaita"));
        QIcon::setThemeName("Adwaita");
#endif
    
    //mainWin.setWindowIcon(QIcon(":logo"));
    mainWin.show();
    return app.exec();
}
