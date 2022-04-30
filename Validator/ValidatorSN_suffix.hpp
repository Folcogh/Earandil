#ifndef VALIDATORSN_SUFFIX_HPP
#define VALIDATORSN_SUFFIX_HPP

#include <QObject>
#include <QString>
#include <QValidator>

class ValidatorSN_suffix : public QValidator
{
  public:
    ValidatorSN_suffix(QObject* parent, bool AcceptEmptyStr = true);
    QValidator::State validate(QString& input, int& pos) const override;

  private:
    bool AcceptEmptyStr;
};

#endif // VALIDATORSN_SUFFIX_HPP
