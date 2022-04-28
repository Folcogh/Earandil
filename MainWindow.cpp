#include "MainWindow.hpp"
#include "./ui_MainWindow.h"
#include "Global.hpp"
#include "Languages.hpp"
#include "Settings.hpp"
#include "WorkingDB.hpp"
#include <QKeySequence>
#include <QMenuBar>
#include <QString>
#include <QStringList>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ActionNew(new QAction("New", this))
    , ActionOpen(new QAction("Open", this))
    , ActionSave(new QAction("Save", this))
    , ActionSaveAs(new QAction("Save as...", this))
    , MenuRecent(new QMenu("Recent...", this))
    , ActionExit(new QAction("Exit", this))
    , ActionDocSetPath(new QAction("Set path", this))
    , ActionDocAdd(new QAction("Add docs", this))
    , ActionDocRemoveOrpheans(new QAction("Remove orpheans", this))
    , ActionDocUpdate(new QAction("Update", this))
{
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE);

    // File menu
    QMenu* MenuFile = new QMenu("File", this);
    MenuFile->addActions(QList<QAction*>({{this->ActionNew, this->ActionOpen, this->ActionSave, this->ActionSaveAs}}));
    MenuFile->addSeparator();
    MenuFile->addMenu(this->MenuRecent);
    MenuFile->addSeparator();
    MenuFile->addAction(this->ActionExit);

    // Documentation menu
    QMenu* MenuDocumentation = new QMenu("Documentation", this);
    MenuDocumentation->addActions(QList<QAction*>({{this->ActionDocSetPath, this->ActionDocAdd, this->ActionDocRemoveOrpheans, this->ActionDocUpdate}}));

    // Create and populate menu bar
    setMenuBar(new QMenuBar);
    menuBar()->addMenu(MenuFile);
    menuBar()->addMenu(MenuDocumentation);

    // Menu connections
    connect(this->ActionNew, &QAction::triggered, [this]() { actionNewTriggered(); });

    // Documentation connections
    connect(this->ActionDocAdd, &QAction::triggered, [this]() { actionDocAddTriggered(); });

    // Make UI consistent
    updateRecentFileMenu();
    updateUI();
}

MainWindow::~MainWindow()
{
    // Delete singleton instances here, because QApplication doesn't always return to main()
    Languages::release();
    Settings::release();
    WorkingDB::release();
    delete ui;
}

void MainWindow::updateRecentFileMenu()
{
    // Clear the menu and get the Recent Database list
    this->MenuRecent->clear();
    QStringList rdb = Settings::instance()->getRecentDatabases();
    if (rdb.isEmpty()) {
        return;
    }

    // Populate the menu by creating an action for each entry
    for (int i = 0; i < rdb.size(); i++) {
        QString filename(rdb.at(i));
        QAction* action = new QAction(filename, this->MenuRecent); // Give parentality to the menu, so clear() can delete the action
        this->MenuRecent->addAction(action);
        connect(action, &QAction::triggered, [this, filename]() { openRecent(filename); });
    }
}

void MainWindow::updateUI()
{    
    if (WorkingDB::instance()->isActive()) {
        this->MenuRecent->setEnabled(Settings::instance()->hasRecentAvailable());
        this->ActionSave->setEnabled(WorkingDB::instance()->isModified());
        this->ActionSaveAs->setEnabled(true);
        this->ActionDocSetPath->setEnabled(true);
        this->ActionDocAdd->setEnabled(true);
        this->ActionDocRemoveOrpheans->setEnabled(WorkingDB::instance()->hasOrpheans());
        this->ActionDocUpdate->setEnabled(true);
    }
    else {
        this->MenuRecent->setEnabled(Settings::instance()->hasRecentAvailable());
        this->ActionSave->setEnabled(false);
        this->ActionSaveAs->setEnabled(false);
        this->ActionDocSetPath->setEnabled(false);
        this->ActionDocAdd->setEnabled(false);
        this->ActionDocRemoveOrpheans->setEnabled(false);
        this->ActionDocUpdate->setEnabled(false);
    }
}

void MainWindow::actionNewTriggered()
{
    // Create the working db
    if (WorkingDB::instance()->newDB(this)) {
        Settings::instance()->addRecentDatabase(WorkingDB::instance()->getFilename());
        updateRecentFileMenu();
        updateUI();
    }
}

void MainWindow::openRecent(QString filename)
{
    (void)filename;
}

void MainWindow::actionDocAddTriggered()
{
    if (WorkingDB::instance()->docAdd(this)) {
        updateUI();
    }
}
