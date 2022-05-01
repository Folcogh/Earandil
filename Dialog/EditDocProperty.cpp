#include "EditDocProperty.hpp"
#include "Global.hpp"
#include "Validator/ValidatorDevStep.hpp"
#include "Validator/ValidatorSN_full.hpp"
#include "Validator/ValidatorSN_suffix.hpp"
#include "WorkingDB.hpp"
#include "ui_EditDocProperty.h"
#include <QComboBox>
#include <QLineEdit>

EditDocProperty::EditDocProperty(QWidget* parent, QString filename)
    : QDialog(parent)
    , ui(new Ui::EditDocProperty)
{
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE);
    adjustSize();
    ui->LabelFilename->setText(filename);

    // Set machine list
    ui->ComboMachine->addItems(WorkingDB::instance()->getMachineList());

    // Connections
    connect(ui->ButtonCancel, &QPushButton::clicked, [this]() { reject(); });
    connect(ui->ButtonOK, &QPushButton ::clicked, [this]() {
        WorkingDB::instance()->addMachine(ui->ComboMachine->currentText());
        accept();
    });
    connect(ui->EditDevStep, &QLineEdit::textChanged, [this]() { updateButtonOK(); });
    connect(ui->EditFromSN, &QLineEdit::textChanged, [this]() { updateButtonOK(); });
    connect(ui->EditToSN, &QLineEdit::textChanged, [this]() { updateButtonOK(); });
    connect(ui->ComboMachine, &QComboBox::editTextChanged, [this]() { updateButtonOK(); });
    connect(ui->ComboMachine, &QComboBox::currentIndexChanged, [this]() { updateButtonOK(); });

    // Validation
    ValidatorSN_suffix* validatorsnsuffix = new ValidatorSN_suffix(this);
    ui->EditToSN->setValidator(validatorsnsuffix);

    ValidatorSN_full* validatorsnfull = new ValidatorSN_full(this);
    ui->EditFromSN->setValidator(validatorsnfull);

    ValidatorDevStep* validatordevstep = new ValidatorDevStep(this);
    ui->EditDevStep->setValidator(validatordevstep);

    // Make UI consistent
    updateButtonOK();
}

EditDocProperty::~EditDocProperty()
{
    delete ui;
}

DocProperty* EditDocProperty::newDocProperty(QWidget* parent, QString filename)
{
    DocProperty* docprop = nullptr;
    EditDocProperty* dlg = new EditDocProperty(parent, filename);
    if (dlg->exec() == QDialog::Accepted) {
        filename += DOCUMENTATION_DOT_EXTENSION;
        docprop = new DocProperty(filename,
                                  dlg->ui->ComboMachine->currentText(),
                                  dlg->ui->EditDevStep->text(),
                                  dlg->ui->EditFromSN->text(),
                                  dlg->ui->EditToSN->text(),
                                  IS_NOT_ORPHEAN);
    }

    delete dlg;
    return docprop;
}

void EditDocProperty::updateButtonOK()
{
    // Set button according to inputs
    bool enabled = ui->EditDevStep->hasAcceptableInput() && ui->EditFromSN->hasAcceptableInput() && ui->EditToSN->hasAcceptableInput();

    // We don't want FromSN > ToSN. They can be equal
    // Not applicable if ToSN is empty
    if (ui->EditFromSN->hasAcceptableInput() && ui->EditToSN->hasAcceptableInput() && !ui->EditToSN->text().isEmpty()) {
        QString FromSuffix = ui->EditFromSN->text().last(SN_SUFFIX_LENGTH);
        QString ToSuffix   = ui->EditToSN->text();
        if (FromSuffix.toInt() > ToSuffix.toInt()) {
            enabled = false;
        }
    }

    ui->ButtonOK->setEnabled(enabled);
}
