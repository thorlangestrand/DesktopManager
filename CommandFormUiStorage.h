#ifndef COMMANDFORMUISTORAGE_H
#define COMMANDFORMUISTORAGE_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

/**
 * @brief The InputBox struct
 *
 * Holds pointers to items in the ui
 */
struct InputBox
{
    QLabel* first;
    QLineEdit* second;
    QPushButton* third = nullptr;
};

#endif // COMMANDFORMUISTORAGE_H
