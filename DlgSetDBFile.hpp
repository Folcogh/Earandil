#ifndef DLGSETDBFILE_HPP
#define DLGSETDBFILE_HPP

#include <QDialog>
#include <QWidget>

namespace Ui {
class DlgSetDBFile;
}

//  DlgSetDBFile
//
// This class
class DlgSetDBFile : public QDialog
{
    Q_OBJECT
  public:
    ~DlgSetDBFile() override;
    static bool newDBFile(QWidget* window, QString& filename, QString& name);

  private:
    Ui::DlgSetDBFile* ui;
    DlgSetDBFile(QWidget* parent);
    void browse();
    void updateButtonOK();
};

#endif // DLGSETDBFILE_HPP
