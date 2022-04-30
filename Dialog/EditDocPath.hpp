#ifndef EDITDOCPATH_HPP
#define EDITDOCPATH_HPP

#include <QDialog>
#include <QString>

namespace Ui {
class EditDocPath;
}

//  EditDocPath
//
// This class ask for a documentation path, and return a null string is the user cancelled the dialog
// Else return a valid path
//
class EditDocPath : public QDialog
{
    Q_OBJECT

  public:
    static QString newPath(QWidget* parent);
    static QString editPath(QString path, QWidget* parent);

  private:
    Ui::EditDocPath* ui;
    EditDocPath(QString title, QWidget* parent);
    ~EditDocPath() override;
    void browse();
    void updateButtonOK();
};

#endif // EDITDOCPATH_HPP
