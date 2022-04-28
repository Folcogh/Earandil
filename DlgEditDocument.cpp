#include "DlgEditDocument.hpp"
#include "Global.hpp"
#include "ui_DlgEditDocument.h"

DlgEditDocument::DlgEditDocument(QWidget* parent, QString filename)
    : QDialog(parent)
    , ui(new Ui::DlgEditDocument)
{
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE);
    adjustSize();
    ui->LabelFilename->setText(filename);

    // Connections
    connect(ui->ButtonCancel, &QPushButton::clicked, [this]() { reject(); });
    connect(ui->ButtonOK, &QPushButton ::clicked, [this]() { accept(); });

    updateButtonOK();
}

DlgEditDocument::~DlgEditDocument()
{
    delete ui;
}

Document* DlgEditDocument::newDocument(QWidget* parent, QString filename)
{
    Document* document   = nullptr;
    DlgEditDocument* dlg = new DlgEditDocument(parent, filename);
    if (dlg->exec() == QDialog::Accepted) {
        // Create Document and populate it
    }

    return document;
}

void DlgEditDocument::updateButtonOK()
{
    ui->ButtonOK->setDisabled(ui->EditDevStep->text().isEmpty() || ui->EditFromSN->text().isEmpty() || ui->EditToSN->text().isEmpty()
                              || (ui->ComboMachine->currentIndex() == -1));
}
