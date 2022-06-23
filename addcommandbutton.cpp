#include "addcommandbutton.h"

AddCommandButton::AddCommandButton(const QString& text, QWidget * parent) : QPushButton(text, parent)
{

}

AddCommandButton::AddCommandButton(QWidget * parent) : QPushButton(parent)
{

}

void AddCommandButton::keyPressEvent(QKeyEvent* e)
{
    e->ignore();
}
