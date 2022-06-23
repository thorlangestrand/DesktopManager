#ifndef SCUFFBUTTON_H
#define SCUFFBUTTON_H
#include <QPushButton>
#include <QEvent>
#include <QDebug>

// This is to avoid recursive inclusion but I honestly
// don't know why it works
struct Command;
class ScuffButton : public QPushButton
{
    Q_OBJECT
public:
    ScuffButton(Command* _cmd);
    ~ScuffButton() {};
public:
    Command* cmd = nullptr;
    void updateCum();
};

#endif // SCUFFBUTTON_H
