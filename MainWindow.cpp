#include "MainWindow.hpp"
#include "./ui_MainWindow.h"
#include "Global.hpp"
#include "Languages.hpp"
#include "QKeySequence"
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
    , ActionAddDocument(new QAction("Add docs", this))
    , ActionRemoveOrphean(new QAction("Remove orpheans", this))
    , ActionUpdateDocuments(new QAction("Update", this))
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
    MenuDocumentation->addActions(QList<QAction*>({{this->ActionDocSetPath, this->ActionAddDocument, this->ActionRemoveOrphean, this->ActionUpdateDocuments}}));

    // Create and populate menu bar
    setMenuBar(new QMenuBar);
    menuBar()->addMenu(MenuFile);
    menuBar()->addMenu(MenuDocumentation);

    // Menu connections
    connect(this->ActionNew, &QAction::triggered, [this]() { actionNewTriggered(); });
    connect(this->ActionOpen, &QAction::triggered, [this]() { actionOpenTriggered(); });
    connect(this->ActionSave, &QAction::triggered, [this]() { actionSaveTriggered(); });

    // Documentation connections
    connect(this->ActionAddDocument, &QAction::triggered, [this]() { actionAddDocumentTriggered(); });

    // Keyboard shortcuts
    this->ActionNew->setShortcut(QKeySequence(QKeySequence::New));
    this->ActionOpen->setShortcut(QKeySequence(QKeySequence::Open));
    this->ActionSave->setShortcut(QKeySequence(QKeySequence::Save));
    this->ActionAddDocument->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));

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
        this->ActionAddDocument->setEnabled(true);
        this->ActionRemoveOrphean->setEnabled(true);
        this->ActionUpdateDocuments->setEnabled(true);
    }
    else {
        this->MenuRecent->setEnabled(Settings::instance()->hasRecentAvailable());
        this->ActionSave->setEnabled(false);
        this->ActionSaveAs->setEnabled(false);
        this->ActionDocSetPath->setEnabled(false);
        this->ActionAddDocument->setEnabled(false);
        this->ActionRemoveOrphean->setEnabled(false);
        this->ActionUpdateDocuments->setEnabled(false);
    }
}

void MainWindow::actionNewTriggered()
{
    if (WorkingDB::instance()->newDB(this)) {
        Settings::instance()->addRecentDatabase(WorkingDB::instance()->getFilename());
        updateRecentFileMenu();
        updateUI();
    }
}

void MainWindow::actionOpenTriggered()
{
    WorkingDB::instance()->openDB(this);
    updateRecentFileMenu();
    updateUI();
}

void MainWindow::actionSaveTriggered()
{
    if (WorkingDB::instance()->saveDB(this)) {
        updateUI();
    }
}

void MainWindow::openRecent(QString filename)
{
    (void)filename;
}

void MainWindow::actionAddDocumentTriggered()
{
    WorkingDB::instance()->addDocument(this);
    updateUI();
}
