
#include "main_window.h"

QJannalMainWindow::QJannalMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
{
    resize(1800, 1000);
    
    Config *config = new Config();
    
    setCentralWidget(tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    this->menu_bar = new MenuBar();
    setMenuBar(menu_bar);
    
    connect(tab_widget, &QTabWidget::tabCloseRequested, this, &QJannalMainWindow::closeTab);
    
    connect(menu_bar, &MenuBar::openFile, this, &QJannalMainWindow::openFile);
    connect(menu_bar, &MenuBar::runPresentation, this, &QJannalMainWindow::runPresentation);
    
    QTabBar *tab_bar = tab_widget->tabBar();
    //connect(tab_bar, &QTabBar::tabMoved, this, &QJannalMainWindow::onTabMoved);
    
    //tab_bar->setMouseTracking(true);
    //tab_bar->installEventFilter(this);
    
    // open the last file if it exists
    QString last_file = config->getSettings()->value("jannal/last_open_path").toString();
    if (QFile(last_file).exists())
    {
        createNewBeamerTab(last_file);
    }
}

void QJannalMainWindow::createNewBeamerTab(QString filepath)
{
    QBeamerProxy *proxy = new QBeamerProxy(filepath);
    proxy->initPresentation();
    
    QFileInfo *file = new QFileInfo(filepath);
    tab_widget->addTab(proxy, file->baseName());
}

void QJannalMainWindow::runPresentation()
{
    QBeamerProxy *proxy = qobject_cast<QBeamerProxy*>(this->tab_widget->currentWidget());
    
    proxy->runPresentation();
}

void QJannalMainWindow::activateNewTab()
{
    tab_widget->setCurrentIndex(tab_widget->count()-1);
}

void QJannalMainWindow::closeTab(int tab_id)
{
    QWidget *tab_to_delete = tab_widget->widget(tab_id);
    tab_widget->removeTab(tab_id);
    tab_to_delete->deleteLater();
}

void QJannalMainWindow::openFile(QString filepath)
{
    createNewBeamerTab(filepath);
}
