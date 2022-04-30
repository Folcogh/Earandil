#ifndef SETDBFILENAME_HPP
#define SETDBFILENAME_HPP

#include <QDialog>
#include <QWidget>

namespace Ui {
class SetDBFilename;
}

//  SetDBFilename
//
// This class
class SetDBFilename : public QDialog
{
    Q_OBJECT
  public:
    ~SetDBFilename() override;
    static bool newFilename(QWidget* window, QString& filename, QString& name);

  private:
    Ui::SetDBFilename* ui;
    SetDBFilename(QWidget* parent);
    void browse();
    void updateButtonOK();
};

#endif // SETDBFILENAME_HPP
