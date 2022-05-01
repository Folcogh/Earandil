#include "ValidatorSN_full.hpp"
#include "Global.hpp"
#include <QChar>

ValidatorSN_full::ValidatorSN_full(QObject* parent)
    : QValidator(parent)
{
}

QValidator::State ValidatorSN_full::validate(QString& input, int& pos) const
{
    (void)pos;
    int Size                = SN_PREFIX_LENGTH + 1 + SN_SUFFIX_LENGTH; // SN is PREFIX + '/' + SUFFIX
    QValidator::State state = QValidator::Acceptable;

    // Odd sizes are tolerated when editing, but not sufficient
    if (input.size() != Size) {
        state = QValidator::Intermediate;
    }
    // If size is ok, we want something valid
    else {
        int i;

        // Check SN prefix
        for (i = 0; i < SN_PREFIX_LENGTH; i++) {
            if (!input.at(i).isDigit()) {
                state = QValidator::Invalid;
                break;
            }
        }

        // Check separator
        if (input.at(i++) != QChar('/')) {
            state = QValidator::Invalid;
        }

        // Check suffix
        for (; i < Size; i++) {
            if (!input.at(i).isDigit()) {
                state = QValidator::Invalid;
                break;
            }
        }
    }
    return state;
}
