
#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QIcon>
#include <QMouseEvent>
#include <QToolTip>

#include "menubar.h"
#include "config.h"
#include "q_canvas_widget.h"
//#include "q_beamer_window.h"

class QJannalMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QJannalMainWindow(QWidget *parent = 0);
    
    void showPresentation();
    
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
    void createNewBeamerTab();
    void activateNewTab();
    void closeTab(int tab_id);
    void onTabMoved(int from, int to);
    
};
