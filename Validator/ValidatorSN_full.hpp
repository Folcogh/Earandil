#ifndef VALIDATORSN_FULL_HPP
#define VALIDATORSN_FULL_HPP

#include <QObject>
#include <QString>
#include <QValidator>

class ValidatorSN_full : public QValidator
{
  public:
    ValidatorSN_full(QObject* parent);
    QValidator::State validate(QString& input, int& pos) const override;
};

#endif // VALIDATORSN_FULL_HPP
