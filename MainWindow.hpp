#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QAction>
#include <QMainWindow>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

  private:
    Ui::MainWindow* ui;

    // File menu
    QAction* ActionNew;
    QAction* ActionOpen;
    QAction* ActionSave;
    QAction* ActionSaveAs;
    QMenu* MenuRecent;
    QAction* ActionExit;

    // Documentation menu
    QAction* ActionDocSetPath;      // Change the current documentatin folder. Usefull only if the doc directory move
    QAction* ActionAddDocument;     // Look for unregistered documentations in the doc folder
    QAction* ActionRemoveOrphean;   // Remove DB entries which haven't a doc file
    QAction* ActionUpdateDocuments; // Browse PIV, looking for doc updates

    // Actions triggers
    void actionNewTriggered();
    void actionOpenTriggered();
    void actionSaveTriggered();
    void openRecent(QString filename);
    void actionAddDocumentTriggered();

    void updateUI();
    void updateRecentFileMenu();
};
#endif // MAINWINDOW_HPP
