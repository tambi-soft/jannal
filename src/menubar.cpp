#include "menubar.h"

MenuBar::MenuBar(QDir *decks_paths, QMenuBar *parent)
{
    this->decks_paths = decks_paths;
    
    addFileMenu();
}

void MenuBar::addFileMenu()
{
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &MenuBar::quitApplication);
    
    QMenu *fileMenu = addMenu("&File");
    fileMenu->addAction(exitAction);
}


void MenuBar::quitApplication()
{
    QApplication::quit();
}

void MenuBar::emitNewDecksOverviewTab()
{
    emit newDecksOverviewTab();
}
