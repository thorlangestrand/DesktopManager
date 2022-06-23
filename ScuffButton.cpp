#include "ScuffButton.h"

ScuffButton::ScuffButton(Command* _cmd)
{
    cmd = _cmd;
}

void ScuffButton::updateCum()
{
    this->setStyleSheet("background-color: rgb(255,220,220);border: none; ");
}
