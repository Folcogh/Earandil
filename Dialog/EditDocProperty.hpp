#ifndef EDITDOCPROPERTY_HPP
#define EDITDOCPROPERTY_HPP

#include "DocProperty.hpp"
#include <QDialog>
#include <QString>
#include <QWidget>

namespace Ui {
class EditDocProperty;
}

class EditDocProperty : public QDialog
{
    Q_OBJECT

  public:
    static DocProperty* newDocProperty(QWidget* parent, QString filename);
    ~EditDocProperty() override;

  private:
    Ui::EditDocProperty* ui;
    EditDocProperty(QWidget* parent, QString filename);
    void updateButtonOK();
};

#endif // EDITDOCPROPERTY_HPP
