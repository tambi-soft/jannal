
#include "main_window.h"

QMarboxMainWindow::QMarboxMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
{
    resize(1024, 768);
    
    Config *config = new Config();
    this->deckpath = config->getDecksPath();
    
    setCentralWidget(tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    this->menu_bar = new MenuBar(this->deckpath);
    setMenuBar(menu_bar);
    
    connect(tab_widget, &QTabWidget::tabCloseRequested, this, &QMarboxMainWindow::closeTab);
    
    QTabBar *tab_bar = tab_widget->tabBar();
    //connect(tab_bar, &QTabBar::tabMoved, this, &QMarboxMainWindow::onTabMoved);
    
    //tab_bar->setMouseTracking(true);
    //tab_bar->installEventFilter(this);
    
    //showUsersOverviewTab();
}

void QMarboxMainWindow::activateNewTab()
{
    tab_widget->setCurrentIndex(tab_widget->count()-1);
}

void QMarboxMainWindow::closeTab(int tab_id)
{
    QWidget *tab_to_delete = tab_widget->widget(tab_id);
    tab_widget->removeTab(tab_id);
    tab_to_delete->deleteLater();
    
    for (auto k : this->deck_item_widgets.keys())
    {
        int value = this->deck_item_widgets[k];
        // remove key->value pair from the map
        if (value == tab_id)
        {
            this->deck_item_widgets.remove(k);
        }
        // adjust the map to match the new tab ids
        else if (value > tab_id)
        {
            this->deck_item_widgets[k] = value -1;
        }
    }
}

void QMarboxMainWindow::onTabMoved(int from, int to)
{
    // adjust the map to match the new tab ids
    for (auto k : this->deck_item_widgets.keys())
    {
        int value = this->deck_item_widgets[k];
        
        if (value == from)
        {
            this->deck_item_widgets[k] = to;
        }
        if (value == to)
        {
            this->deck_item_widgets[k] = from;
        }
        
    }
}

bool QMarboxMainWindow::eventFilter(QObject *watched, QEvent *event)
{
    /*
    if (watched == this->tab_widget->tabBar())
    {
        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent* mev = static_cast<QMouseEvent*>(event);
            int index = this->tab_widget->tabBar()->tabAt(mev->pos());
            
            if (index != this->tooltip_last_index)
            {
                this->tooltip_last_index = index;
                QString name = this->tab_widget->tabBar()->tabText(index);
                
                QPoint pos = QPoint(mev->x(), -20);
                QToolTip::showText(this->mapToParent(pos), name);
            }
        }
    }
    */
    return QWidget::eventFilter(watched, event);
}
