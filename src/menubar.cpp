#include "menubar.h"

MenuBar::MenuBar(QDir *decks_paths, QMenuBar *parent)
{
    this->decks_paths = decks_paths;
    
    addFileMenu();
    addPresentationMenu();
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

void MenuBar::addPresentationMenu()
{
    QAction *presentationAction = new QAction(QIcon::fromTheme("video-display"), "&Run Presentation");
    presentationAction->setShortcut(QKeySequence::fromString("F5"));
    presentationAction->setStatusTip("run presentation");
    connect(presentationAction, &QAction::triggered, this, &MenuBar::emitRunPresentation);
    
    QMenu *presentationMenu = addMenu("&Presentation");
    presentationMenu->addAction(presentationAction);
}


void MenuBar::quitApplication()
{
    QApplication::quit();
}

void MenuBar::emitNewDecksOverviewTab()
{
    emit newDecksOverviewTab();
}

void MenuBar::emitRunPresentation()
{
    emit runPresentation();
}
