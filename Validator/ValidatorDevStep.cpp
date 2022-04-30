#include "ValidatorDevStep.hpp"
#include <QChar>

ValidatorDevStep::ValidatorDevStep(QObject* parent)
    : QValidator(parent)
{
}

QValidator::State ValidatorDevStep::validate(QString& input, int& pos) const
{
    (void)pos;

    QValidator::State state = QValidator::Acceptable;

    // We don't accept empty string
    if (input.isEmpty()) {
        state = QValidator::Intermediate;
    }

    // We just wants digits
    for (int i = 0; i < input.size(); i++) {
        if (!input.at(i).isDigit()) {
            state = QValidator::Invalid;
            break;
        }
    }

    return state;
}
