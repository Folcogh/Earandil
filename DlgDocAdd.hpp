#ifndef DLGDOCADD_HPP
#define DLGDOCADD_HPP

#include "Document.hpp"
#include <QDialog>
#include <QList>
#include <QWidget>

namespace Ui {
class DlgDocAdd;
}

class DlgDocAdd : public QDialog
{
    Q_OBJECT

  public:
    ~DlgDocAdd() override;
    static QList<Document*> addDocumentations(QWidget* parent);

  private:
    Ui::DlgDocAdd* ui;
    DlgDocAdd(QWidget* parent);
    void updateButtons();
    void openFile();
    void openFileDirectory();
    void linkDoc(QWidget* parent);
    void unlinkDoc(QWidget* parent);
};

#endif // DLGDOCADD_HPP
