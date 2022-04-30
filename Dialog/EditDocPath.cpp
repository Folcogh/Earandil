#include "EditDocPath.hpp"
#include "ui_EditDocPath.h"
#include <QDir>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>

EditDocPath::EditDocPath(QString title, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditDocPath)
{
    ui->setupUi(this);
    setWindowTitle(title);
    adjustSize();

    connect(ui->EditPath, &QLineEdit::textChanged, [this]() { updateButtonOK(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, [this]() { reject(); });
    connect(ui->ButtonOK, &QPushButton::clicked, [this]() { accept(); });
    connect(ui->ButtonBrowse, &QPushButton::clicked, [this]() { browse(); });
}

EditDocPath::~EditDocPath()
{
    delete ui;
}

QString EditDocPath::newPath(QWidget* parent)
{
    QString path;
    EditDocPath* dlg = new EditDocPath("Select documentation path", parent);
    dlg->updateButtonOK(); // Need to call it manually because it won't be triggered on dialog creation

    if (dlg->exec() == QDialog::Accepted) {
        path = dlg->ui->EditPath->text();
    }

    delete dlg;
    return path;
}

QString EditDocPath::editPath(QString path, QWidget* parent)
{
    EditDocPath* Dlg = new EditDocPath("Select documentation path", parent);
    Dlg->ui->EditPath->setText(path);

    if (Dlg->exec() == QDialog::Accepted) {
        path = Dlg->ui->EditPath->text();
    }
    else {
        path = QString();
    }

    delete Dlg;
    return path;
}

void EditDocPath::browse()
{
    // Set a default path if it doesn't exist
    QString path = ui->EditPath->text();
    QDir dir(path);
    if (!dir.exists()) {
        path = QDir::homePath();
    }

    path = QFileDialog::getExistingDirectory(this, "Choose documentation directory", path);
    if (!path.isNull()) {
        ui->EditPath->setText(path);
    }
}

//  updateButtonOK
//
// Button OK is enabled only if the path is valid (and absolute, because else an empty string is valid)
//
void EditDocPath::updateButtonOK()
{
    QString path = ui->EditPath->text();
    QDir dir(path);
    ui->ButtonOK->setEnabled(dir.exists() && dir.isAbsolute());
}
