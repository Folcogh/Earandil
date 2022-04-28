#ifndef DLGDOCSETPATH_HPP
#define DLGDOCSETPATH_HPP

#include <QDialog>
#include <QString>

namespace Ui {
class DlgDocSetPath;
}

//  DlgDocSetPath
//
// This class ask for a documentation path, and return a null string is the user cancelled the dialog
// Else return a valid path
//
class DlgDocSetPath : public QDialog
{
    Q_OBJECT

  public:
    static QString newPath(QWidget* parent);
    static QString editPath(QString path, QWidget* parent);

  private:
    Ui::DlgDocSetPath* ui;
    DlgDocSetPath(QString title, QWidget* parent);
    ~DlgDocSetPath() override;
    void browse();
    void updateButtonOK();
};

#endif // DLGDOCSETPATH_HPP
