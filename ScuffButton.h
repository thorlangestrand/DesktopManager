#ifndef SCUFFBUTTON_H
#define SCUFFBUTTON_H
#include <QPushButton>
#include <QDebug>
#include "Command.h"

class ScuffButton : public QPushButton
{
    Q_OBJECT
public:
    ScuffButton(Command* _cmd);
    ~ScuffButton() {};
public:
    Command* cmd = nullptr;
};

#endif // SCUFFBUTTON_H
