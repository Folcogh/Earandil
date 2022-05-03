#ifndef ADDDOCUMENTATION_HPP
#define ADDDOCUMENTATION_HPP

#include "DocProperty.hpp"
#include <QDialog>
#include <QList>
#include <QWidget>

namespace Ui {
class AddDocumentation;
}

class AddDocumentation : public QDialog
{
    Q_OBJECT

  public:
    ~AddDocumentation() override;
    static QList<DocProperty*> addDocumentation(QWidget* parent);

  private:
    Ui::AddDocumentation* ui;
    AddDocumentation(QWidget* parent);
    void updateButtons();
    void openUnlinkedFile();
    void openLinkedFile();
    void openFileDirectory();
    void linkDoc(QWidget* parent);
    void unlinkDoc(QWidget* parent);
    void editLinkedDoc(QWidget* parent);
};

#endif // ADDDOCUMENTATION_HPP
