#ifndef DLGEDITDOCUMENT_HPP
#define DLGEDITDOCUMENT_HPP

#include "Document.hpp"
#include <QDialog>
#include <QString>
#include <QWidget>

namespace Ui {
class DlgEditDocument;
}

class DlgEditDocument : public QDialog
{
    Q_OBJECT

  public:
    static Document* newDocument(QWidget* parent, QString filename);
    ~DlgEditDocument() override;

  private:
    Ui::DlgEditDocument *ui;
    DlgEditDocument(QWidget* parent, QString filename);
    void updateButtonOK();
};

#endif // DLGEDITDOCUMENT_HPP
