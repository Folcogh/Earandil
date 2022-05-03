#include "AddDocumentation.hpp"
#include "EditDocProperty.hpp"
#include "Global.hpp"
#include "WorkingDB.hpp"
#include "ui_AddDocumentation.h"
#include <QAction>
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

AddDocumentation::AddDocumentation(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AddDocumentation)
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
    connect(ui->ListLinked, &QListWidget::itemDoubleClicked, [this]() { openLinkedFile(); });
    connect(ui->ListUnlinked, &QListWidget::itemDoubleClicked, [this]() { openUnlinkedFile(); });

    // Context menu over the lists
    ui->ListLinked->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->ListUnlinked->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* ActionOpenUnlinkedFile       = new QAction("Open file", this);
    QAction* ActionOpenLinkedFile         = new QAction("Open file", this);
    QAction* ActionEditDocumentProperties = new QAction("Edit document properties", this);
    ui->ListUnlinked->addAction(ActionOpenUnlinkedFile);
    ui->ListLinked->addActions(QList<QAction*>({{ActionOpenLinkedFile, ActionEditDocumentProperties}}));

    connect(ActionOpenUnlinkedFile, &QAction::triggered, [this]() { openUnlinkedFile(); });
    connect(ActionOpenLinkedFile, &QAction::triggered, [this]() { openLinkedFile(); });
    connect(ActionEditDocumentProperties, &QAction::triggered, [this, parent]() { editLinkedDoc(parent); });

    // Make UI consistent
    updateButtons();
}

AddDocumentation::~AddDocumentation()
{
    delete ui;
}

void AddDocumentation::updateButtons()
{
    ui->ButtonLink->setDisabled(ui->ListUnlinked->selectedItems().isEmpty());
    ui->ButtonUnlink->setDisabled(ui->ListLinked->selectedItems().isEmpty());
    ui->ButtonOK->setDisabled(ui->ListLinked->count() == 0);
}

QList<DocProperty*> AddDocumentation::addDocumentation(QWidget* parent)
{
    QList<DocProperty*> List;
    AddDocumentation* dlg = new AddDocumentation(parent);
    if (dlg->exec() == QDialog::Accepted) {
        // Populate the list with the newly created Documents
        for (int i = 0; i < dlg->ui->ListLinked->count(); i++) {
            QListWidgetItem* item = dlg->ui->ListLinked->item(i);
            DocProperty* docprop  = item->data(DOC_PROPERTY_ROLE).value<DocProperty*>();
            List.append(docprop);
        }
    }

    delete dlg;
    return List;
}

void AddDocumentation::openUnlinkedFile()
{
    QString path     = WorkingDB::instance()->getDocPath();
    QString filename = ui->ListUnlinked->selectedItems().at(0)->text();
    QString url      = path + QDir::separator() + filename;
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void AddDocumentation::openLinkedFile()
{
    QString path     = WorkingDB::instance()->getDocPath();
    QString filename = ui->ListLinked->selectedItems().at(0)->text();
    QString url      = path + QDir::separator() + filename;
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void AddDocumentation::openFileDirectory()
{
    QString path = WorkingDB::instance()->getDocPath();
    path += QDir::separator();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void AddDocumentation::linkDoc(QWidget* parent)
{
    int row               = ui->ListUnlinked->currentRow();
    QListWidgetItem* item = ui->ListUnlinked->item(row);
    QString text          = item->text();

    // TODO: check if this file matches an existing document to fill the form

    DocProperty* document = EditDocProperty::newDocProperty(parent, text);
    if (document != nullptr) {
        item->setData(DOC_PROPERTY_ROLE, QVariant::fromValue(document));
        ui->ListUnlinked->takeItem(row);
        ui->ListLinked->addItem(item);
        updateButtons();
    }
}

void AddDocumentation::unlinkDoc(QWidget* parent)
{
    int row               = ui->ListLinked->currentRow();
    QListWidgetItem* item = ui->ListLinked->item(row);
    if (QMessageBox::question(parent, WINDOW_TITLE, tr("Do you want to remove the document %1?").arg(item->text())) == QMessageBox::Yes) {
        ui->ListLinked->takeItem(row);
        ui->ListUnlinked->addItem(item);
        delete item->data(DOC_PROPERTY_ROLE).value<DocProperty*>();
    }
}

void AddDocumentation::editLinkedDoc(QWidget* parent)
{
    (void)parent;
}
