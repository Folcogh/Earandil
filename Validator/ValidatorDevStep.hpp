#ifndef VALIDATORDEVSTEP_HPP
#define VALIDATORDEVSTEP_HPP

#include <QObject>
#include <QString>
#include <QValidator>

class ValidatorDevStep : public QValidator
{
  public:
    ValidatorDevStep(QObject* parent);
    QValidator::State validate(QString& input, int& pos) const override;
};

#endif // VALIDATORDEVSTEP_HPP
