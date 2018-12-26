
#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QIcon>
#include <QMouseEvent>
#include <QToolTip>

#include "menubar.h"
#include "config.h"

class QMarboxMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QMarboxMainWindow(QWidget *parent = 0);
    
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QTabWidget *tab_widget;
    MenuBar *menu_bar;
    
    QDir *deckpath;
    QString last_image_import_path = QDir::homePath();
    QString last_audio_import_path = QDir::homePath();
    
    int SELECT_DECK_INDEX = 0;
    int DECK_OVERVIEW_INDEX = 1;
    int NEW_DECK_INDEX = 2;
    int DECK_LEARN_INDEX = 3;
    int DECK_DIRTY_DOZEN_INDEX = 4;
    
    QMap<QString,int> deck_item_widgets; // {deck_name} OR {deck_name}_{item_id} -> tab widget id
    
    int tooltip_last_index;
    
private slots:
    void activateNewTab();
    void closeTab(int tab_id);
    void onTabMoved(int from, int to);
    
};
