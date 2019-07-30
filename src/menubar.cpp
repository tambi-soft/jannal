#include "menubar.h"

MenuBar::MenuBar(QMenuBar *parent)
{
    addFileMenu();
    addEditMenu();
    addPresentationMenu();
    addHelpMenu();
}

void MenuBar::addFileMenu()
{
    QAction *openAction = new QAction(QIcon::fromTheme("document-open"), "&Open");
    openAction->setShortcut(QKeySequence::fromString("Ctrl+O"));
    openAction->setStatusTip("Open file");
    connect(openAction, &QAction::triggered, this, &MenuBar::emitOpenFile);
    
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit application");
    connect(exitAction, &QAction::triggered, this, &MenuBar::quitApplication);
    
    QMenu *fileMenu = addMenu("&File");
    fileMenu->addAction(openAction);
    fileMenu->addAction(exitAction);
}

void MenuBar::addEditMenu()
{
    QAction *importImagesAction = new QAction("Import Images");
    importImagesAction->setStatusTip("Import multiple Images at once");
    
    
    QAction *generateStepsAction = new QAction("Generate Steps");
    generateStepsAction->setStatusTip("Generate Steps for all Frames");
    
    QMenu *editMenu = addMenu("&Edit");
    editMenu->addAction(importImagesAction);
    editMenu->addAction(generateStepsAction);
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

void MenuBar::addHelpMenu()
{
    QAction *aboutAction = new QAction(QIcon(":logo"), "About Jannal");
    connect(aboutAction, &QAction::triggered, this, &MenuBar::emitAboutTab);
    
    QMenu *menu = addMenu("&Help");
    menu->addAction(aboutAction);
    
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

void MenuBar::emitOpenFile()
{
    QString default_path = QDir::homePath();
    QString filepath = QFileDialog::getOpenFileName(this, ("Open File"), default_path, ("*.json"));
    
    emit openFile(filepath);
}
