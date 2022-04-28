#include "DlgDocAdd.hpp"
#include "DlgEditDocument.hpp"
#include "Global.hpp"
#include "WorkingDB.hpp"
#include "ui_DlgDocAdd.h"
#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QToolButton>
#include <QUrl>

DlgDocAdd::DlgDocAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDocAdd)
{
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE);
    adjustSize();

    // Populate list
    ui->ListUnlinked->addItems(WorkingDB::instance()->getUnlinkedDocs());

    // Buttons connections
    connect(ui->ButtonCancel, &QPushButton::clicked, [this]() { reject(); });
    connect(ui->ButtonOK, &QPushButton::clicked, [this]() { accept(); });
    connect(ui->ButtonOpenFileDirectory, &QPushButton::clicked, [this]() { openFileDirectory(); });
    connect(ui->ButtonLink, &QPushButton::clicked, [this]() { linkDoc(this); });
    connect(ui->ButtonUnlink, &QPushButton::clicked, [this]() { unlinkDoc(this); });

    // List handling
    connect(ui->ListLinked, &QListWidget::itemSelectionChanged, [this]() { updateButtons(); });
    connect(ui->ListUnlinked, &QListWidget::itemSelectionChanged, [this]() { updateButtons(); });
    connect(ui->ListLinked, &QListWidget::itemDoubleClicked, [this]() { openFile(); });
    connect(ui->ListUnlinked, &QListWidget::itemDoubleClicked, [this]() { openFile(); });

    // Make UI consistent
    updateButtons();
}

DlgDocAdd::~DlgDocAdd()
{
    delete ui;
}

QList<Document*> DlgDocAdd::addDocumentations(QWidget* parent)
{
    QList<Document*> List;
    DlgDocAdd* dlg = new DlgDocAdd(parent);
    if (dlg->exec() == QDialog::Accepted) {
        // Populate the list with the newly created Documents
        for (int i = 0; dlg->ui->ListLinked->count(); i++) {
            QListWidgetItem* item = dlg->ui->ListLinked->item(i);
            Document* doc         = item->data(DOCUMENT_ROLE).value<Document*>();
            List.append(doc);
        }
    }

    delete dlg;
    return List;
}

void DlgDocAdd::openFile()
{
    QString path     = WorkingDB::instance()->getDocPath();
    QString filename = ui->ListUnlinked->selectedItems().at(0)->text();
    QString url      = path + QDir::separator() + filename;
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void DlgDocAdd::openFileDirectory()
{
    QString path = WorkingDB::instance()->getDocPath();
    path += QDir::separator();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void DlgDocAdd::linkDoc(QWidget* parent)
{
    int row               = ui->ListUnlinked->currentRow();
    QListWidgetItem* item = ui->ListUnlinked->item(row);
    QString text          = item->text();

    // TODO: check if this file matches an existing documentR to fill the form

    Document* document = DlgEditDocument::newDocument(parent, text);
    if (document != nullptr) {
        item->setData(DOCUMENT_ROLE, QVariant::fromValue(document));
        ui->ListUnlinked->takeItem(row);
        ui->ListLinked->addItem(item);
    }
}

void DlgDocAdd::unlinkDoc(QWidget* parent)
{
    int row               = ui->ListLinked->currentRow();
    QListWidgetItem* item = ui->ListLinked->item(row);
    if (QMessageBox::question(parent, WINDOW_TITLE, tr("Do you want to remove the document %1?").arg(item->text())) == QMessageBox::Yes) {
        ui->ListLinked->takeItem(row);
        ui->ListUnlinked->addItem(item);
        delete item->data(DOCUMENT_ROLE).value<Document*>();
    }
}

void DlgDocAdd::updateButtons()
{
    ui->ButtonLink->setDisabled(ui->ListUnlinked->selectedItems().isEmpty());
    ui->ButtonUnlink->setDisabled(ui->ListLinked->selectedItems().isEmpty());
    ui->ButtonOK->setDisabled(ui->ListLinked->count() == 0);
}
