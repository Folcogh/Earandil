#include "ValidatorSN_suffix.hpp"
#include "Global.hpp"

ValidatorSN_suffix::ValidatorSN_suffix(QObject* parent, bool AcceptEmptyStr)
    : QValidator(parent)
    , AcceptEmptyStr(AcceptEmptyStr)
{
}

QValidator::State ValidatorSN_suffix::validate(QString& input, int& pos) const
{
    (void)pos;

    QValidator::State state = QValidator::Acceptable;

    // Early validation if an emtpy string is allowed
    if (this->AcceptEmptyStr && input.isEmpty()) {
        return state;
    }

    // Odd sizes are tolerated when editing, but not sufficient
    if (input.size() != SN_SUFFIX_LENGTH) {
        state = QValidator::Intermediate;
    }

    // Check if it's a valid integer. Don't use QString::toInt() to skip size limit
    for (int i = 0; i < input.size(); i++) {
        if (!input.at(i).isDigit()) {
            state = QValidator::Invalid;
            break;
        }
    }

    return state;
}
