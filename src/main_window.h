
#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QIcon>
#include <QMouseEvent>
#include <QToolTip>

#include "menubar.h"
#include "config.h"
#include "q_beamer_proxy.h"

class QJannalMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QJannalMainWindow(QWidget *parent = nullptr);
    
    void openFile(QString filepath);
    void runPresentation();
    
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
    
    int tooltip_last_index;
    
private slots:
    void createNewBeamerTab(QString filepath);
    void activateNewTab();
    void closeTab(int tab_id);
    
    
};
