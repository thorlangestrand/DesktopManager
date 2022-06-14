#ifndef COMMANDFORMUISTORAGE_H
#define COMMANDFORMUISTORAGE_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

struct InputBox
{
    QLabel* first;
    QLineEdit* second;
    QPushButton* third = nullptr;
};

#endif // COMMANDFORMUISTORAGE_H
