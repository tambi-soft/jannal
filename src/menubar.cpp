#include "menubar.h"

MenuBar::MenuBar(QDir *decks_paths, QMenuBar *parent)
{
    this->decks_paths = decks_paths;
    
    addFileMenu();
    addSearchMenu();
}

void MenuBar::addFileMenu()
{
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &MenuBar::quitApplication);
    
    QMenu *fileMenu = addMenu("&Datei");
    fileMenu->addAction(exitAction);
}

void MenuBar::addSearchMenu()
{
    QAction *searchAction = new QAction(QIcon::fromTheme("system-search"), "&Search");
    searchAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
    searchAction->setStatusTip("search for an item");
    connect(searchAction, &QAction::triggered, this, &MenuBar::emitSearchTab);
    
    QMenu *searchMenu = addMenu("&Suche");
    searchMenu->addAction(searchAction);
}

void MenuBar::quitApplication()
{
    QApplication::quit();
}

void MenuBar::emitNewDecksOverviewTab()
{
    emit newDecksOverviewTab();
}

void MenuBar::emitSearchTab()
{
    emit newSearchTab();
}
