#include "DlgSetDBFile.hpp"
#include "Global.hpp"
#include "ui_DlgSetDBFile.h"
#include <QDir>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>

DlgSetDBFile::DlgSetDBFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetDBFile)
{
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE);
    adjustSize();

    connect(ui->ButtonBrowse, &QPushButton::clicked, [this]() { browse(); });
    connect(ui->EditName, &QLineEdit::textChanged, [this]() { updateButtonOK(); });
    connect(ui->EditFilename, &QLineEdit::textChanged, [this]() { updateButtonOK(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, [this]() { reject(); });
    connect(ui->ButtonOK, &QPushButton::clicked, [this]() { accept(); });
}

DlgSetDBFile::~DlgSetDBFile()
{
    delete ui;
}

bool DlgSetDBFile::newDBFile(QWidget* window, QString& filename, QString& name)
{
    bool ret          = false;
    DlgSetDBFile* dlg = new DlgSetDBFile(window);

    if (dlg->exec() == QDialog::Accepted) {
        filename = dlg->ui->EditFilename->text();
        name     = dlg->ui->EditName->text();
        ret      = true;
    }

    delete dlg;
    return ret;
}

void DlgSetDBFile::browse()
{
    // Default directory: use the existing one if one was already selected, else fall back to default (home path)
    QString dir;
    if (ui->EditFilename->text().isEmpty()) {
        dir = QDir::homePath();
    }
    else {
        QFileInfo info(dir);
        dir = info.absolutePath();
    }

    // Get the filename
    QString filename = QFileDialog::getSaveFileName(this, WINDOW_TITLE " - Select a file", dir, APPLICATION_NAME " database (*." DATABASE_EXTENSION ")");
    if (filename.isNull()) {
        return;
    }

    // Display it if the user validated the dialog
    ui->EditFilename->setText(filename);
}

void DlgSetDBFile::updateButtonOK()
{
    // We want both fields filled to validate the dialog
    bool enabled = !(ui->EditFilename->text().isEmpty() || ui->EditName->text().isEmpty());
    ui->ButtonOK->setEnabled(enabled);
}
